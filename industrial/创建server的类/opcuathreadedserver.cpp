// OpcUaThreadedServer.cpp

#include"opcuathreadedserver.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>
#include <chrono>
#include <QFuture>
#include <QMutexLocker>
#include <QtConcurrent/QtConcurrent>
//-------------------open62541不报错了---------------------------------------


// 包含 open62541 单文件版本
// 注意：确保 open62541.h 单文件版本在 include 路径中
//extern "C" {
//#include "open62541.h"
//}

namespace Industrial {

// ==================== OpcUaServerWorker 实现 ====================

OpcUaServerWorker::OpcUaServerWorker(QObject* parent)
    : QObject(parent)
    , m_server(nullptr)
    , m_running(false)
    , m_namespaceIndex(0) {

    // 启动操作处理线程 - 简化版本
    QThread* processThread = new QThread;
    this->moveToThread(processThread);

    // 连接线程完成的信号
    connect(processThread, &QThread::finished, processThread, &QObject::deleteLater);
    connect(processThread, &QThread::finished, this, &QObject::deleteLater);

    // 启动线程后，异步调用 processOperations
    connect(processThread, &QThread::started, this, [this]() {
        // 在新线程中启动处理循环
        processOperations();
    });

    processThread->start();
}

OpcUaServerWorker::~OpcUaServerWorker() {
    stopServer();

    if (m_server) {
        UA_Server_delete(m_server);
        m_server = nullptr;
    }

    // 清理节点ID
    for (UA_NodeId* nodeId : m_variableNodes) {
        if (nodeId) {
            UA_NodeId_clear(nodeId);
            delete nodeId;
        }
    }
    m_variableNodes.clear();
}



void OpcUaServerWorker::initializeServer(const QString& appUri, quint16 port) {
    QMutexLocker locker(&m_serverMutex);

    qDebug() << "[DEBUG] initializeServer called with port:" << port;

    // 如果已有服务器，先清理
    if (m_server) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        m_running = false;
    }

    // 1. 创建服务器（和你的客户端一样使用new函数）
    m_server = UA_Server_new();
    if (!m_server) {
        qDebug() << "[ERROR] UA_Server_new() failed";
        emit errorOccurred("Failed to create OPC UA server");
        emit serverInitialized(false);
        return;
    }
    qDebug() << "[DEBUG] UA_Server_new() succeeded";

    // 2. 获取配置（注意：是UA_Server_getConfig，不是UA_Client_getConfig）
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (!config) {
        qDebug() << "[ERROR] UA_Server_getConfig() failed";
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred("Failed to get server config");
        emit serverInitialized(false);
        return;
    }
    qDebug() << "[DEBUG] UA_Server_getConfig() succeeded";

    // 3. 设置默认配置（和你的客户端类似）
    UA_StatusCode status = UA_ServerConfig_setDefault(config);
    if (status != UA_STATUSCODE_GOOD) {
        qDebug() << "[ERROR] UA_ServerConfig_setDefault() failed:" << UA_StatusCode_name(status);
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred(QString("Failed to set default config: %1")
                               .arg(UA_StatusCode_name(status)));
        emit serverInitialized(false);
        return;
    }
    qDebug() << "[DEBUG] UA_ServerConfig_setDefault() succeeded";

    // 4. 设置应用程序URI
    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri =
        UA_STRING_ALLOC(appUri.toUtf8().constData());

    // 5. 设置端口（49320）
    if (port > 0 && config->endpointsSize > 0) {
        UA_String_clear(&config->endpoints[0].endpointUrl);

        QString endpointUrl = QString("opc.tcp://127.0.0.1:%1").arg(port);
        config->endpoints[0].endpointUrl =
            UA_STRING_ALLOC(endpointUrl.toUtf8().constData());

        qDebug() << "[DEBUG] Endpoint set to:" << endpointUrl;
    }

    // 6. 添加命名空间
    const char* namespaceUri = "urn:industrial:opcua:server";
    m_namespaceIndex = UA_Server_addNamespace(m_server, namespaceUri);
    qDebug() << "[DEBUG] Namespace index:" << m_namespaceIndex;

    // 7. 设置运行标志
    m_running = false;

    qDebug() << "[DEBUG] Server initialization complete";
    emit serverInitialized(true);
}

void OpcUaServerWorker::startServer() {
    QMutexLocker locker(&m_serverMutex);

    qDebug() << "[DEBUG] startServer called";

    if (!m_server) {
        qDebug() << "[ERROR] Server not initialized";
        emit errorOccurred("Server not initialized");
        return;
    }

    if (m_running) {
        qDebug() << "[WARN] Server already running";
        emit errorOccurred("Server is already running");
        return;
    }

    // 关键步骤：启动服务器（不是UA_Client_connect！）
    qDebug() << "[DEBUG] Calling UA_Server_run_startup()...";
    UA_StatusCode status = UA_Server_run_startup(m_server);

    if (status != UA_STATUSCODE_GOOD) {
        qDebug() << "[ERROR] UA_Server_run_startup() failed:" << UA_StatusCode_name(status);

        // 详细的错误处理
        QString errorMsg;
        switch (status) {
        case UA_STATUSCODE_BADINTERNALERROR:
            errorMsg = "Port may be in use. Try: netstat -ano | findstr :49320";
            break;
        case UA_STATUSCODE_BADOUTOFMEMORY:
            errorMsg = "Out of memory";
            break;
        case UA_STATUSCODE_BADRESOURCEUNAVAILABLE:
            errorMsg = "Resource unavailable";
            break;
        default:
            errorMsg = QString("Error code: %1").arg(UA_StatusCode_name(status));
        }

        qDebug() << "[ERROR]" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    qDebug() << "[DEBUG] UA_Server_run_startup() succeeded";

    // 设置运行标志
    m_running = true;

    // 启动服务器运行循环线程
    QThread* serverThread = QThread::create([this]() {
        qDebug() << "[DEBUG] Server loop thread started";

        while (m_running) {
            {
                QMutexLocker locker(&m_serverMutex);
                if (m_server && m_running) {
                    UA_Server_run_iterate(m_server, false);
                }
            }

            // 控制循环频率（100ms一次迭代）
            QThread::msleep(100);
        }

        qDebug() << "[DEBUG] Server loop thread exiting";
    });

    // 连接线程完成信号
    connect(serverThread, &QThread::finished, serverThread, &QObject::deleteLater);

    // 启动线程
    serverThread->start();

    // 等待一下确保线程启动
    QThread::msleep(50);

    // 获取并显示服务器信息
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (config && config->endpointsSize > 0) {
        QString endpoint = QString::fromUtf8(
            (char*)config->endpoints[0].endpointUrl.data,
            config->endpoints[0].endpointUrl.length);
        qDebug() << "[SUCCESS] Server started at:" << endpoint;
    } else {
        qDebug() << "[SUCCESS] Server started (unable to get endpoint info)";
    }

    qDebug() << "[DEBUG] Emitting serverStarted signal";
    emit serverStarted();
}

void OpcUaServerWorker::stopServer() {
    QMutexLocker locker(&m_serverMutex);

    qDebug() << "[DEBUG] stopServer called";

    if (!m_server || !m_running) {
        qDebug() << "[DEBUG] Server not running or not initialized";
        return;
    }

    qDebug() << "[DEBUG] Setting m_running = false";
    m_running = false;

    // 等待一下让循环线程检测到停止标志
    QThread::msleep(200);

    qDebug() << "[DEBUG] Calling UA_Server_run_shutdown()";
    UA_Server_run_shutdown(m_server);

    qDebug() << "[DEBUG] Calling UA_Server_delete()";
    UA_Server_delete(m_server);
    m_server = nullptr;

    // 清理变量节点
    for (UA_NodeId* nodeId : m_variableNodes) {
        if (nodeId) {
            UA_NodeId_clear(nodeId);
            delete nodeId;
        }
    }
    m_variableNodes.clear();

    qDebug() << "[DEBUG] Server stopped, emitting serverStopped signal";
    emit serverStopped();
}
/*
void OpcUaServerWorker::initializeServer(const QString& appUri, quint16 port) {
    QMutexLocker locker(&m_serverMutex);

    if (m_server) {
        UA_Server_delete(m_server);
        m_server = nullptr;
    }

    qDebug() << "Initializing OPC UA server (open62541 1.3.6)...";

    // 1. 创建服务器实例
    m_server = UA_Server_new();
    if (!m_server) {
        emit errorOccurred("Failed to allocate memory for OPC UA server");
        emit serverInitialized(false);
        return;
    }

    // 2. 获取配置
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (!config) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred("Failed to get server config");
        emit serverInitialized(false);
        return;
    }

    // 3. 设置默认配置
    UA_StatusCode status = UA_ServerConfig_setDefault(config);
    if (status != UA_STATUSCODE_GOOD) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred(QString("Failed to set default config: %1")
                               .arg(UA_StatusCode_name(status)));
        emit serverInitialized(false);
        return;
    }

    // 4. 设置应用程序URI
    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri =
        UA_STRING_ALLOC(appUri.toUtf8().constData());

    // 5. 设置端口（如果需要）
    if (port > 0 && port != 4840) {
        if (config->endpointsSize > 0) {
            UA_String_clear(&config->endpoints[0].endpointUrl);

            QString endpointUrl = QString("opc.tcp://localhost:%1").arg(port);
            config->endpoints[0].endpointUrl =
                UA_STRING_ALLOC(endpointUrl.toUtf8().constData());
        }
    }

    // 6. 添加自定义命名空间
    const char* namespaceUri = "urn:industrial:opcua:server";
    m_namespaceIndex = UA_Server_addNamespace(m_server, namespaceUri);

    // 检查命名空间是否添加成功
    if (m_namespaceIndex == 0) {
        qWarning() << "Failed to add custom namespace, using default";
        m_namespaceIndex = 1;
    }

    // 7. 设置运行标志
    m_running = false;

    qDebug() << "OPC UA Server initialized successfully";
    qDebug() << "  - Application URI:" << appUri;
    qDebug() << "  - Port:" << (port > 0 ? port : 4840);
    qDebug() << "  - Namespace Index:" << m_namespaceIndex;

    emit serverInitialized(true);
}

void OpcUaServerWorker::startServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server) {
        emit errorOccurred("Server not initialized");
        return;
    }

    if (m_running) {
        emit errorOccurred("Server is already running");
        return;
    }

    // 启动服务器
    UA_StatusCode status = UA_Server_run_startup(m_server);
    if (status != UA_STATUSCODE_GOOD) {
        emit errorOccurred(QString("Server startup failed: %1")
                               .arg(UA_StatusCode_name(status)));
        return;
    }

    m_running = true;

    // 启动服务器循环线程
    QThread* serverThread = QThread::create([this]() {
        qDebug() << "OPC UA server loop started in thread" << QThread::currentThreadId();

        while (m_running) {
            UA_Server_run_iterate(m_server, false);
            QThread::msleep(100);  // 控制循环频率
        }

        qDebug() << "Server loop stopped";
    });

    connect(serverThread, &QThread::finished, serverThread, &QObject::deleteLater);
    serverThread->start();

    // 等待一下确保服务器启动
    QThread::msleep(100);

    // 检查配置
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (config && config->endpointsSize > 0) {
        QString endpoint = QString::fromUtf8(
            (char*)config->endpoints[0].endpointUrl.data,
            config->endpoints[0].endpointUrl.length);
        qDebug() << "Server endpoint:" << endpoint;
    }

    emit serverStarted();
    qDebug() << "OPC UA server started successfully";
}
void OpcUaServerWorker::stopServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server || !m_running) {
        return;
    }

    qDebug() << "Stopping OPC UA server...";

    m_running = false;

    // 等待一下确保循环线程检测到停止标志
    QThread::msleep(200);

    // 停止服务器
    UA_Server_run_shutdown(m_server);

    // 清理资源
    UA_Server_delete(m_server);
    m_server = nullptr;

    // 清理变量节点
    for (UA_NodeId* nodeId : m_variableNodes) {
        if (nodeId) {
            UA_NodeId_clear(nodeId);
            delete nodeId;
        }
    }
    m_variableNodes.clear();

    qDebug() << "OPC UA server stopped";
    emit serverStopped();
}
*/


//----------------------------------针对1.3.6版本的修改-----------------------------------------



/*
void OpcUaServerWorker::initializeServer(const QString& appUri, quint16 port) {
    QMutexLocker locker(&m_serverMutex);

    if (m_server) {
        emit errorOccurred("Server already initialized");
        emit serverInitialized(false);
        return;
    }

    // 1. 创建服务器实例
    m_server = UA_Server_new();
    if (!m_server) {
        emit errorOccurred("Failed to allocate memory for OPC UA server");
        emit serverInitialized(false);
        return;
    }

    // 2. 获取配置并设置为默认配置
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    UA_StatusCode status = UA_ServerConfig_setDefault(config);
    if (status != UA_STATUSCODE_GOOD) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred(QString("Failed to set default config: %1")
                               .arg(UA_StatusCode_name(status)));
        emit serverInitialized(false);
        return;
    }

    // 3. 设置应用程序 URI
    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri =
        UA_STRING_ALLOC(appUri.toUtf8().constData());

    // 4. 设置端口（如果需要）
    if (port > 0 && port != 4840) {
        if (config->endpointsSize > 0) {
            UA_String_clear(&config->endpoints[0].endpointUrl);

            QString endpointUrl = QString("opc.tcp://localhost:%1").arg(port);
            config->endpoints[0].endpointUrl =
                UA_STRING_ALLOC(endpointUrl.toUtf8().constData());
        }
    }

    // 5. 添加自定义命名空间 - 注意：只有两个参数！
    const char* namespaceUri = "http://your.namespace.org";
    m_namespaceIndex = UA_Server_addNamespace(m_server, namespaceUri);

    // UA_Server_addNamespace 在 1.3.6 中返回 UA_UInt16（命名空间索引）
    // 而不是 UA_StatusCode

    // 检查返回值，如果失败，m_namespaceIndex 会是 0
    if (m_namespaceIndex == 0) {
        qWarning() << "Failed to add custom namespace, using default index";
        m_namespaceIndex = 1; // 通常1是自定义命名空间起始索引
    }

    // 6. 设置运行标志
    m_running = false;

    // 7. 记录日志
    qDebug() << "OPC UA Server initialized successfully";
    qDebug() << "  - Application URI:" << appUri;
    qDebug() << "  - Port:" << (port > 0 ? port : 4840);
    qDebug() << "  - Namespace Index:" << m_namespaceIndex;

    // 8. 发出成功信号
    emit serverInitialized(true);
}

void OpcUaServerWorker::startServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server) {
        emit errorOccurred("Server not initialized");
        return;
    }

    if (m_running) {
        emit errorOccurred("Server is already running");
        return;
    }

    // 检查服务器是否已经启动
    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (!config) {
        emit errorOccurred("Server configuration not available");
        return;
    }

    // 开始运行服务器
    // 注意：在 Open62541 1.3.6 中，UA_Server_run 是阻塞调用
    // 我们需要在单独的线程中运行它

    // 设置运行标志
    m_running = true;

    // 在单独的线程中启动服务器运行循环
    QFuture<void> future = QtConcurrent::run([this]() {
        qDebug() << "OPC UA server starting main loop...";

        // 这是阻塞调用，直到服务器被停止
        UA_StatusCode status = UA_Server_run(m_server, &m_running);

        if (status != UA_STATUSCODE_GOOD && m_running) {
            // 只有在我们期望它运行时才报告错误
            QString errorMsg = QString("Server stopped with error: %1")
                                   .arg(UA_StatusCode_name(status));
            qCritical() << errorMsg;

            // 在主线程中发出错误信号
            QMetaObject::invokeMethod(this, [this, errorMsg]() {
                emit errorOccurred(errorMsg);
                emit serverStopped();
            }, Qt::QueuedConnection);
        } else {
            qDebug() << "OPC UA server stopped normally";
        }
    });

    // 等待一小段时间确保服务器启动
    QThread::msleep(100);

    // 检查服务器是否成功启动
    if (m_running) {
        emit serverStarted();
        qDebug() << "OPC UA server started successfully";
    } else {
        emit errorOccurred("Failed to start server");
    }
}

void OpcUaServerWorker::stopServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server || !m_running) {
        return;
    }

    qDebug() << "Stopping OPC UA server...";

    // 设置停止标志
    m_running = false;

    // 停止服务器运行循环
    UA_Server_run_shutdown(m_server);

    // 等待一小段时间让服务器完全停止
    QThread::msleep(100);

    // 清理资源
    if (m_server) {
        UA_Server_delete(m_server);
        m_server = nullptr;
    }

    // 清理变量节点映射
    for (UA_NodeId* nodeId : m_variableNodes) {
        if (nodeId) {
            UA_NodeId_clear(nodeId);
            delete nodeId;
        }
    }
    m_variableNodes.clear();

    qDebug() << "OPC UA server stopped";
    emit serverStopped();
}


//-----------------------------以前的初始化程序-----------------------------------------
void OpcUaServerWorker::initializeServer(const QString& appUri, quint16 port) {
    QMutexLocker locker(&m_serverMutex);

    if (m_server) {
        UA_Server_delete(m_server);
        m_server = nullptr;
    }

    // 方法1：使用新版本API (open62541 1.x+)
    m_server = UA_Server_new();
    if (!m_server) {
        emit errorOccurred("Failed to create OPC UA server");
        emit serverInitialized(false);
        return;
    }

    UA_ServerConfig* config = UA_Server_getConfig(m_server);
    if (!config) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred("Failed to get server config");
        emit serverInitialized(false);
        return;
    }

    // 设置最小化配置（包含端口）
    UA_StatusCode status = UA_ServerConfig_setMinimal(config, port, nullptr);
    if (status != UA_STATUSCODE_GOOD) {
        UA_Server_delete(m_server);
        m_server = nullptr;
        emit errorOccurred(QString("Failed to set minimal config: %1")
                               .arg(UA_StatusCode_name(status)));
        emit serverInitialized(false);
        return;
    }

    // 新版本设置应用URI的方式
    UA_String appUriStr = UA_STRING_ALLOC(appUri.toUtf8().constData());

    // 直接设置到结构体中
    if (config->applicationDescription.applicationUri.data) {
        UA_String_clear(&config->applicationDescription.applicationUri);
    }
    config->applicationDescription.applicationUri = appUriStr;

    // 注意：这里不需要 UA_String_clear(&appUriStr) 因为所有权已转移

    // 添加命名空间
    m_namespaceIndex = UA_Server_addNamespace(m_server,
                                              "urn:industrial:opcua:server");

    qDebug() << "OPC UA server initialized successfully on port" << port;
    emit serverInitialized(true);
}


void OpcUaServerWorker::startServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server || m_running) {
        return;
    }

    // 启动服务器（非阻塞方式）
    UA_StatusCode status = UA_Server_run_startup(m_server);
    if (status != UA_STATUSCODE_GOOD) {
        emit errorOccurred(QString("Server startup failed: %1")
                               .arg(UA_StatusCode_name(status)));
        return;
    }

    m_running = true;

    // 启动服务器循环在独立线程中
    QThread* serverThread = QThread::create([this]() { serverLoop(); });
    connect(serverThread, &QThread::finished, serverThread, &QObject::deleteLater);
    serverThread->start();

    emit serverStarted();
}

void OpcUaServerWorker::stopServer() {
    QMutexLocker locker(&m_serverMutex);

    if (!m_server || !m_running) {
        return;
    }

    m_running = false;

    // 等待操作队列处理完成
    m_queueCondition.wakeAll();

    // 停止服务器
    UA_Server_run_shutdown(m_server);

    emit serverStopped();
}*/

void OpcUaServerWorker::registerVariable(const QString& tagName,
                                         VariableDefinition* varDef) {
    QMutexLocker locker(&m_queueMutex);

    PendingOperation op;
    op.type = PendingOperation::RegisterVariable;
    op.tagName = tagName;
    op.data.setValue(varDef);
    op.timestamp = OpcUaServerWorker::getDateTime_now();

    m_pendingOps.enqueue(op);
    m_queueCondition.wakeOne();
}

void OpcUaServerWorker::updateVariableValue(const QString& tagName,
                                            double value,
                                            UA_DateTime timestamp) {
    QMutexLocker locker(&m_queueMutex);

    PendingOperation op;
    op.type = PendingOperation::UpdateValue;
    op.tagName = tagName;
    op.data.setValue(value);
    op.timestamp = timestamp ? timestamp : getDateTime_now();

    m_pendingOps.enqueue(op);
    m_queueCondition.wakeOne();
}


void OpcUaServerWorker::batchUpdateValues(const QVariant &valuesVariant) {

    QMutexLocker locker(&m_queueMutex);

    QVariantMap map = valuesVariant.toMap();
    QHash<QString, double> values;
    for (auto it = map.begin(); it != map.end(); ++it) {
        values.insert(it.key(), it.value().toDouble());
    }

    PendingOperation op;
    op.type = PendingOperation::BatchUpdate;
    op.data.setValue(values);
    op.timestamp = getDateTime_now();

    m_pendingOps.enqueue(op);
    m_queueCondition.wakeOne();

 /*   QMutexLocker locker(&m_queueMutex);

    PendingOperation op;
    op.type = PendingOperation::BatchUpdate;
    op.data.setValue(values);
    op.timestamp = DateTime_now();

    m_pendingOps.enqueue(op);
    m_queueCondition.wakeOne();*/
}

/*
void OpcUaServerWorker::processOperations() {
    while (true) {
        PendingOperation op;

        // 1. 等待队列中有操作
        {
            QMutexLocker locker(&m_queueMutex);
            while (m_pendingOps.isEmpty()) {
                m_queueCondition.wait(&m_queueMutex);
            }
            op = m_pendingOps.dequeue();
        }

        // 2. 处理操作（需要服务器锁）
        QMutexLocker serverLocker(&m_serverMutex);

        switch (op.type) {
        case PendingOperation::RegisterVariable: {
            // 从 QVariant 中提取 VariableDefinition*
            VariableDefinition* varDef = static_cast<VariableDefinition*>(op.data.value<void*>());
            if (!varDef) {
                qWarning() << "Invalid variable definition in queue for tag:" << op.tagName;
                break;
            }

            // 检查是否已存在（可能在等待时已被注册）
            if (m_variableNodes.contains(op.tagName)) {
                qWarning() << "Variable" << op.tagName << "already registered (from queue)";
                break;
            }

            // 如果服务器已初始化，创建节点
            if (m_server) {
                // ✅ 正确：传入 varDef，不是 tagName
                UA_NodeId* nodeId = createVariableNode(varDef);
                if (nodeId) {
                    m_variableNodes.insert(op.tagName, nodeId);
                    qDebug() << "Registered queued variable:" << op.tagName;
                } else {
                    qWarning() << "Failed to register queued variable:" << op.tagName;
                }
            } else {
                // 服务器仍未初始化，重新放回队列
                qWarning() << "Server still not initialized, re-queuing variable:" << op.tagName;

                QMutexLocker queueLocker(&m_queueMutex);
                m_pendingOps.enqueue(op);  // 重新放入队列
            }
            break;
        }

        case PendingOperation::UpdateValue: {
            // 处理单个变量更新
            if (m_server && m_running) {
                UA_NodeId* nodeId = m_variableNodes.value(op.tagName, nullptr);
                if (nodeId && !UA_NodeId_isNull(nodeId)) {
                    double value = op.data.toDouble();
                    updateNodeValue(nodeId, value, op.timestamp);
                }
            }
            break;
        }

        case PendingOperation::BatchUpdate: {
            // 处理批量更新
            if (m_server && m_running) {
                // 需要 variantToHash 函数
                QHash<QString, double> values = variantToHash(op.data);
                UA_DateTime timestamp = getDateTime_now();

                for (auto it = values.begin(); it != values.end(); ++it) {
                    UA_NodeId* nodeId = m_variableNodes.value(it.key(), nullptr);
                    if (nodeId && !UA_NodeId_isNull(nodeId)) {
                        updateNodeValue(nodeId, it.value(), timestamp);
                    }
                }
            }
            break;
        }
        }
    }
}
*/
void OpcUaServerWorker::processOperations() {
    while (true) {
        PendingOperation op;

        {
            QMutexLocker locker(&m_queueMutex);

            if (m_pendingOps.isEmpty()) {
                m_queueCondition.wait(&m_queueMutex);
                if (!m_running && m_pendingOps.isEmpty()) {
                    break; // 退出循环
                }
                continue;
            }

            op = m_pendingOps.dequeue();
        }

        // 处理操作
        QMutexLocker serverLocker(&m_serverMutex);

        if (!m_server) {
            continue;
        }

        switch (op.type) {
        case PendingOperation::RegisterVariable: {
            VariableDefinition* varDef = op.data.value<VariableDefinition*>();
            if (varDef) {
                UA_NodeId* nodeId = createVariableNode(varDef);
                if (nodeId && !UA_NodeId_isNull(nodeId)) {
                    m_variableNodes.insert(op.tagName, nodeId);
                }
            }
            break;
        }

        case PendingOperation::UpdateValue: {
            if (m_variableNodes.contains(op.tagName)) {
                updateNodeValue(m_variableNodes[op.tagName],
                                op.data.toDouble(),
                                op.timestamp);
            }
            break;
        }

        case PendingOperation::BatchUpdate: {
            QHash<QString, double> values = op.data.value<QHash<QString, double>>();
            for (auto it = values.begin(); it != values.end(); ++it) {
                if (m_variableNodes.contains(it.key())) {
                    updateNodeValue(m_variableNodes[it.key()],
                                    it.value(),
                                    op.timestamp);
                }
            }
            break;
        }
        }
    }
}

void OpcUaServerWorker::serverLoop() {
    QElapsedTimer timer;
    const int ITERATION_DELAY = 100; // ms

    while (m_running) {
        timer.restart();

        {
            QMutexLocker locker(&m_serverMutex);
            if (m_server) {
                // 执行一次服务器迭代（非阻塞）
                UA_Server_run_iterate(m_server, false);
            }
        }

        // 控制循环频率
        int elapsed = timer.elapsed();
        if (elapsed < ITERATION_DELAY) {
            QThread::msleep(ITERATION_DELAY - elapsed);
        }

        // 处理事件循环
        QCoreApplication::processEvents();
    }
}

UA_NodeId* OpcUaServerWorker::createVariableNode(VariableDefinition* varDef) {
    if (!m_server || !varDef) {
        return nullptr;
    }

    UA_NodeId* nodeId = new UA_NodeId;
    *nodeId = UA_NODEID_NUMERIC(0, 0); // 初始化为NULL节点

    // 创建变量属性
    UA_VariableAttributes attr = UA_VariableAttributes_default;

    // 设置显示名称
    QByteArray displayNameBytes = varDef->tagName().toUtf8();
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", displayNameBytes.constData());

    // 设置访问级别
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    if (varDef->writable()) {
        attr.accessLevel |= UA_ACCESSLEVELMASK_WRITE;
    }

    // 设置数据类型和初始值
    double initValue = varDef->initialValue();
    UA_Variant_setScalar(&attr.value, &initValue, &UA_TYPES[UA_TYPES_DOUBLE]);

    // 创建节点ID字符串
    QString nodeIdStr = varDef->tagName().replace('.', '_');
    QByteArray nodeIdBytes = nodeIdStr.toUtf8();
    UA_NodeId requestedId = UA_NODEID_STRING_ALLOC(m_namespaceIndex, nodeIdBytes.constData());

    // 设置节点名称
    UA_QualifiedName nodeName = UA_QUALIFIEDNAME_ALLOC(m_namespaceIndex, nodeIdBytes.constData());

    // 添加节点
    UA_StatusCode status = UA_Server_addVariableNode(
        m_server,
        requestedId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        nodeName,
        UA_NODEID_NULL,
        attr,
        nullptr,
        nodeId);

    // 设置写回调
    if (status == UA_STATUSCODE_GOOD && varDef->writable()) {
        UA_ValueCallback callback;
        callback.onRead = nullptr;
        callback.onWrite = onWriteCallbackWrapper;

        UA_Server_setVariableNode_valueCallback(m_server, *nodeId, callback);

        // 设置节点上下文（指向this）
        UA_Server_setNodeContext(m_server, *nodeId, this);
    }

    // 清理
    UA_VariableAttributes_clear(&attr);
    UA_NodeId_clear(&requestedId);
    UA_QualifiedName_clear(&nodeName);

    if (status != UA_STATUSCODE_GOOD) {
        UA_NodeId_clear(nodeId);
        delete nodeId;
        nodeId = nullptr;
        qWarning() << "Failed to create variable node for" << varDef->tagName()
                   << ":" << UA_StatusCode_name(status);
    }

    return nodeId;
}

void OpcUaServerWorker::updateNodeValue(UA_NodeId* nodeId,
                                        double value,
                                        UA_DateTime timestamp) {
    if (!m_server || !nodeId) {
        return;
    }

    UA_Variant varValue;
    UA_Variant_setScalar(&varValue, &value, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_WriteValue wv;
    UA_WriteValue_init(&wv);
    wv.nodeId = *nodeId;
    wv.attributeId = UA_ATTRIBUTEID_VALUE;
    wv.value.value = varValue;
    wv.value.hasValue = true;
    wv.value.hasSourceTimestamp = true;
    wv.value.sourceTimestamp = timestamp;

    UA_Server_write(m_server, &wv);

    UA_Variant_clear(&varValue);
}

bool OpcUaServerWorker::UA_NodeId_isNull(const UA_NodeId* nodeId) {
    if (!nodeId) return true;
    return nodeId->namespaceIndex == 0 &&
           nodeId->identifierType == UA_NODEIDTYPE_NUMERIC &&
           nodeId->identifier.numeric == 0;
}

void OpcUaServerWorker::UA_NodeId_clear(UA_NodeId* nodeId) {
    if (nodeId) {
        ::UA_NodeId_clear(nodeId);
    }
}

UA_DateTime OpcUaServerWorker::getDateTime_now() {

    return ::UA_DateTime_now();  // 调用 open62541 的原始函数

 /*--------------------这是以前写的时间函数，现在改用open62541的时间函数------------
    // 获取当前时间（从1601-01-01 UTC开始的100纳秒间隔数）
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    // 转换为100纳秒间隔数
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    UA_DateTime dateTime = nanoseconds.count() / 100;

    // OPC UA时间从1601-01-01开始，需要添加从1970-01-01到1601-01-01的差值
    // 11644473600秒 * 10000000（100纳秒/秒）= 116444736000000000
    const UA_DateTime UA_EPOCH_DIFF = 116444736000000000ULL;

    return dateTime + UA_EPOCH_DIFF;*/
}

QString OpcUaServerWorker::getServerStatus()
{
    QMutexLocker locker(&m_serverMutex);

    if (!m_server) {
        return "Not Initialized";
    }

    if (m_running) {
        UA_ServerConfig* config = UA_Server_getConfig(m_server);
        if (config && config->endpointsSize > 0) {
            QString endpoint = QString::fromUtf8(
                (char*)config->endpoints[0].endpointUrl.data,
                config->endpoints[0].endpointUrl.length);
            return QString("Running on %1").arg(endpoint);
        }
        return "Running";
    }

    return "Stopped";
}

QStringList OpcUaServerWorker::getRegisteredVariables()
{
    QMutexLocker locker(&m_serverMutex);
    return m_variableNodes.keys();
}

int OpcUaServerWorker::getVariableCount()
{
    QMutexLocker locker(&m_serverMutex);
    return m_variableNodes.size();
}

bool OpcUaServerWorker::isVariableRegistered(const QString &tagName)
{
    QMutexLocker locker(&m_serverMutex);

    // 直接检查映射表是否存在该变量
    return m_variableNodes.contains(tagName);
}

void OpcUaServerWorker::onWriteCallbackWrapper(UA_Server* server,const UA_NodeId* sessionId,void* sessionContext,
    const UA_NodeId* nodeId,
    void* nodeContext,
    const UA_NumericRange* range,
    const UA_DataValue* data) {

    Q_UNUSED(server);
    Q_UNUSED(sessionId);
    Q_UNUSED(sessionContext);
    Q_UNUSED(range);

    OpcUaServerWorker* worker = static_cast<OpcUaServerWorker*>(nodeContext);
    if (worker) {
        worker->onWriteCallback(nodeId, data);
    }
}

void OpcUaServerWorker::onWriteCallback(const UA_NodeId* nodeId,
                                        const UA_DataValue* data) {
    if (!nodeId || !data) {
        return;
    }

    // 根据节点ID查找标签名
    QString tagName;
    for (auto it = m_variableNodes.begin(); it != m_variableNodes.end(); ++it) {
        if (UA_NodeId_equal(nodeId, it.value())) {
            tagName = it.key();
            break;
        }
    }

    if (tagName.isEmpty()) {
        return;
    }

    // 提取写入的值
    double value = 0.0;
    if (data->value.type == &UA_TYPES[UA_TYPES_DOUBLE]) {
        value = *(UA_Double*)data->value.data;
    } else if (data->value.type == &UA_TYPES[UA_TYPES_INT32]) {
        value = *(UA_Int32*)data->value.data;
    } else if (data->value.type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
        value = *(UA_Boolean*)data->value.data ? 1.0 : 0.0;
    }

    // 发射信号（跨线程）
    emit variableWritten(tagName, value);
}

// ==================== ThreadSafeVariableCache 实现 ====================

void OpcUaThreadedServer::ThreadSafeVariableCache::update(
    const QString& tagName, double value, UA_DateTime timestamp) {

    QWriteLocker locker(&m_cacheLock);

    if (!m_cache.contains(tagName)) {
        m_cache[tagName] = new CachedVariable();
        m_cache[tagName]->dirty.store(false);
    }

    CachedVariable* var = m_cache[tagName];
    QWriteLocker varLocker(&var->lock);

    var->value = value;
    var->timestamp = timestamp;
    var->dirty.store(true, std::memory_order_release);
}

bool OpcUaThreadedServer::ThreadSafeVariableCache::get(
    const QString& tagName, double& value, UA_DateTime& timestamp) {

    QReadLocker locker(&m_cacheLock);

    if (!m_cache.contains(tagName)) {
        return false;
    }

    CachedVariable* var = m_cache[tagName];
    QReadLocker varLocker(&var->lock);

    value = var->value;
    timestamp = var->timestamp;
    return true;
}

QHash<QString, double> OpcUaThreadedServer::ThreadSafeVariableCache::getDirtyValues() {
    QHash<QString, double> dirtyValues;

    QWriteLocker locker(&m_cacheLock);

    for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
        CachedVariable* var = it.value();
        QReadLocker varLocker(&var->lock);

        if (var->dirty.load(std::memory_order_acquire)) {
            dirtyValues.insert(it.key(), var->value);
            var->dirty.store(false, std::memory_order_release);
        }
    }

    return dirtyValues;
}

void OpcUaThreadedServer::ThreadSafeVariableCache::clear() {
    QWriteLocker locker(&m_cacheLock);

    for (CachedVariable* var : m_cache) {
        delete var;
    }
    m_cache.clear();
}

// ==================== OpcUaThreadedServer 实现 ====================

OpcUaThreadedServer::OpcUaThreadedServer(QObject* parent)
    : QObject(parent)
    , m_worker(new OpcUaServerWorker())
    , m_workerThread(new QThread(this))
    , m_batchTimer(new QTimer(this))
    , m_running(false) {

    // 初始化统计信息
    m_stats.writeCount = 0;
    m_stats.readCount = 0;
    m_stats.errorCount = 0;
    m_stats.avgUpdateLatency = 0.0;

    // 将worker移到独立线程
    m_worker->moveToThread(m_workerThread);

    // 连接信号
    connect(m_worker, &OpcUaServerWorker::variableWritten,
            this, &OpcUaThreadedServer::onWorkerVariableWritten);

    connect(m_worker, &OpcUaServerWorker::serverStarted,
            this, [this]() {
                m_running = true;
                emit serverStatusChanged(true);
            });

    connect(m_worker, &OpcUaServerWorker::serverStopped,
            this, [this]() {
                m_running = false;
                emit serverStatusChanged(false);
            });

    connect(m_workerThread, &QThread::finished,
            m_worker, &QObject::deleteLater);

    // 启动线程
    m_workerThread->start();

    // 配置批量更新定时器
    m_batchTimer->setInterval(50); // 50ms批量刷新
    connect(m_batchTimer, &QTimer::timeout,
            this, &OpcUaThreadedServer::flushPendingUpdates);
}

OpcUaThreadedServer::~OpcUaThreadedServer() {
    stop();

    m_workerThread->quit();
    m_workerThread->wait();

    m_valueCache.clear();
}

bool OpcUaThreadedServer::configure(const QString& appUri,
                                    const QString& appName,
                                    quint16 port) {
    Q_UNUSED(appName);

    // 异步初始化服务器
    QMetaObject::invokeMethod(m_worker, "initializeServer",
                              Qt::QueuedConnection,
                              Q_ARG(QString, appUri),
                              Q_ARG(quint16, port));

    return true;
}

bool OpcUaThreadedServer::registerVariable(VariableDefinition* varDef) {
    if (!varDef) {
        return false;
    }

    // 验证变量
    if (!varDef->validate()) {
        qWarning() << "Variable validation failed for" << varDef->tagName();
        return false;
    }

    // 缓存变量值
    m_valueCache.update(varDef->tagName(), varDef->initialValue(),
                       OpcUaServerWorker::getDateTime_now());

    // 异步注册到服务器
    QMetaObject::invokeMethod(m_worker, "registerVariable",
                              Qt::QueuedConnection,
                              Q_ARG(QString, varDef->tagName()),
                              Q_ARG(VariableDefinition*, varDef));

    return true;
}

bool OpcUaThreadedServer::batchRegisterVariables(const QList<VariableDefinition*>& varDefs) {
    for (VariableDefinition* varDef : varDefs) {
        if (!registerVariable(varDef)) {
            return false;
        }
    }
    return true;
}

void OpcUaThreadedServer::updateValue(const QString& tagName, double value) {

    // 更新统计
    updateStats(1, 0, 0);

    // 更新缓存
    m_valueCache.update(tagName, value, OpcUaServerWorker::getDateTime_now());

    // 添加到批量更新队列
    {
        QMutexLocker locker(&m_updateMutex);
        m_pendingUpdates.insert(tagName, value);
    }

    // 启动批量定时器（如果未运行）
    if (!m_batchTimer->isActive()) {
        m_batchTimer->start();
    }

}

void OpcUaThreadedServer::batchUpdateValues(const QHash<QString, double>& values) {
    // 批量更新缓存
    UA_DateTime now = OpcUaServerWorker::getDateTime_now();
    for (auto it = values.begin(); it != values.end(); ++it) {
        m_valueCache.update(it.key(), it.value(), now);
    }

    // 转换为 QVariant 再发送
    QVariant valuesVariant = hashToVariant(values);
    QMetaObject::invokeMethod(m_worker, "batchUpdateValues",
                              Qt::QueuedConnection,
                              Q_ARG(QVariant, valuesVariant));

 /*   UA_DateTime now = OpcUaServerWorker::DateTime_now();
    for (auto it = values.begin(); it != values.end(); ++it) {
        m_valueCache.update(it.key(), it.value(), now);
    }

    // 批量发送到服务器
    QMetaObject::invokeMethod(m_worker, "batchUpdateValues",
                              Qt::QueuedConnection,
                              Q_ARG((QHash<QString, double>), values));*/
}

bool OpcUaThreadedServer::start() {
    if (m_running) {
        return true;
    }

    QMetaObject::invokeMethod(m_worker, "startServer",
                              Qt::QueuedConnection);

    // 启动批量更新定时器
    m_batchTimer->start();

    return true;
}

void OpcUaThreadedServer::stop() {
    // 停止批量定时器
    m_batchTimer->stop();

    // 刷新所有待更新
    flushPendingUpdates();

    // 停止服务器
    QMetaObject::invokeMethod(m_worker, "stopServer",
                              Qt::QueuedConnection);
}

bool OpcUaThreadedServer::isRunning() const {
    return m_running;
}

void OpcUaThreadedServer::resetStats()
{
    QMutexLocker locker(&m_statsMutex);
    m_stats.writeCount = 0;
    m_stats.readCount = 0;
    m_stats.errorCount = 0;
    m_stats.avgUpdateLatency = 0.0;
}

OpcUaThreadedServer::ServerStats OpcUaThreadedServer::getStats() const {
    QMutexLocker locker(&m_statsMutex);
    return m_stats;
}

void OpcUaThreadedServer::onWorkerVariableWritten(const QString& tagName,
                                                  double value) {
    // 更新统计信息
    updateStats(1, 0, 0);

    // OPC UA客户端写入的值，通知上层应用
    emit variableValueChanged(tagName, value);
}

void OpcUaThreadedServer::flushPendingUpdates() {
    QHash<QString, double> updates;

    {
        QMutexLocker locker(&m_updateMutex);
        updates = std::move(m_pendingUpdates);
        m_pendingUpdates.clear();
    }

    if (!updates.isEmpty()) {
        // 批量发送到服务器
        QVariant updatesVariant = hashToVariant(updates);
        QMetaObject::invokeMethod(m_worker, "batchUpdateValues",
                                  Qt::QueuedConnection,
                                  Q_ARG(QVariant, updatesVariant));

        // 更新统计信息
        updateStats(updates.size(), 0, 0);
    }

    // 如果没有更多更新，停止定时器
    {
        QMutexLocker locker(&m_updateMutex);
        if (m_pendingUpdates.isEmpty()) {
            m_batchTimer->stop();
        }
    }


/*    QHash<QString, double> updates;

    {
        QMutexLocker locker(&m_updateMutex);
        updates = std::move(m_pendingUpdates);
        m_pendingUpdates.clear();
    }

    if (!updates.isEmpty()) {
        // 批量发送到服务器
        QMetaObject::invokeMethod(m_worker, "batchUpdateValues",
                                  Qt::QueuedConnection,
                                  Q_ARG((QHash<QString, double>), updates));

        // 更新统计信息
        updateStats(updates.size(), 0, 0);
    }

    // 如果没有更多更新，停止定时器
    {
        QMutexLocker locker(&m_updateMutex);
        if (m_pendingUpdates.isEmpty()) {
            m_batchTimer->stop();
        }
    }*/
}

void OpcUaThreadedServer::updateStats(quint64 writes, quint64 reads, quint64 errors) {
    QMutexLocker locker(&m_statsMutex);
    m_stats.writeCount += writes;
    m_stats.readCount += reads;
    m_stats.errorCount += errors;

    // 这里可以计算平均延迟等更复杂的统计信息
}

QVariant OpcUaThreadedServer::hashToVariant(const QHash<QString, double> &hash)
{
    QVariantMap map;
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        map.insert(it.key(), it.value());
    }
    return map;
}

QHash<QString, double> OpcUaThreadedServer::variantToHash(const QVariant &variant)
{
    QHash<QString, double> hash;
    QVariantMap map = variant.toMap();
    for (auto it = map.begin(); it != map.end(); ++it) {
        hash.insert(it.key(), it.value().toDouble());
    }
    return hash;
}

} // namespace Industrial
