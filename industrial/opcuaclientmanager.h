// OPCUAClientManager.h - 整合版本
#ifndef OPCUACLIENTMANAGER_H
#define OPCUACLIENTMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMap>
#include <QVariant>
#include <QList>
#include <QPair>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QThreadPool>
#include <QRunnable>
#include <atomic>
#include <QHash>
#include <memory>
#include <cmath>
#include "variablesystem.h"
#include <QMutexLocker>
#include <QVariant>
#include <QUuid>
// 条件编译确保 open62541.h 只包含一次
#ifndef OPEN62541_H_INCLUDED
#include "open62541.h"
#define OPEN62541_H_INCLUDED
#endif

namespace Industrial {

// ==================== 类型定义 ====================

// OPC UA 订阅监控模式
enum SubscriptionMode {
    SUBSCRIPTION_POLLING = 0,      // 轮询模式
    SUBSCRIPTION_MONITORED = 1     // 监控项模式（推荐）
};

// 连接状态
enum ConnectionState {
    STATE_DISCONNECTED = 0,// 初始状态，未连接
    STATE_CONNECTING,// 正在连接中
    STATE_CONNECTED,// 已连接，正常工作
    STATE_RECONNECTING, // 连接丢失，正在重连
    STATE_ERROR// 发生错误
};

// 操作类型
enum OperationType {
    OP_READ_SINGLE = 0,//读取单个变量
    OP_READ_BATCH = 1,//批量读取变量
    OP_WRITE_SINGLE = 2,//写入单个变量
    OP_WRITE_BATCH = 3,//批量写入变量
    OP_CONNECT = 4,//连接操作
    OP_DISCONNECT = 5,//断开连接操作
    OP_BROWSE = 6,//浏览节点操作
    OP_KEEPALIVE = 7//心跳保持操作
};

// 重连策略
struct ReconnectPolicy {
    int initialDelay = 1000;        // 初始重连延迟(ms)
    int maxDelay = 60000;           // 最大重连延迟(ms)
    int delayMultiplier = 2;        // 延迟乘数（指数退避）
    int maxRetries = 30;            // 最大重试次数（0表示无限重试）
    bool exponentialBackoff = true; // 是否使用指数退避
    int keepaliveInterval = 5000;  // 心跳间隔(ms)
    int keepaliveTimeout = 15000;   // 心跳超时(ms)

    ReconnectPolicy() = default;

    ReconnectPolicy(int initDelay, int maxDelay, int multiplier = 2)
        : initialDelay(initDelay), maxDelay(maxDelay),
        delayMultiplier(multiplier) {}
};

// 节点状态
struct NodeStatus {
    UA_StatusCode status;
    UA_DateTime sourceTimestamp;
    UA_DateTime serverTimestamp;
    DataQuality quality;
    bool isConnected;

    NodeStatus()
        : status(UA_STATUSCODE_BADNOTCONNECTED),
        sourceTimestamp(0),
        serverTimestamp(0),
        quality(QUALITY_BAD),
        isConnected(false) {}
};

// 会话统计
struct SessionStatistics {
    int totalConnections = 0;
    int failedConnections = 0;
    int successfulReads = 0;
    int failedReads = 0;
    int successfulWrites = 0;
    int failedWrites = 0;
    qint64 totalBytesRead = 0;
    qint64 totalBytesWritten = 0;
    QDateTime firstConnectTime;
    QDateTime lastConnectTime;
    QDateTime lastDisconnectTime;
    int currentReconnectAttempt = 0;

    SessionStatistics() = default;
};

// 操作请求
struct OperationRequest {
    OperationType type;//操作请求类型
    QString tagName;//变量名
    QVariant data;
    int requestId;//请求ID
    QDateTime timestamp;//请求生命周期管理和系统监控 请求时间

    OperationRequest()
        : type(OP_READ_SINGLE),
        requestId(0),
        timestamp(QDateTime::currentDateTime()) {}

    OperationRequest(OperationType t, int id)
        : type(t),
        requestId(id),
        timestamp(QDateTime::currentDateTime()) {}

    OperationRequest(OperationType t, const QString &name, int id)
        : type(t),
        tagName(name),
        requestId(id),
        timestamp(QDateTime::currentDateTime()) {}
};

// 操作结果
struct OperationResult {
    int requestId;
    bool success;
    QVariant data;
    QString error;
    QDateTime timestamp;

    OperationResult()
        : requestId(0),
        success(false),
        timestamp(QDateTime::currentDateTime()) {}

    OperationResult(int id, bool s, const QVariant &d = QVariant(),
                    const QString &e = QString())
        : requestId(id),
        success(s),
        data(d),
        error(e),
        timestamp(QDateTime::currentDateTime()) {}
};

// OPC UA 变量句柄
struct OPCUAVariableHandle {
    QString tagName; // 用户定义的标签名
    UA_NodeId nodeId;// OPC UA服务器的节点标识符
    UA_UInt32 monitoredItemId;// OPC UA订阅中的监控项ID（服务器分配）
    VariableDefinition* variableDef;// 变量定义信息（数据类型、范围等）
    NodeStatus lastStatus; // 最后一次读取的状态（质量戳、时间戳）
    QVariant lastValue;  // 最后一次读取的值（转换为Qt类型）
    bool isSubscribed; // 是否已建立数据订阅（用于变化通知）
    bool isBrowsed;  // 节点是否已浏览

    OPCUAVariableHandle()
        : monitoredItemId(0),
        variableDef(nullptr),
        lastValue(QVariant()),
        isSubscribed(false),
        isBrowsed(false) {
        UA_NodeId_init(&nodeId);
    }

    ~OPCUAVariableHandle() {
        UA_NodeId_clear(&nodeId);
    }

    // 禁用拷贝
    OPCUAVariableHandle(const OPCUAVariableHandle&) = delete;
    OPCUAVariableHandle& operator=(const OPCUAVariableHandle&) = delete;

    // 允许移动
    OPCUAVariableHandle(OPCUAVariableHandle&& other) noexcept {
        *this = std::move(other);
    }

    OPCUAVariableHandle& operator=(OPCUAVariableHandle&& other) noexcept {
        if (this != &other) {
            tagName = std::move(other.tagName);
            nodeId = other.nodeId;
            monitoredItemId = other.monitoredItemId;
            variableDef = other.variableDef;
            lastStatus = other.lastStatus;
            lastValue = other.lastValue;
            isSubscribed = other.isSubscribed;
            isBrowsed = other.isBrowsed;

            // 防止双重释放
            UA_NodeId_init(&other.nodeId);
            other.variableDef = nullptr;
        }
        return *this;
    }
};

// 订阅配置
struct SubscriptionConfig {
    double publishingInterval;  // 发布间隔(ms)// 发布间隔(ms)服务器向客户端发送不变化数据的间隔时间，变化数据，数据变化发送，不是每1s推送一次
    UA_UInt32 lifetimeCount;    // 生命周期计数
    UA_UInt32 maxKeepAliveCount;// 最大保活计数
    UA_Byte priority;           // 优先级

    SubscriptionConfig()                    //默认配置
        : publishingInterval(1000.0),    // 1秒发布间隔 发布间隔(ms)服务器向客户端发送不变化数据的间隔时间，变化数据，数据变化发送，不是每1s推送一次
        lifetimeCount(60),               // 生命周期计数 客户端允许服务器最多连续错过多少次心跳/数据更新后，就认为订阅已失效
        maxKeepAliveCount(10),          // 最大保活计数 服务器在没有数据变化时，最多可以"沉默"多少次，就必须强制发送一次心跳
        priority(0) {}                   // 优先级0 用于控制订阅在服务器资源分配中的相对重要性。0-255

    SubscriptionConfig(double interval, UA_UInt32 lifetime, UA_UInt32 keepalive)
        : publishingInterval(interval),
        lifetimeCount(lifetime),
        maxKeepAliveCount(keepalive),
        priority(0) {}
};

// 监控项配置
struct MonitoredItemConfig {
    double samplingInterval;    // 采样间隔(ms)服务器检查变量值变化的频率（单位：毫秒）
    UA_UInt32 queueSize;        // 队列大小服务器为每个监控项（每个变量）维护的一个数据队列的大小。保存最多10个数据变化事件（变化的值）
    bool discardOldest;         // 队列已满时，丢弃最旧的数据变化，
    UA_UInt32 clientHandle;     // 客户端句柄客户端自己生成一个唯一的ID给每个监控的变量分配一个唯一的"身份证号"，用于识别哪个变量发送了数据

    MonitoredItemConfig()
        : samplingInterval(1000.0), // 1秒采样间隔
        queueSize(10),              // 队列大小10
        discardOldest(true),        // 丢弃最旧数据
        clientHandle(0) {}

    MonitoredItemConfig(double interval, UA_UInt32 queue)
        : samplingInterval(interval),
        queueSize(queue),
        discardOldest(true),
        clientHandle(0) {}
};

}

// ==================== OPCUAConnectionManager 类 ====================
namespace Industrial {
class OPCUAConnectionManager : public QObject
{
    Q_OBJECT

public:
    explicit OPCUAConnectionManager(QObject *parent = nullptr);
    ~OPCUAConnectionManager();

    bool connectToserver(const QString &endpointUrl,
                 const QString &username = "",
                 const QString &password = "");
    void disconnect();
    bool reconnect();

    ConnectionState state() const { return m_state.load(); }
    bool isConnected() const ;
    UA_Client* client()const ;

    void setReconnectPolicy(const ReconnectPolicy &policy);
    ReconnectPolicy reconnectPolicy() const { return m_policy; }

    SessionStatistics statistics() const;
    void resetStatistics();


public:
    // 获取连接信息
    QString endpointUrl() const ;
    QString username() const ;

    //检查客户端是否有效的方法
    bool isClientValid() const;

    // 辅助方法
    QString getLastError() const;// 获取最后一个错误
    QString connectionStateName() const;
    void logConnectionAttempt(const QString &details= "");//记录连接尝试的日志

private:
    // 心跳检测
    bool sendKeepalive();
    qint64 lastKeepaliveTime() const { return m_lastKeepaliveTime.load(); }
    bool isKeepaliveExpired() const;//检查心跳是否已超

    bool performConnection();
    void updateState(ConnectionState newState);
    void scheduleReconnect();
    int calculateReconnectDelay();
    void recordConnectionSuccess();
    void recordConnectionFailure();



    // 私有方法：记录错误
    //void logConnectionAttempt(const QString &details= "");//记录连接尝试的日志
    void recordError(const QString &error);
    //void recordError(UA_StatusCode status);
    void clearError();

signals:
    void stateChanged(ConnectionState newState, ConnectionState oldState);
    void connected();
    void disconnected();
    void connectionLost();
    void reconnecting(int attempt, int delay);
    void connectionError(const QString &error);
    void keepaliveReceived();
    void keepaliveFailed();
    void logAttemptChanged(const QString details);

private slots:
    void onKeepaliveTimer();
    void onReconnectTimer();

private:
    bool getServerTime(UA_DateTime  &serverTime);
    UA_Client *m_client=nullptr;// OPC UA 客户端
    std::atomic<ConnectionState> m_state;// 连接状态

    QString m_endpointUrl;// 服务器端点URL
    QString m_username;// 用户名
    QString m_password;// 密码

    ReconnectPolicy m_policy;// 重连策略
    SessionStatistics m_stats;// 统计信息

    QTimer *m_keepaliveTimer; // 心跳定时器
    QTimer *m_reconnectTimer; // 重连定时器
    std::atomic<qint64> m_lastKeepaliveTime;// 最后心跳时间
    std::atomic<qint64> m_lastActivityTime;// 最后活动时间
    std::atomic<int> m_reconnectAttempt; // 重连尝试次数

    mutable QMutex m_mutex; // 互斥锁

    // 添加错误记录相关成员
    QString m_lastError;

    mutable QMutex m_errorMutex;

    mutable QReadWriteLock m_rwLock;  // 替换 QMutex


};
}

// ==================== OPCUAVariableManager 类 ====================
namespace Industrial {
class OPCUAVariableManager : public QObject
{
    Q_OBJECT

public:
    explicit OPCUAVariableManager(QObject *parent = nullptr);
    ~OPCUAVariableManager();

    // ==================== 连接管理 ====================
    bool connect(const QString &endpointUrl,
                 const QString &username = "",
                 const QString &password = "");
    void disconnect();
    bool reconnect();

    ConnectionState connectionState() const;
    bool isConnected() const;

    // ==================== 配置管理 ====================
    void setReconnectPolicy(const ReconnectPolicy &policy);
    ReconnectPolicy reconnectPolicy() const;

    void setRequestTimeout(int timeoutMs);//设置异步操作的超时时间
    void setRetryCount(int count);//设置失败操作的重试次数
    void setMaxThreadCount(int count);//动态调整线程池大小

    // 订阅配置
    void setSubscriptionConfig(const SubscriptionConfig &config);//设定阅订模式
    SubscriptionConfig subscriptionConfig() const; //读取监控配置

    void setMonitoredItemConfig(const MonitoredItemConfig &config);//设定阅订模式
    MonitoredItemConfig monitoredItemConfig() const;                //读取阅订模式

    // ==================== 变量管理 ====================
    bool registerVariable(VariableDefinition *variable);
    bool registerVariables(const QList<VariableDefinition*> &variables);
    bool unregisterVariable(const QString &tagName);
    void clearVariables();

    bool browseVariableNode(const QString &tagName);
    bool browseAllVariables();

    // ==================== 异步操作 ====================
    int readVariableAsync(const QString &tagName);
    int readAllVariablesAsync();
    int writeVariableAsync(const QString &tagName, const QVariant &value);

    // ==================== 同步操作 ====================
    QVariant readVariableSync(const QString &tagName, bool *ok = nullptr,
                            int timeoutMs = 5000);
    bool writeVariableSync(const QString &tagName, const QVariant &value,
                           int timeoutMs = 5000);
    QVariantMap readAllVariablesSync();

    // ==================== 订阅管理 ====================
    bool startSubscription(SubscriptionMode mode = SUBSCRIPTION_MONITORED);
    void stopSubscription();
    bool isSubscribed() const;

    void setPollingInterval(int intervalMs);
    int pollingInterval() const;

    // ==================== 查询方法 ====================
    VariableDefinition* getVariable(const QString &tagName) const;
    QList<VariableDefinition*> getAllVariables() const;
    QList<QString> getRegisteredTagNames() const;

    OPCUAVariableHandle* getVariableHandle(const QString &tagName) const;
    NodeStatus getVariableStatus(const QString &tagName) const;
    QVariant getLastValue(const QString &tagName) const;

    // ==================== 统计信息 ====================
    SessionStatistics connectionStatistics() const;
    int pendingRequests() const;
    int activeThreads() const;
    double averageResponseTime() const;
    void resetStatistics();

    // ==================== 服务器信息 ====================
    QString serverName() const;
    QString serverEndPointUrl() const;
    QString serverVersion() const;
    UA_DateTime serverTime() const;

    // ==================== 诊断功能 ====================
    bool testConnection(int timeoutMs = 3000);
    QString lastError() const;
    QList<QString> recentErrors(int count = 10) const;


    // ==================== 批量操作 ====================
    bool batchRead(const QList<QString> &tagNames, QVariantMap &results,
                   int timeoutMs = 10000);
    bool batchWrite(const QVariantMap &values, int timeoutMs = 10000);

signals:
    // ==================== 连接状态信号 ====================
    void connectionStateChanged(ConnectionState state);
    void connected();
    void disconnected();
    void connectionLost();
    void reconnecting(int attempt, int delay);
    void connectionError(const QString &error);
    void connectionRestored();
    void cleanedUp();


    // ==================== 数据操作信号 ====================
    void readCompleted(int requestId, const QString &tagName,
                       const QVariant &value, bool success, const QString &error);
    void writeCompleted(int requestId, const QString &tagName,
                        bool success, const QString &error);
    void batchReadCompleted(int requestId,
                            const QVariantMap &values,
                            bool success, const QString &error);
    void batchWriteCompleted(int requestId, bool success, const QString &error);

    // ==================== 实时数据信号 ====================
    void variableValueChanged(const QString &tagName,  const QVariant &value,
                              const QDateTime &timestamp, DataQuality quality);
    void variableStatusChanged(const QString &tagName, const NodeStatus &status);
    void batchValuesUpdated(const QVariantMap &values);

    // ==================== 报警信号 ====================
    void alarmTriggered(const QString &tagName, AlarmLevel level, double value);
    void alarmCleared(const QString &tagName);

    // ==================== 心跳信号 ====================
    void heartbeatReceived();
    void heartbeatTimeout();

    // ==================== 节点浏览信号 ====================
    void variableNodeBrowsed(const QString &tagName, bool success,
                             const QString &error);
    void allVariablesBrowsed(int successCount, int failureCount);



public slots:
    // ==================== 刷新操作 ====================
    void refreshAllVariablesAsync();
    void refreshVariableAsync(const QString &tagName);
    void refreshAllVariablesSync();
    void refreshVariableSync(const QString &tagName);

    // ==================== 连接操作 ====================
    void forceReconnect();//从新连接服务器
    void testConnectionAsync();

    // ==================== 订阅操作 ====================
    void restartSubscription();

    // ==================== 工具操作 ====================
    void clearErrorLog();
    void dumpStatusToLog();

    void onSubscriptionDeleted(UA_UInt32 subId);//阅被删除

    // ==================== 阅订回调 ====================
    void startProcessing();
    void stopProcessing();



private slots:
    // ==================== 定时器槽 ====================
    void onPollingTimer();//询定时器的回调函数，用于实现轮询模式的订阅

    // ==================== 连接管理槽 ====================
    void onConnectionStateChanged(ConnectionState newState, ConnectionState oldState);

    // ==================== 任务完成槽 ====================
    void onTaskCompleted(int requestId, bool success,
                         const QVariant &result, const QString &error);

    // ==================== 内部槽 ====================
    void onInternalReconnect();
    //void onKeepaliveReceived();

    void updateVariableFromCallback(OPCUAVariableHandle* handle,
                                    UA_DataValue* value);



private:

    // ==================== 连接管理 ====================
    std::unique_ptr<OPCUAConnectionManager> m_connectionManager;

    // ==================== 工作线程 ====================
    QThreadPool *m_threadPool;
    int m_maxThreadCount;

    // ==================== 变量管理 ====================
    QHash<QString, std::shared_ptr<OPCUAVariableHandle>> m_variables;
    mutable QReadWriteLock m_variablesLock;

    // ==================== 订阅管理 ====================
    SubscriptionMode m_subscriptionMode;
    UA_UInt32 m_subscriptionId;
    SubscriptionConfig m_subscriptionConfig;
    MonitoredItemConfig m_monitoredItemConfig;
    QTimer *m_pollingTimer;
    int m_pollingInterval;

    // ==================== 服务器时间 ====================
    //UA_DateTime getServerTime() const;

    // ==================== 请求管理 ====================
    std::atomic<int> m_requestIdCounter{1};
    QMap<int, OperationRequest> m_pendingRequests;
   // QMutex m_requestsMutex;
    mutable QMutex m_requestsMutex;

    // ==================== 错误记录 ====================
    QList<QPair<QDateTime, QString>> m_errorLog;
    mutable QMutex m_errorMutex;

    // ==================== 同步等待 ====================
    struct SyncWaitData {
        bool completed = false;
        QVariant result;
        QString error;
        QWaitCondition condition;
    };
    QMap<int, SyncWaitData*> m_syncWaitMap;
    mutable QMutex m_syncWaitMutex;

    // 添加这个互斥锁声明
    mutable QMutex m_mutex;  // 通用互斥锁，用于连接等操作

     QTimer* m_processTimer;

    // ==================== 统计信息 ====================
    std::atomic<int> m_successfulReads{0};
    std::atomic<int> m_failedReads{0};
    std::atomic<int> m_successfulWrites{0};
    std::atomic<int> m_failedWrites{0};
    QList<int> m_responseTimes;
    mutable QMutex m_statsMutex;

    // ==================== 内部状态 ====================
    bool m_isInitialized;

    // ==================== 私有方法 ====================

    // 初始化/清理
    bool initialize();
    void cleanup();

    // 请求ID管理
    int generateRequestId();
    void addPendingRequest(const OperationRequest &request);
    void removePendingRequest(int requestId);

    // 同步等待
    bool waitForCompletion(int requestId, int timeoutMs,
                           QVariant &result, QString &error);

    // 错误处理
    void recordError(const QString &error);
    void recordSuccess(const QString &operation);


    // 连接验证
    bool validateConnection() const;
    bool attemptGracefulReconnect();

    // OPC UA 辅助方法
    bool parseNodeId(const QString &address, UA_NodeId &nodeId);
    QString nodeIdToString(const UA_NodeId &nodeId) const;
    DataQuality statusCodeToQuality(UA_StatusCode statusCode) const;
    //类型转换

    // 变量句柄管理
    OPCUAVariableHandle* getOrCreateHandle(const QString &tagName);
    const OPCUAVariableHandle* getHandle(const QString &tagName) const;

    int m_requestTimeout = 5000;      // 默认5秒超时
    int m_retryCount = 2;             // 默认重试2次

    // 订阅管理
    bool createSubscription();
    bool deleteSubscription();
    bool createMonitoredItem(OPCUAVariableHandle *handle);
    bool deleteMonitoredItem(OPCUAVariableHandle *handle);

    // 数据转换
    QString connectionStateToString(ConnectionState state) const;

    //辅助转换函数
    QVariant uaVariantToQVariant(const UA_Variant &variant) const;
    UA_Variant qVariantToUAVariant(const QVariant &qtVariant,
                                   const UA_DataType* expectedType = nullptr) const;  
    // 类型检测和验证
    //bool validateVariantType(const QVariant &value, VariableDefinition::DataType expectedType) const;
    //VariableDefinition::DataType detectVariantType(const QVariant &value) const;


    // 监控项回调（静态）
    static void dataChangeNotificationCallback(
        UA_Client *client, UA_UInt32 subId, void *subContext,
        UA_UInt32 monId, void *monContext, UA_DataValue *value);

    static void deleteSubscriptionCallback(
        UA_Client *client, UA_UInt32 subId, void *subContext);


    // 内部任务
    void executeBrowseTask(const QString &tagName);

    // 声明友元类，让 OPCUATask 可以访问私有方法
    friend class OPCUATask;
};
}
// ==================== OPCUATask 类 ====================
namespace Industrial {
class OPCUATask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    OPCUATask(OperationType type, const QString &tagName,
              const QVariant &data, int requestId,
              class OPCUAVariableManager *manager);
    ~OPCUATask();

    void run() override;

signals:
    void taskCompleted(int requestId, bool success,
                       const QVariant &result, const QString &error);

private:
    OperationType m_type;
    QString m_tagName;
    QVariant m_data;
    int m_requestId;
    class OPCUAVariableManager *m_manager;

    // 执行具体操作
    QVariant executeReadSingle();
    QVariant executeReadBatch();
    QVariant executeWriteSingle();
    QVariant executeWriteBatch();
    QVariant executeBrowse();

    // 辅助方法
    UA_Client* createTemporaryClient();
    void cleanupTemporaryClient();
    bool connectTemporaryClient(UA_Client *client);
    void disconnectTemporaryClient(UA_Client *client);
    //变量跟新
    // 修改函数定义
    inline void updateVariableDirectly(OPCUAVariableHandle* handle,
                                       const QVariant& value,
                                       UA_StatusCode status,
                                       const OPCUAVariableManager* manager) {
        QDateTime timestamp = QDateTime::currentDateTime();
        DataQuality quality = manager->statusCodeToQuality(status);

        // 使用完整的 setValue（如果支持）
        handle->variableDef->setValue(value, timestamp, quality);
        handle->lastValue = value;
        handle->lastStatus.quality = quality;
        handle->lastStatus.status = status;  // 可选：存储原始状态码
    }
};
}

namespace Industrial {
class DataParse :public QObject
{
    Q_OBJECT
public:
    explicit DataParse(QObject *parent=nullptr);
};

}
#endif // OPCUACLIENTMANAGER_H
