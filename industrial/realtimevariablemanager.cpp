// RealTimeVariableManager.cpp

#include"realtimevariablemanager.h"
#include"variabledatabase.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QMutexLocker>
#include <QMetaObject>
#include <cmath>
#include <algorithm>
#include <limits>

namespace Industrial {
// ==================== 静态成员定义 ====================
const int RealTimeVariable::HISTORY_BUFFER_SIZE = 3600; // 1小时数据，每秒一点
// ==================== RealTimeVariable 实现 ====================

RealTimeVariable::RealTimeVariable(VariableDefinition *definition, QObject *parent)
    : QObject(parent)
    , m_definition(definition)
    , m_timestamp(QDateTime::currentDateTime())
    , m_quality(QUALITY_GOOD)
    , m_alarmLevel(ALARM_NONE)
    , m_alarmAcknowledged(true)
    , m_history(HISTORY_BUFFER_SIZE)
    , m_historyIndex(0)
{
    if (m_definition) {
        // 设置初始值
        m_value = m_definition->initialValue();

        // 初始化历史缓冲区
        HistoryPoint initialPoint;
        initialPoint.timestamp = m_timestamp;
        initialPoint.value = m_value;
        initialPoint.quality = m_quality;
        initialPoint.alarmLevel = m_alarmLevel;

        for (int i = 0; i < HISTORY_BUFFER_SIZE; i++) {
            m_history[i] = initialPoint;
        }
    }
}
QVariant RealTimeVariable::value() const
{
    QReadLocker locker(&m_lock);
    return m_value;
}

void RealTimeVariable::updateValue(const QVariant &value, DataQuality quality)
{
    QWriteLocker locker(&m_lock);
    // 检查死区
    if (m_definition && m_definition->deadband() > 0 &&
        m_value.isValid() && value.isValid()) {
        double current = m_value.toDouble();
        double newValue = value.toDouble();
        double deadband = m_definition->deadband();

        if (qAbs(current - newValue) < deadband) {
            // 变化小于死区，不更新
            return;
        }
    }  
    // 更新值
    m_value = value;
    m_timestamp = QDateTime::currentDateTime();

    // 检查质量变化
    if (m_quality != quality) {
        m_quality = quality;
        emit qualityChanged(quality);
    }

    // 检查报警
    checkAlarm(value);

    // 添加到历史
    addToHistory();

    // 释放锁后再发射其他信号
    locker.unlock();

    emit valueChanged(value);
    emit timestampChanged(m_timestamp);
}

void RealTimeVariable::addToHistory()
{
    QMutexLocker locker(&m_historyMutex);  // 保护整个操作
    if (m_historyIndex >= HISTORY_BUFFER_SIZE) {
        m_historyIndex = 0;
    }

    HistoryPoint newPoint;
    newPoint.timestamp = m_timestamp;
    newPoint.value = m_value;
    newPoint.quality = m_quality;
    newPoint.alarmLevel = m_alarmLevel;

    m_history[m_historyIndex] = newPoint;
    m_historyIndex++;
    /*--------------以前的程序-----------
    if (m_historyIndex >= HISTORY_BUFFER_SIZE) {
        m_historyIndex = 0;
    }

    if (m_historyIndex < m_history.size()) {
        m_history[m_historyIndex].timestamp = m_timestamp;
        m_history[m_historyIndex].value = m_value;
        m_history[m_historyIndex].quality = m_quality;
        m_history[m_historyIndex].alarmLevel = m_alarmLevel;

        m_historyIndex++;
    }*/
}

void RealTimeVariable::acknowledgeAlarm()
{
    QWriteLocker locker(&m_lock);
    if (!m_alarmAcknowledged && m_alarmLevel != ALARM_NONE) {
        m_alarmAcknowledged = true;
        locker.unlock();
        emit alarmAcknowledged(true);
    }
}

void RealTimeVariable::resetAlarm()
{
    QWriteLocker locker(&m_lock);
    if (m_alarmLevel != ALARM_NONE) {
        m_alarmLevel = ALARM_NONE;
        m_alarmAcknowledged = true;
        m_alarmTime = QDateTime();
        locker.unlock();
        emit alarmChanged(ALARM_NONE);
    }
}

void RealTimeVariable::checkAlarm(const QVariant &value)
{
    if (!m_definition || m_definition->alarmLevel() == ALARM_NONE) {
        return;
    }

    bool ok = false;
    double val = value.toDouble(&ok);
    if (!ok) {
        // 无法转换为数值，跳过报警检查
        return;
    }
    // 使用快速报警检查（带缓存优化）
    AlarmLevel newLevel = ALARM_NONE;

    if(m_definition){
        // 使用优化后的方法
        newLevel = m_definition->checkAlarmFast(val);
    }
    else{
        if (val <= m_definition->alarmLoLo()) {
            newLevel = ALARM_CRITICAL;
        } else if (val >= m_definition->alarmHiHi()) {
            newLevel = ALARM_CRITICAL;
        } else if (val <= m_definition->alarmLo()) {
            newLevel = ALARM_MAJOR;
        } else if (val >= m_definition->alarmHi()) {
            newLevel = ALARM_MAJOR;
        }
    }
    if (newLevel != m_alarmLevel) {
        m_alarmLevel = newLevel;
        m_alarmAcknowledged = false;
        m_alarmTime = QDateTime::currentDateTime();

        emit alarmChanged(newLevel);
    }
}

void RealTimeVariable::updateQuality(DataQuality quality)
{
    if (quality != m_quality) {
        m_quality = quality;
        emit qualityChanged(quality);
    }
}

QVector<QPair<QDateTime, QVariant>> RealTimeVariable::getHistory(int maxPoints) const
{
    QReadLocker locker(&m_lock);

    int points = qMin(maxPoints, HISTORY_BUFFER_SIZE);
    QVector<QPair<QDateTime, QVariant>> result;
    result.reserve(points);

    // 计算起始索引
    int startIdx = (m_historyIndex - points + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;

    for (int i = 0; i < points; i++) {
        int idx = (startIdx + i) % HISTORY_BUFFER_SIZE;
        if (m_history[idx].timestamp.isValid()) {
            result.append(qMakePair(m_history[idx].timestamp, m_history[idx].value));
        }
    }

    return result;
}

double RealTimeVariable::averageValue(int seconds) const
{
    QReadLocker locker(&m_lock);

    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-seconds);
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < HISTORY_BUFFER_SIZE; i++) {
        if (m_history[i].timestamp >= cutoff && m_history[i].quality == QUALITY_GOOD) {
            bool ok = false;
            double val = m_history[i].value.toDouble(&ok);
            if (ok) {
                sum += val;
                count++;
            }
        }
    }

    return count > 0 ? sum / count : 0.0;
}

double RealTimeVariable::maxValue(int seconds) const
{
    QReadLocker locker(&m_lock);

    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-seconds);
    double maxVal = -std::numeric_limits<double>::max();
    bool found = false;

    for (int i = 0; i < HISTORY_BUFFER_SIZE; i++) {
        if (m_history[i].timestamp >= cutoff && m_history[i].quality == QUALITY_GOOD) {
            bool ok = false;
            double val = m_history[i].value.toDouble(&ok);
            if (ok && val > maxVal) {
                maxVal = val;
                found = true;
            }
        }
    }

    return found ? maxVal : 0.0;
}

double RealTimeVariable::minValue(int seconds) const
{
    QReadLocker locker(&m_lock);

    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-seconds);
    double minVal = std::numeric_limits<double>::max();
    bool found = false;

    for (int i = 0; i < HISTORY_BUFFER_SIZE; i++) {
        if (m_history[i].timestamp >= cutoff && m_history[i].quality == QUALITY_GOOD) {
            bool ok = false;
            double val = m_history[i].value.toDouble(&ok);
            if (ok && val < minVal) {
                minVal = val;
                found = true;
            }
        }
    }

    return found ? minVal : 0.0;
}

double RealTimeVariable::rateOfChange() const
{
    QReadLocker locker(&m_lock);

    if (m_historyIndex < 2) {
        return 0.0;
    }

    int idx1 = (m_historyIndex - 1 + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;
    int idx2 = (m_historyIndex - 2 + HISTORY_BUFFER_SIZE) % HISTORY_BUFFER_SIZE;

    if (!m_history[idx1].timestamp.isValid() || !m_history[idx2].timestamp.isValid()) {
        return 0.0;
    }

    bool ok1 = false, ok2 = false;
    double val1 = m_history[idx1].value.toDouble(&ok1);
    double val2 = m_history[idx2].value.toDouble(&ok2);

    if (!ok1 || !ok2) {
        return 0.0;
    }

    qint64 timeDiff = m_history[idx2].timestamp.msecsTo(m_history[idx1].timestamp);

    if (timeDiff == 0) {
        return 0.0;
    }

    return (val1 - val2) / (timeDiff / 1000.0); // 变化率：单位/秒
}

// ==================== RealTimeVariableManager 实现 ====================

RealTimeVariableManager::RealTimeVariableManager(QObject *parent)
    : QObject(parent)
    , m_database(nullptr)
    , m_updateTimer(new QTimer(this))
    , m_loggingTimer(new QTimer(this))
    , m_cleanupTimer(new QTimer(this))
    , m_statsTimer(new QTimer(this))
    , m_threadPool(new QThreadPool(this))
{
    // 配置线程池
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount() * 2);

    // 连接定时器
    connect(m_updateTimer, &QTimer::timeout, this, &RealTimeVariableManager::processUpdates);
    connect(m_loggingTimer, &QTimer::timeout, this, &RealTimeVariableManager::logHistoricalData);
    connect(m_cleanupTimer, &QTimer::timeout, this, &RealTimeVariableManager::cleanupOldData);
    connect(m_statsTimer, &QTimer::timeout, this, &RealTimeVariableManager::checkConnectionStatus);
    connect(m_statsTimer, &QTimer::timeout, this, &RealTimeVariableManager::onStatsTimerTimeout);

    // 初始化统计
    m_stats = PerformanceStats();
    m_statsStartTime = QDateTime::currentDateTime();
}

RealTimeVariableManager::~RealTimeVariableManager()
{
    stopMonitoring();

    // 增加线程池等待时间，但要有超时
    if (m_threadPool) {
        if (!m_threadPool->waitForDone(2000)) {
            qWarning() << "Thread pool tasks not completed in time";
        }
    }
    cleanupAllVariables();  // 使用新的清理函数

    // 定时器由父对象机制自动删除，不需要手动delete
 /*
    // 1. 先停止所有定时器
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }

    if (m_loggingTimer && m_loggingTimer->isActive()) {
        m_loggingTimer->stop();
    }

    if (m_cleanupTimer && m_cleanupTimer->isActive()) {
        m_cleanupTimer->stop();
    }

    if (m_statsTimer && m_statsTimer->isActive()) {
        m_statsTimer->stop();
    }

     // 2. 停止线程池接受新任务
    if (m_threadPool) {
        m_threadPool->clear();
        m_threadPool->waitForDone(3000);
    }
    // 3. 断开所有信号连接
      disconnect(this, nullptr, nullptr, nullptr);
    if (m_updateTimer) {
        m_updateTimer->disconnect();
    }
    if (m_loggingTimer) {
        m_loggingTimer->disconnect();
    }
     // 4. 清理变量（需要在锁保护下进行）
    QWriteLocker locker(&m_lock);
    // 清理变量
    qDeleteAll(m_variables);
    m_variables.clear();
    m_subscriptions.clear();
    // 5. 删除定时器对象（父对象机制会自动删除，但可以显式删除）
    delete m_updateTimer;
    delete m_loggingTimer;
    delete m_cleanupTimer;
    delete m_statsTimer;
    delete m_threadPool;
    qDebug() << "RealTimeVariableManager destroyed";
*/
}

bool RealTimeVariableManager::initialize(VariableDatabase *db)
{
    m_database = db;
    if (!m_database || !m_database->isInitialized()) {
        qCritical() << "Invalid database";
        return false;
    }

    // 加载所有变量定义并创建实时变量
    QList<VariableDefinition*> allVars = m_database->loadAllVariables();

    QWriteLocker locker(&m_lock);
    for (VariableDefinition *var : allVars) {
        RealTimeVariable *rtVar = new RealTimeVariable(var, this);
        m_variables.insert(var->tagName(), rtVar);
    }

    qInfo() << "Initialized RealTimeVariableManager with" << m_variables.size() << "variables";
    return true;
}

bool RealTimeVariableManager::addVariable(VariableDefinition *definition)
{
    if (!definition) return false;

    QWriteLocker locker(&m_lock);

    QString tagName = definition->tagName();
    if (m_variables.contains(tagName)) {
        qWarning() << "Variable already exists:" << tagName;
        return false;
    }

    RealTimeVariable *rtVar = new RealTimeVariable(definition, this);
    m_variables.insert(tagName, rtVar);

    locker.unlock();

    emit variableAdded(tagName);
    return true;
}

bool RealTimeVariableManager::removeVariable(const QString &tagName)
{
    QWriteLocker locker(&m_lock);

    if (m_variables.contains(tagName)) {
        RealTimeVariable *var = m_variables.take(tagName);
        delete var;

        // 同时移除相关订阅
        m_subscriptions.remove(tagName);

        locker.unlock();
        emit variableRemoved(tagName);
        return true;
    }

    return false;
}

RealTimeVariable* RealTimeVariableManager::getVariable(const QString &tagName) const
{
    QReadLocker locker(&m_lock);
    return m_variables.value(tagName, nullptr);
}

QList<RealTimeVariable*> RealTimeVariableManager::getAllVariables() const
{
    QReadLocker locker(&m_lock);
    return m_variables.values();
}

QList<RealTimeVariable*> RealTimeVariableManager::getVariablesByGroup(const QString &groupName) const
{
    QList<RealTimeVariable*> result;

    if (!m_database) return result;

    VariableGroup *group = m_database->loadVariableGroup(groupName);
    if (!group) return result;

    QList<VariableDefinition*> groupVars = group->variables();

    QReadLocker locker(&m_lock);
    for (VariableDefinition *varDef : groupVars) {
        RealTimeVariable *rtVar = m_variables.value(varDef->tagName());
        if (rtVar) {
            result.append(rtVar);
        }
    }

    delete group;
    return result;
}

QList<RealTimeVariable*> RealTimeVariableManager::getVariablesByArea(const QString &areaCode) const
{
    QList<RealTimeVariable*> result;

    if (!m_database) return result;

    QList<VariableDefinition*> areaVars = m_database->findVariablesByArea(areaCode);

    QReadLocker locker(&m_lock);
    for (VariableDefinition *varDef : areaVars) {
        RealTimeVariable *rtVar = m_variables.value(varDef->tagName());
        if (rtVar) {
            result.append(rtVar);
        }
    }

    return result;
}

QList<RealTimeVariable*> RealTimeVariableManager::getVariablesByType(VariableType type) const
{
    QList<RealTimeVariable*> result;

    QReadLocker locker(&m_lock);
    for (RealTimeVariable *rtVar : m_variables.values()) {
        if (rtVar->definition()->type() == type) {
            result.append(rtVar);
        }
    }

    return result;
}

QList<RealTimeVariable*> RealTimeVariableManager::getAlarmVariables() const
{
    QList<RealTimeVariable*> result;

    QReadLocker locker(&m_lock);
    for (RealTimeVariable *rtVar : m_variables.values()) {
        if (rtVar->isInAlarm()) {
            result.append(rtVar);
        }
    }

    // 按报警级别排序
    std::sort(result.begin(), result.end(),
              [](RealTimeVariable *a, RealTimeVariable *b) {
                  return a->alarmLevel() > b->alarmLevel();
              });

    return result;
}

QList<RealTimeVariable*> RealTimeVariableManager::getUnacknowledgedAlarms() const
{
    QList<RealTimeVariable*> result;

    QReadLocker locker(&m_lock);
    for (RealTimeVariable *rtVar : m_variables.values()) {
        if (rtVar->isInAlarm() && !rtVar->isAcknowledged()) {
            result.append(rtVar);
        }
    }

    return result;
}

void RealTimeVariableManager::acknowledgeAllAlarms()
{
    QList<RealTimeVariable*> alarms = getAlarmVariables();
    for (RealTimeVariable *rtVar : alarms) {
        rtVar->acknowledgeAlarm();
        emit alarmAcknowledged(rtVar->tagName());
    }
}

void RealTimeVariableManager::updateVariables(const QMap<QString, QVariant> &values)
{
    // 使用局部变量收集要更新的变量，减少锁持有时间
    QVector<QPair<QString, QVariant>> updates;
    updates.reserve(values.size());

    {
        QReadLocker locker(&m_lock);  // 改为读锁
        for (auto it = values.begin(); it != values.end(); ++it) {
            if (m_variables.contains(it.key())) {
                updates.append(qMakePair(it.key(), it.value()));
            } else {
                qWarning() << "Variable not found for update:" << it.key();
            }
        }
        m_stats.updateCount += values.size();
    }

    // 在没有锁的情况下启动线程
    for (const auto& update : updates) {
        m_threadPool->start([this, tagName = update.first, value = update.second]() {
            // 在线程内重新获取变量指针
            RealTimeVariable* rtVar = getVariable(tagName);
            if (rtVar) {
                rtVar->updateValue(value);
            }
        });
    }

  /*   // 先收集需要更新的变量指针和值
    QVector<QPair<RealTimeVariable*, QVariant>> updates;
    {
        QReadLocker locker(&m_lock);
        m_stats.updateCount += values.size();

        for (auto it = values.begin(); it != values.end(); ++it) {
            RealTimeVariable *rtVar = m_variables.value(it.key());
            if (rtVar) {
                updates.append(qMakePair(rtVar, it.value()));
            }
        }
    }

    // 在锁外执行异步更新
    for (const auto &update : updates) {
        m_threadPool->start([var = update.first, value = update.second]() {
            var->updateValue(value);
        });
    }

    QWriteLocker locker(&m_lock);  // 这里获取了写锁

    m_stats.updateCount += values.size();

    for (auto it = values.begin(); it != values.end(); ++it) {
        RealTimeVariable *rtVar = m_variables.value(it.key());
        if (rtVar) {
            // 问题：在lambda中释放了锁，但锁对象被捕获
            m_threadPool->start([rtVar, value = it.value()]() {
                rtVar->updateValue(value);  // 这里可能需要访问共享数据
            });
        }
    }*/
}

void RealTimeVariableManager::updateVariableQuality(const QString &tagName, DataQuality quality)
{
    RealTimeVariable *rtVar = getVariable(tagName);
    if (rtVar) {
        rtVar->updateValue(rtVar->value(), quality);
    }
}

void RealTimeVariableManager::subscribeToVariable(const QString &tagName,
                                                  QObject *receiver,
                                                  const char *slot,
                                                  int updateRate)
{
    if (!receiver || !slot) return;

    QWriteLocker locker(&m_lock);

    Subscription sub;
    sub.receiver = receiver;
    //sub.slotName = QMetaObject::normalizedSignature(slot + 1); // 去掉前面的'1'
    // 改为：-----------------------------------
    sub.slotName = QMetaObject::normalizedSignature(slot);
    if (!sub.slotName.isEmpty() && sub.slotName.at(0) == '1') {
        sub.slotName = sub.slotName.mid(1);
    }
    //----------------------------------------------
    sub.updateRate = updateRate;
    sub.lastUpdate = QDateTime::currentDateTime();

    m_subscriptions[tagName].append(sub);

    // 连接destroyed信号，当接收者被销毁时自动取消订阅
    connect(receiver, &QObject::destroyed, this, [this, tagName, receiver]() {
        unsubscribeFromVariable(tagName, receiver);
    }, Qt::UniqueConnection);
}

void RealTimeVariableManager::unsubscribeFromVariable(const QString &tagName,
                                                      QObject *receiver)
{
    QWriteLocker locker(&m_lock);

    if (m_subscriptions.contains(tagName)) {
        QList<Subscription> &subs = m_subscriptions[tagName];
        auto it = std::remove_if(subs.begin(), subs.end(),
                                 [receiver](const Subscription &sub) {
                                     return sub.receiver == receiver;
                                 });
        subs.erase(it, subs.end());

        if (subs.isEmpty()) {
            m_subscriptions.remove(tagName);
        }
    }
}

void RealTimeVariableManager::startDataLogging()
{
    if (!m_loggingTimer->isActive()) {
        m_loggingTimer->start(60000); // 每分钟记录一次
        qDebug() << "Started data logging (1 minute interval)";
    }
}

void RealTimeVariableManager::stopDataLogging()
{
    if (m_loggingTimer->isActive()) {
        m_loggingTimer->stop();
        qDebug() << "Stopped data logging";
    }
}

void RealTimeVariableManager::setLoggingInterval(int seconds)
{
    if (seconds < 1) seconds = 1;

    bool wasActive = m_loggingTimer->isActive();
    m_loggingTimer->stop();
    m_loggingTimer->setInterval(seconds * 1000);

    if (wasActive) {
        m_loggingTimer->start();
        qDebug() << "Updated logging interval to" << seconds << "seconds";
    }
}

RealTimeVariableManager::PerformanceStats RealTimeVariableManager::getPerformanceStats() const
{
    QReadLocker locker(&m_lock);
    return m_stats;
}

void RealTimeVariableManager::startMonitoring()
{
    if (!m_updateTimer->isActive()) {
        m_updateTimer->start(100); // 100ms 更新周期
        qDebug() << "Started update monitoring (100ms interval)";
    }

    if (!m_cleanupTimer->isActive()) {
        m_cleanupTimer->start(3600000); // 每小时清理一次
        qDebug() << "Started cleanup timer (1 hour interval)";
    }

    if (!m_statsTimer->isActive()) {
        m_statsTimer->start(10000); // 每10秒更新统计
        qDebug() << "Started stats timer (10 second interval)";
    }

    if (!m_loggingTimer->isActive()) {
        m_loggingTimer->start(60000); // 每分钟记录一次
        qDebug() << "Started data logging (1 minute interval)";
    }
}

void RealTimeVariableManager::stopMonitoring()
{
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }

    if (m_loggingTimer && m_loggingTimer->isActive()) {
        m_loggingTimer->stop();
    }

    if (m_cleanupTimer && m_cleanupTimer->isActive()) {
        m_cleanupTimer->stop();
    }

    if (m_statsTimer && m_statsTimer->isActive()) {
        m_statsTimer->stop();
    }

    // 清理线程池中的任务
    if (m_threadPool) {
        m_threadPool->clear();
        m_threadPool->waitForDone(3000); // 最多等待3秒
    }

    qDebug() << "RealTimeVariableManager monitoring stopped";
}

void RealTimeVariableManager::pauseMonitoring()
{
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
        qDebug() << "Paused update monitoring";
    }
}

void RealTimeVariableManager::resumeMonitoring()
{
    if (m_updateTimer && !m_updateTimer->isActive()) {
        m_updateTimer->start();
        qDebug() << "Resumed update monitoring";
    }
}
//------------------新添加-----------------------
void RealTimeVariableManager::cleanupAllVariables()
{
    QWriteLocker locker(&m_lock);
    QList<RealTimeVariable*> varsToDelete = m_variables.values();
    m_variables.clear();
    m_subscriptions.clear();
    locker.unlock();  // 尽早释放锁

    qDeleteAll(varsToDelete);  // 在无锁状态下删除
}

void RealTimeVariableManager::processUpdates()
{
    QReadLocker locker(&m_lock);

    // 处理订阅通知
    QDateTime now = QDateTime::currentDateTime();

    for (auto it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it) {
        const QString& tagName = it.key();
        RealTimeVariable *rtVar = m_variables.value(tagName);
        if (!rtVar) continue;

        QList<Subscription> &subs = it.value();
        for (Subscription &sub : subs) {
            // 检查更新频率
            if (sub.updateRate > 0) {
                qint64 elapsed = sub.lastUpdate.msecsTo(now);
                if (elapsed < sub.updateRate) {
                    continue;
                }
            }

            // 通知订阅者
            QVariant value = rtVar->value();
            if (sub.receiver && !sub.slotName.isEmpty()) {
                QMetaObject::invokeMethod(sub.receiver, sub.slotName.constData(),
                                          Qt::QueuedConnection,
                                          Q_ARG(QVariant, value),
                                          Q_ARG(QDateTime, rtVar->timestamp()),
                                          Q_ARG(QString, tagName));
            }

            sub.lastUpdate = now;
        }
    }
}

void RealTimeVariableManager::logHistoricalData()
{
    QReadLocker locker(&m_lock);

    for (RealTimeVariable *rtVar : m_variables.values()) {
        // 检查是否需要记录历史
        if (rtVar->definition() && rtVar->definition()->historyEnabled()) {
            saveToHistoryDatabase(rtVar);
        }
    }
}

void RealTimeVariableManager::cleanupOldData()
{
    QWriteLocker locker(&m_lock);

    QDateTime cutoffTime = QDateTime::currentDateTime().addDays(-30); // 清理30天前的数据

    // 这里可以实现数据清理逻辑
    // 例如：从历史数据库删除旧记录

    qDebug() << "Cleanup old data completed";
}

void RealTimeVariableManager::checkConnectionStatus()
{
    QReadLocker locker(&m_lock);

    int activeCount = 0;
    int errorCount = 0;

    for (RealTimeVariable *rtVar : m_variables.values()) {
        if (rtVar) {
            if (rtVar->quality() == QUALITY_GOOD) {
                activeCount++;
            } else if (rtVar->quality() == QUALITY_COMM_FAIL ||
                       rtVar->quality() == QUALITY_BAD) {
                errorCount++;
                QString errorMsg = qualityToString(rtVar->quality());
                emit communicationError(rtVar->tagName(), errorMsg);
            }
        }
    }

    if (errorCount > 0) {
        qWarning() << "Connection check:" << errorCount << "variables have communication errors";
    }
}

void RealTimeVariableManager::onStatsTimerTimeout()
{
    calculatePerformanceStats();
    emit performanceStatsUpdated(m_stats);
}

void RealTimeVariableManager::calculatePerformanceStats()
{
    PerformanceStats newStats;

    // 获取当前统计值
    {
        QReadLocker locker(&m_lock);
        newStats.updateCount = m_stats.updateCount;
        newStats.missedUpdates = m_stats.missedUpdates;
        newStats.maxUpdateRate = m_stats.maxUpdateRate;
    }

    // 计算报警数量
    {
        QReadLocker locker(&m_lock);
        newStats.alarmCount = 0;
        for (RealTimeVariable *rtVar : m_variables.values()) {
            if (rtVar && rtVar->isInAlarm()) {
                newStats.alarmCount++;
            }
        }
    }

    // 计算运行时间和平均更新率
    qint64 elapsed = m_statsStartTime.msecsTo(QDateTime::currentDateTime());
    newStats.elapsedTime = elapsed;

    if (elapsed > 0) {
        newStats.avgUpdateRate = static_cast<double>(newStats.updateCount) * 1000.0 / static_cast<double>(elapsed);
    } else {
        newStats.avgUpdateRate = 0.0;
    }

    // 更新最大更新率
    if (newStats.avgUpdateRate > newStats.maxUpdateRate) {
        newStats.maxUpdateRate = newStats.avgUpdateRate;
    }

    // 更新成员变量
    {
        QWriteLocker locker(&m_lock);
        m_stats = newStats;
    }
}

void RealTimeVariableManager::notifySubscribers(const QString &tagName, RealTimeVariable *var)
{
    Q_UNUSED(tagName);
    Q_UNUSED(var);
    // 这个函数现在由processUpdates统一处理
}

void RealTimeVariableManager::saveToHistoryDatabase(RealTimeVariable *var)
{
    if (!m_database) return;

    // 这里可以实现将实时数据保存到历史数据库的逻辑
    // 例如：使用VariableDatabase的historyDatabase()
    Q_UNUSED(var);
}

QString RealTimeVariableManager::qualityToString(DataQuality quality) const
{
    switch (quality) {
    case QUALITY_GOOD: return "Good";
    case QUALITY_BAD: return "Bad";
    case QUALITY_UNCERTAIN: return "Uncertain";
    case QUALITY_OLD: return "Old";
    case QUALITY_COMM_FAIL: return "Communication Failed";
    case QUALITY_OUT_RANGE: return "Out of Range";
    case QUALITY_SENSOR_FAIL: return "Sensor Failed";
    case QUALITY_CALIBRATION: return "Calibrating";
    case QUALITY_MAINTENANCE: return "Maintenance";
    default: return "Unknown";
    }
}

} // namespace Industrial
