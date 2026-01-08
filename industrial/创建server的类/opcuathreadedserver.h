// OpcUaThreadedServer.h
#ifndef OPCUATHREADEDSERVER_H
#define OPCUATHREADEDSERVER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QAtomicInteger>
#include <QTimer>
#include <QHash>
#include <QVariant>
#include <QReadWriteLock>
#include "variablesystem.h"

// 如果是单文件版本，定义这个宏
#define UA_ENABLE_AMALGAMATION
extern "C" {
#include "open62541.h"
}




namespace Industrial {


class OpcUaServerWorker : public QObject {
    Q_OBJECT

public:
    explicit OpcUaServerWorker(QObject* parent = nullptr);
    ~OpcUaServerWorker();

public slots:
    void initializeServer(const QString& appUri, quint16 port);
    void startServer();
    void stopServer();
    void registerVariable(const QString& tagName, VariableDefinition* varDef);
    void updateVariableValue(const QString& tagName, double value,
                             UA_DateTime timestamp = 0);
    //void batchUpdateValues(const QHash<QString, double>& values);
    // 修改参数类型为 QVariant
    void batchUpdateValues(const QVariant& valuesVariant);

signals:
    void serverInitialized(bool success);
    void serverStarted();
    void serverStopped();
    void variableWritten(const QString& tagName, double value);
    void errorOccurred(const QString& error);


private:
    UA_Server* m_server;
    bool m_running;
    QMutex m_serverMutex;

    // 节点映射
    QHash<QString, UA_NodeId*> m_variableNodes;
    UA_UInt16 m_namespaceIndex;

    // 待处理的操作队列
    struct PendingOperation {
        enum Type { RegisterVariable, UpdateValue, BatchUpdate } type;
        QString tagName;
        QVariant data;
        UA_DateTime timestamp;
    };

    QQueue<PendingOperation> m_pendingOps;
    QMutex m_queueMutex;
    QWaitCondition m_queueCondition;

    // 工作线程函数
    void processOperations();
    void serverLoop();

    // 服务器回调的包装器
    static void onWriteCallbackWrapper(UA_Server* server,
                                       const UA_NodeId* sessionId,
                                       void* sessionContext,
                                       const UA_NodeId* nodeId,
                                       void* nodeContext,
                                       const UA_NumericRange* range,
                                       const UA_DataValue* data);

    void onWriteCallback(const UA_NodeId* nodeId, const UA_DataValue* data);

    // 辅助方法
    UA_NodeId* createVariableNode(VariableDefinition* varDef);
    void updateNodeValue(UA_NodeId* nodeId, double value, UA_DateTime timestamp);

    // 工具函数
    static bool UA_NodeId_isNull(const UA_NodeId* nodeId);
    static void UA_NodeId_clear(UA_NodeId* nodeId);
   // static UA_DateTime DateTime_now();
public:
    static UA_DateTime getDateTime_now();
    QString  getServerStatus() ;//获取服务器的当前状态（运行中、已停止、未初始化等） 调试、监控界面显示、日志记录
    QStringList getRegisteredVariables() ;//获取所有已注册的变量名列表 调试、变量管理界面、导出配置
    int getVariableCount()  ;//获取已注册变量的数量 统计、监控、资源管理
    bool isVariableRegistered(const QString& tagName) ;//检查特定变量是否已注册 防止重复注册、验证变量存在性 避免重复注册同一个变量
};

class OpcUaThreadedServer : public QObject {
    Q_OBJECT

public:
    explicit OpcUaThreadedServer(QObject* parent = nullptr);
    ~OpcUaThreadedServer();

    // 配置接口（线程安全）
    bool configure(const QString& appUri, const QString& appName, quint16 port = 4840);

    // 变量管理接口
    bool registerVariable(VariableDefinition* varDef);
    bool batchRegisterVariables(const QList<VariableDefinition*>& varDefs);

    // 数据更新接口（异步）
    void updateValue(const QString& tagName, double value);
    void batchUpdateValues(const QHash<QString, double>& values);

    // 服务器控制
    bool start();
    void stop();
    bool isRunning() const;

    // 统计信息
    struct ServerStats {
        quint64 writeCount;
        quint64 readCount;
        quint64 errorCount;
        double avgUpdateLatency;
    };

    ServerStats getStats() const;//获取服务器运行统计信息,未实现
    void resetStats();//重置统计


signals:
    void variableValueChanged(const QString& tagName, double value);
    void clientConnected(const QString& endpoint);
    void clientDisconnected(const QString& endpoint);
    void serverStatusChanged(bool running);

private slots:
    void onWorkerVariableWritten(const QString& tagName, double value);

private:
    OpcUaServerWorker* m_worker;
    QThread* m_workerThread;

    // 线程安全的变量缓存（减少锁竞争）
    class ThreadSafeVariableCache {
    public:
        struct CachedVariable {
            double value;
            UA_DateTime timestamp;
            std::atomic<bool> dirty;
            QReadWriteLock lock;
        };

        void update(const QString& tagName, double value, UA_DateTime timestamp);
        bool get(const QString& tagName, double& value, UA_DateTime& timestamp);
        QHash<QString, double> getDirtyValues();
        void clear();

    private:
        QHash<QString, CachedVariable*> m_cache;
        QReadWriteLock m_cacheLock;
    };

    ThreadSafeVariableCache m_valueCache;

    // 批量更新优化
    QTimer* m_batchTimer;
    QHash<QString, double> m_pendingUpdates;
    QMutex m_updateMutex;
    bool m_running;

    // 统计信息
    mutable QMutex m_statsMutex;
    ServerStats m_stats;

private slots:
    void flushPendingUpdates();

private:
    void updateStats(quint64 writes, quint64 reads, quint64 errors);

    QVariant hashToVariant(const QHash<QString, double>& hash);
    QHash<QString, double> variantToHash(const QVariant& variant);
};

} // namespace Industrial

#endif // OPCUATHREADEDSERVER_H
