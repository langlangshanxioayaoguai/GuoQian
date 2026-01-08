// RealTimeVariableManager.h
#ifndef REALTIMEVARIABLEMANAGER_H
#define REALTIMEVARIABLEMANAGER_H

#include"variablesystem.h"
#include"variabledatabase.h"
#include <QObject>
#include <QTimer>
#include <QThreadPool>
#include <QReadWriteLock>
#include <QDateTime>
#include <QVariant>
#include <QMap>
#include <QList>

namespace Industrial {

// ==================== 实时变量类 ====================
class RealTimeVariable : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(DataQuality quality READ quality NOTIFY qualityChanged)
    Q_PROPERTY(AlarmLevel alarmLevel READ alarmLevel NOTIFY alarmChanged)

public:
    RealTimeVariable(VariableDefinition *definition, QObject *parent = nullptr);

    QString tagName() const { return m_definition ? m_definition->tagName() : QString(); }
    VariableDefinition* definition() const { return m_definition; }

    // 实时值访问
    QVariant value() const;
    QDateTime timestamp() const { return m_timestamp; }
    DataQuality quality() const { return m_quality; }

    // 报警状态
    AlarmLevel alarmLevel() const { return m_alarmLevel; }
    bool isInAlarm() const { return m_alarmLevel != ALARM_NONE; }
    bool isAcknowledged() const { return m_alarmAcknowledged; }

    // 历史数据
    void addToHistory();
    QVector<QPair<QDateTime, QVariant>> getHistory(int maxPoints = 1000) const;

    // 统计计算
    double averageValue(int seconds = 60) const;
    double maxValue(int seconds = 60) const;
    double minValue(int seconds = 60) const;
    double rateOfChange() const;

signals:
    void valueChanged(const QVariant &value);
    void timestampChanged(const QDateTime &timestamp);
    void qualityChanged(DataQuality quality);
    void alarmChanged(AlarmLevel level);
    void alarmAcknowledged(bool acknowledged);
    void valueOutOfRange(const QVariant &value);

public slots:
    void updateValue(const QVariant &value, DataQuality quality = QUALITY_GOOD);
    void acknowledgeAlarm();
    void resetAlarm();

private:
    void checkAlarm(const QVariant &value);
    void updateQuality(DataQuality quality);

    VariableDefinition *m_definition;

    // 实时值
    QVariant m_value;
    QDateTime m_timestamp;
    DataQuality m_quality;

    // 报警
    AlarmLevel m_alarmLevel;
    bool m_alarmAcknowledged;
    QDateTime m_alarmTime;

    // 历史数据（环形缓冲区）
    struct HistoryPoint {
        QDateTime timestamp;
        QVariant value;
        DataQuality quality;
        AlarmLevel alarmLevel;
    };

    static const int HISTORY_BUFFER_SIZE ; // 1小时数据，每秒一点
    QVector<HistoryPoint> m_history;
    int m_historyIndex;

    // 读写锁
    mutable QReadWriteLock m_lock;
    //------------------------------后加----------
private:
    mutable QMutex m_historyMutex;  // 专用于历史数据的互斥锁
};

// ==================== 实时变量管理器 ====================
class RealTimeVariableManager : public QObject {
    Q_OBJECT

public:
    explicit RealTimeVariableManager(QObject *parent = nullptr);
    ~RealTimeVariableManager();

    bool initialize(VariableDatabase *db);

    // ==================== 变量管理 ====================
    bool addVariable(VariableDefinition *definition);
    bool removeVariable(const QString &tagName);
    RealTimeVariable* getVariable(const QString &tagName) const;
    QList<RealTimeVariable*> getAllVariables() const;

    // ==================== 分组查询 ====================
    QList<RealTimeVariable*> getVariablesByGroup(const QString &groupName) const;
    QList<RealTimeVariable*> getVariablesByArea(const QString &areaCode) const;
    QList<RealTimeVariable*> getVariablesByType(VariableType type) const;

    // ==================== 报警管理 ====================
    QList<RealTimeVariable*> getAlarmVariables() const;
    QList<RealTimeVariable*> getUnacknowledgedAlarms() const;
    void acknowledgeAllAlarms();

    // ==================== 批量操作 ====================
    void updateVariables(const QMap<QString, QVariant> &values);
    void updateVariableQuality(const QString &tagName, DataQuality quality);

    // ==================== 订阅管理 ====================
    void subscribeToVariable(const QString &tagName, QObject *receiver,
                             const char *slot, int updateRate = 0);
    void unsubscribeFromVariable(const QString &tagName, QObject *receiver);

    // ==================== 数据记录 ====================
    void startDataLogging();
    void stopDataLogging();
    void setLoggingInterval(int seconds);


    // ==================== 性能统计 ====================
    struct PerformanceStats {
        int updateCount = 0;
        int alarmCount = 0;
        double avgUpdateRate = 0.0;
        double maxUpdateRate = 0.0;
        int missedUpdates = 0;
        qint64 elapsedTime = 0;
    };

    PerformanceStats getPerformanceStats() const;

    // ==================== 监控控制 ====================
    void startMonitoring();
    void stopMonitoring();
    void pauseMonitoring();
    void resumeMonitoring();
    // ==================== 数据清理 ====================
private:
    void cleanupAllVariables();

signals:
    void variableAdded(const QString &tagName);
    void variableRemoved(const QString &tagName);
    void variableUpdated(const QString &tagName, const QVariant &value);
    void alarmTriggered(const QString &tagName, AlarmLevel level);
    void alarmAcknowledged(const QString &tagName);
    void communicationError(const QString &tagName, const QString &error);
    void performanceStatsUpdated(const PerformanceStats &stats);

public slots:
    void processUpdates();
    void logHistoricalData();
    void cleanupOldData();
    void checkConnectionStatus();
    void onStatsTimerTimeout();

private:
    struct Subscription {
        QObject *receiver = nullptr;
        QByteArray slotName;
        int updateRate = 0;
        QDateTime lastUpdate;
    };

    VariableDatabase *m_database;
    QMap<QString, RealTimeVariable*> m_variables;
    QMap<QString, QList<Subscription>> m_subscriptions;

    // 定时器
    QTimer *m_updateTimer;
    QTimer *m_loggingTimer;
    QTimer *m_cleanupTimer;
    QTimer *m_statsTimer;

    // 线程池
    QThreadPool *m_threadPool;

    // 性能统计
    PerformanceStats m_stats;
    QDateTime m_statsStartTime;

    // 读写锁
    mutable QReadWriteLock m_lock;

    // 内部方法
    void notifySubscribers(const QString &tagName, RealTimeVariable *var);
    void saveToHistoryDatabase(RealTimeVariable *var);
    QString qualityToString(DataQuality quality) const;
    void calculatePerformanceStats();
};

} // namespace Industrial

#endif // REALTIMEVARIABLEMANAGER_H
