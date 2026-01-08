// OPCUAClientManager.cpp - 修复连接问题

#include "opcuaclientmanager.h"
#include <QDebug>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMutexLocker>
#include <QReadLocker>
#include <QWriteLocker>
#include <QCoreApplication>
#include <cmath>
#include <random>
#include <QRandomGenerator>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

/* ------------------------------------open62541的回调机制-----------------------------------------
您的 Qt 程序                          open62541 库                          OPC UA 服务器
    |                                      |                                      |
    |--- m_processTimer 触发 (每200ms) ---> |                                      |
    |                                      |--- UA_Client_run_iterate() --------->|
    |                                      |     ↓                                |
    |                                      |    "有数据吗？"                        |
    |                                      |                                      |
    |                                      |<--- "有，订阅25有新数据" ------------   |
    |                                      |     ↓                                |
    |                                      | 查找订阅25的回调函数                    |
    |                                      |     ↓                                |
    |<-- dataChangeNotificationCallback()--| 调用您的回调                           |
    |     (看起来像服务器主动调用)             |                                      |
    |                                      |                                      |


*/

/*
// 信号传递链：
ConnectionManager::reconnecting(int, int)  // 底层发出
    ↓ 信号连接
OPCUAVariableManager::onReconnecting(int, int)  // 信号处理函数
    ↓ 转发信号
OPCUAVariableManager::reconnecting(int, int)     // 上层信号

*/

/*
OPC UA 服务器 → OPCUAVariableManager → VariableDefinition → 客户端
    ↓ 订阅/读取        ↓ 转发数据        ↓ 存储并发射信号

客户端 → OPCUAVariableManager → OPC UA 服务器 → VariableDefinition
    ↓ 请求写入        ↓ 执行OPC UA写入   ↓ 服务器确认后更新
*/

//下层 (OPCUAConnectionManager)：负责心跳、检测失败、执行重连


//上层 (OPCUAVariableManager)：负责变量管理、信号转发、资源清理

// ==================== 辅助函数 ====================
namespace Industrial {


// QString 到 UA_String
static UA_String qStringToUAString(const QString &qStr) {
    UA_String uaStr;
    QByteArray bytes = qStr.toUtf8();
    uaStr.length = bytes.length();
    uaStr.data = (UA_Byte*)UA_malloc(uaStr.length * sizeof(UA_Byte));
    if (uaStr.data) {
        memcpy(uaStr.data, bytes.constData(), uaStr.length);
    }
    return uaStr;
}

// 生成随机客户端句柄
static UA_UInt32 generateClientHandle() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<UA_UInt32> dis(1, 0x7FFFFFFF);
    return dis(gen);
}

QVariant publicUaVariantToQVariant(const UA_Variant &variant)
{

    if (!variant.data || !variant.type) {

        return QVariant();
    }
    // 🔴 关键修复：检查数组维度
    if (variant.arrayLength > 0 || variant.arrayDimensionsSize > 0) {
        // 尝试处理单元素数组
        if (variant.arrayLength == 1) {
            // 获取数组指针
            void* arrayData = variant.data;

            if (variant.type == &UA_TYPES[UA_TYPES_FLOAT]) {
                UA_Float* floatArray = (UA_Float*)arrayData;
                UA_Float floatValue = floatArray[0];
                return QVariant(static_cast<double>(floatValue));
            }
            else if (variant.type == &UA_TYPES[UA_TYPES_DOUBLE]) {
                UA_Double* doubleArray = (UA_Double*)arrayData;
                UA_Double doubleValue = doubleArray[0];
                return QVariant(doubleValue);
            }
        }
        //qWarning() << "Array type not supported:"
         //          << (variant.type ? variant.type->typeName : "unknown");
        return QVariant();
    }

    // 处理标量数据
    const UA_DataType* type = variant.type;

    // 🔴 改进的 float 处理
    if (type == &UA_TYPES[UA_TYPES_FLOAT]) {
        try {
            UA_Float rawValue = *(UA_Float*)variant.data;
           // qDebug() << "原始UA_Float值:" << rawValue;

            // 转换为 double
            double convertedValue = static_cast<double>(rawValue);
            QVariant result = QVariant(convertedValue);

            //qDebug() << "转换后double值:" << convertedValue;
            //qDebug() << "QVariant有效性:" << result.isValid();
           // qDebug() << "QVariant类型:" << result.typeName();

            return result;
        } catch (...) {
            qWarning() << "处理UA_Float时发生异常";
            return QVariant();
        }
    }
    else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {
        try {
            UA_Double rawValue = *(UA_Double*)variant.data;
            //qDebug() << "原始UA_Double值:" << rawValue;

            QVariant result = QVariant(rawValue);
           // qDebug() << "QVariant有效性:" << result.isValid();
           // qDebug() << "QVariant类型:" << result.typeName();

            return result;
        } catch (...) {
            qWarning() << "处理UA_Double时发生异常";
            return QVariant();
        }
    }
    // ... 其他类型处理

    qWarning() << "未支持的OPC UA类型:" << type->typeName;
    return QVariant();
}

UA_Variant publicQVariantToUAVariant(const QVariant &qtVariant,
                                                     const UA_DataType* expectedType)
{
    UA_Variant uaVariant;
    UA_Variant_init(&uaVariant);

   // qDebug() << "\n=== qVariantToUAVariant ===";
   // qDebug() << "Input QVariant:" << qtVariant << "type:" << qtVariant.typeName()
    //         << "type id:" << qtVariant.userType();
   // qDebug() << "Expected OPC UA type:" << (expectedType ? expectedType->typeName : "null");

    if (!qtVariant.isValid()) {
        qDebug() << "Invalid QVariant";
        return uaVariant;
    }

    // 如果有期望类型，按期望类型转换
    if (expectedType) {
        bool converted = false;

        // 布尔类型
        if (expectedType == &UA_TYPES[UA_TYPES_BOOLEAN]) {
            UA_Boolean value = qtVariant.toBool();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Boolean:" << value << "(from" << qtVariant << ")";
        }
        // 双精度浮点数
        else if (expectedType == &UA_TYPES[UA_TYPES_DOUBLE]) {
            UA_Double value = qtVariant.toDouble();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Double:" << value << "(from" << qtVariant << ")";
        }
        // 单精度浮点数 - 关键修复！
        else if (expectedType == &UA_TYPES[UA_TYPES_FLOAT]) {
            // 支持从各种类型转换为 Float
            if (qtVariant.canConvert<float>()) {
                UA_Float value = qtVariant.toFloat();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to Float:" << value << "(from" << qtVariant << ")";
            } else {
                // 尝试从 double 转换
                double doubleValue = qtVariant.toDouble();
                UA_Float value = static_cast<UA_Float>(doubleValue);
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted double to Float:" << value << "(from" << doubleValue << ")";
            }
        }
        // 32位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT32]) {
            if (qtVariant.canConvert<int>()) {
                UA_Int32 value = qtVariant.toInt();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to Int32:" << value << "(from" << qtVariant << ")";
            } else {
                // 尝试从 double 转换
                double doubleValue = qtVariant.toDouble();
                UA_Int32 value = static_cast<UA_Int32>(doubleValue);
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted double to Int32:" << value << "(from" << doubleValue << ")";
            }
        }
        // 无符号32位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_UINT32]) {
            if (qtVariant.canConvert<uint>()) {
                UA_UInt32 value = qtVariant.toUInt();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to UInt32:" << value << "(from" << qtVariant << ")";
            }
        }
        // 16位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT16]) {
            UA_Int16 value = static_cast<UA_Int16>(qtVariant.toInt());
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Int16:" << value << "(from" << qtVariant << ")";
        }
        // 64位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT64]) {
            UA_Int64 value = static_cast<UA_Int64>(qtVariant.toLongLong());
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Int64:" << value << "(from" << qtVariant << ")";
        }
        // 字符串
        else if (expectedType == &UA_TYPES[UA_TYPES_STRING]) {
            QString str = qtVariant.toString();
            QByteArray utf8 = str.toUtf8();

            // 安全地分配字符串内存
            UA_String *uaStr = (UA_String*)UA_malloc(sizeof(UA_String));
            if (uaStr) {
                uaStr->length = utf8.length();
                uaStr->data = (UA_Byte*)UA_malloc(uaStr->length);
                if (uaStr->data) {
                    memcpy(uaStr->data, utf8.constData(), uaStr->length);
                    UA_Variant_setScalar(&uaVariant, uaStr, expectedType);
                    converted = true;
                    qDebug() << "Converted to String:" << str << "(from" << qtVariant << ")";
                } else {
                    UA_free(uaStr);
                }
            }
        }
        // 字节串
        else if (expectedType == &UA_TYPES[UA_TYPES_BYTESTRING]) {
            QByteArray bytes = qtVariant.toByteArray();
            UA_ByteString *uaBytes = (UA_ByteString*)UA_malloc(sizeof(UA_ByteString));
            if (uaBytes) {
                uaBytes->length = bytes.length();
                uaBytes->data = (UA_Byte*)UA_malloc(uaBytes->length);
                if (uaBytes->data) {
                    memcpy(uaBytes->data, bytes.constData(), uaBytes->length);
                    UA_Variant_setScalar(&uaVariant, uaBytes, expectedType);
                    converted = true;
                    qDebug() << "Converted to ByteString, length:" << bytes.length();
                } else {
                    UA_free(uaBytes);
                }
            }
        }
        // 日期时间
        else if (expectedType == &UA_TYPES[UA_TYPES_DATETIME]) {
            QDateTime dt = qtVariant.toDateTime();
            if (dt.isValid()) {
                UA_DateTime uaDt = UA_DateTime_fromUnixTime(dt.toMSecsSinceEpoch() / 1000);
                UA_Variant_setScalarCopy(&uaVariant, &uaDt, expectedType);
                converted = true;
                qDebug() << "Converted to DateTime:" << dt.toString();
            }
        }

        if (!converted) {
            qWarning() << "Cannot convert QVariant to expected OPC UA type:"
                       << (expectedType ? expectedType->typeName : "null");
            qDebug() << "QVariant value:" << qtVariant << "type:" << qtVariant.typeName();

            // 尝试最后的自动转换
            qDebug() << "Attempting fallback conversion...";
            switch (qtVariant.userType()) {
            case QMetaType::Double:
            case QMetaType::Float:
                if (expectedType == &UA_TYPES[UA_TYPES_INT32]) {
                    UA_Int32 value = static_cast<UA_Int32>(qtVariant.toDouble());
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted float/double to Int32:" << value;
                }
                else if (expectedType == &UA_TYPES[UA_TYPES_FLOAT]) {
                    UA_Float value = qtVariant.toFloat();
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted to Float:" << value;
                }
                else if (expectedType == &UA_TYPES[UA_TYPES_DOUBLE]) {
                    UA_Double value = qtVariant.toDouble();
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted to Double:" << value;
                }
                break;
            }
        }

        qDebug() << "Conversion result:" << (converted ? "success" : "failed");
        return uaVariant;
    }

    // ========== 自动检测类型（无期望类型时）==========
   // qDebug() << "No expected type specified, auto-detecting...";

    int variantType = qtVariant.userType();

    // 布尔类型
    if (variantType == QMetaType::Bool) {
        UA_Boolean value = qtVariant.toBool();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
        qDebug() << "Auto-converted to Boolean:" << value;
    }
    // 双精度浮点数
    else if (variantType == QMetaType::Double) {
        UA_Double value = qtVariant.toDouble();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
        qDebug() << "Auto-converted to Double:" << value;
    }
    // 单精度浮点数 - 关键修复！
    else if (variantType == QMetaType::Float) {
        UA_Float value = qtVariant.toFloat();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_FLOAT]);
        qDebug() << "Auto-converted to Float:" << value;
    }
    // 整数类型
    else if (variantType == QMetaType::Int ||
             variantType == QMetaType::UInt ||
             variantType == QMetaType::Short ||
             variantType == QMetaType::UShort) {
        UA_Int32 value = qtVariant.toInt();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_INT32]);
        qDebug() << "Auto-converted to Int32:" << value;
    }
    // 大整数类型
    else if (variantType == QMetaType::Long ||
             variantType == QMetaType::LongLong ||
             variantType == QMetaType::ULong ||
             variantType == QMetaType::ULongLong) {
        qint64 value = qtVariant.toLongLong();
        if (value >= INT32_MIN && value <= INT32_MAX) {
            UA_Int32 val32 = static_cast<UA_Int32>(value);
            UA_Variant_setScalarCopy(&uaVariant, &val32, &UA_TYPES[UA_TYPES_INT32]);
            qDebug() << "Auto-converted long to Int32:" << val32;
        } else {
            UA_Int64 val64 = static_cast<UA_Int64>(value);
            UA_Variant_setScalarCopy(&uaVariant, &val64, &UA_TYPES[UA_TYPES_INT64]);
            qDebug() << "Auto-converted to Int64:" << val64;
        }
    }
    // 字符串
    else if (variantType == QMetaType::QString) {
        QString str = qtVariant.toString();
        QByteArray utf8 = str.toUtf8();

        UA_String *uaStr = (UA_String*)UA_malloc(sizeof(UA_String));
        if (uaStr) {
            uaStr->length = utf8.length();
            uaStr->data = (UA_Byte*)UA_malloc(uaStr->length);
            if (uaStr->data) {
                memcpy(uaStr->data, utf8.constData(), uaStr->length);
                UA_Variant_setScalar(&uaVariant, uaStr, &UA_TYPES[UA_TYPES_STRING]);
                qDebug() << "Auto-converted to String:" << str;
            } else {
                UA_free(uaStr);
            }
        }
    }
    // 字节数组
    else if (variantType == QMetaType::QByteArray) {
        QByteArray bytes = qtVariant.toByteArray();
        UA_ByteString *uaBytes = (UA_ByteString*)UA_malloc(sizeof(UA_ByteString));
        if (uaBytes) {
            uaBytes->length = bytes.length();
            uaBytes->data = (UA_Byte*)UA_malloc(uaBytes->length);
            if (uaBytes->data) {
                memcpy(uaBytes->data, bytes.constData(), uaBytes->length);
                UA_Variant_setScalar(&uaVariant, uaBytes, &UA_TYPES[UA_TYPES_BYTESTRING]);
                qDebug() << "Auto-converted to ByteString, length:" << bytes.length();
            } else {
                UA_free(uaBytes);
            }
        }
    }
    // 日期时间
    else if (variantType == QMetaType::QDateTime) {
        QDateTime dt = qtVariant.toDateTime();
        if (dt.isValid()) {
            UA_DateTime uaDt = UA_DateTime_fromUnixTime(dt.toMSecsSinceEpoch() / 1000);
            UA_Variant_setScalarCopy(&uaVariant, &uaDt, &UA_TYPES[UA_TYPES_DATETIME]);
            qDebug() << "Auto-converted to DateTime:" << dt.toString();
        }
    }
    // 未知类型
    else {
        qWarning() << "Cannot auto-convert QVariant type:" << qtVariant.typeName()
        << "(type id:" << variantType << ")";
        qDebug() << "QVariant value:" << qtVariant;

        // 尝试通用转换
        if (qtVariant.canConvert<double>()) {
            UA_Double value = qtVariant.toDouble();
            UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
            qDebug() << "Generic conversion to Double:" << value;
        }
    }

    qDebug() << "Auto-conversion result:" << (uaVariant.data ? "success" : "failed");
    return uaVariant;
}


} // namespace Industrial


// ==================== OPCUAConnectionManager 完整实现 ====================
namespace Industrial {



OPCUAConnectionManager::OPCUAConnectionManager(QObject *parent) : QObject(parent)
                                                                , m_client(nullptr)
                                                                , m_state(STATE_DISCONNECTED)
                                                                , m_lastKeepaliveTime(0)
                                                                , m_lastActivityTime(0)
                                                                , m_reconnectAttempt(0)
{
    // 初始化定时器
    m_keepaliveTimer = new QTimer(this);//初始化心跳定时器
    m_keepaliveTimer->setInterval(m_policy.keepaliveInterval);//设置心跳定时器心跳间隔
    QObject::connect(m_keepaliveTimer, &QTimer::timeout, this, &OPCUAConnectionManager::onKeepaliveTimer);//连接的心跳检测

    m_reconnectTimer = new QTimer(this);// 初始化重连定时器
    m_reconnectTimer->setSingleShot(true);//定启动定时，只触发一次
    QObject::connect(m_reconnectTimer, &QTimer::timeout, this, &OPCUAConnectionManager::onReconnectTimer);//在连接断开后自动尝试重新连接

    // ==================== 初始化 OPC UA 客户端 ====================
    m_client = UA_Client_new();//创建一个新的 OPC UA 客户端

    if (!m_client) {//
        // 客户端创建失败
        qCritical() << "Failed to create OPC UA client";
        recordError("Client creation failed");
        updateState(STATE_ERROR);
        return;  // 重要：创建失败直接返回，不继续初始化
    }

    // 获取客户端配置
    UA_ClientConfig *config = UA_Client_getConfig(m_client);
    if (!config) {
        // 获取配置失败
        qCritical() << "Failed to get client config";
        UA_Client_delete(m_client);
        m_client = nullptr;
        recordError("Failed to get client config");
        updateState(STATE_ERROR);
        return;
    }
   // config->outstandingPublishRequests = 3;  // 从日志看最终稳定在 3
    config->outStandingPublishRequests= 10;
    UA_ClientConfig_setDefault(config);// 设置默认配置

    config->timeout = 10000;  // 设置客户端等待服务器响应的最长时间
    // 设置客户端描述信息（可选，但推荐）
    config->clientDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC("en-US", "OPC UA Client");
    config->clientDescription.applicationUri = UA_STRING_ALLOC("urn:Industrial:OPCUAClient");
    config->clientDescription.productUri = UA_STRING_ALLOC("urn:Industrial:OPCUAClient");
    config->clientDescription.applicationType = UA_APPLICATIONTYPE_CLIENT;

    qDebug() << "OPC UA client initialized successfully with default config";
    updateState(STATE_DISCONNECTED);  // 设置为断开状态（正常）// 初始状态，未连接
}


OPCUAConnectionManager::~OPCUAConnectionManager()//析构流程
{   
    disconnect();// 停止所有活动

    // 清理 OPC UA 客户端
    if (m_client) {
        UA_ClientConfig *config = UA_Client_getConfig(m_client);
        if (config) {
            UA_LocalizedText_clear(&config->clientDescription.applicationName);
            UA_String_clear(&config->clientDescription.applicationUri);
            UA_String_clear(&config->clientDescription.productUri);
        }
        UA_Client_delete(m_client);//销毁链接
        m_client = nullptr;
    }
    qDebug() << "OPCUAConnectionManager destroyed";
}

//----------------------------------公共函数---------------------------------------------

bool OPCUAConnectionManager::connectToserver(const QString &endpointUrl,//链接服务器
                                             const QString &username,
                                             const QString &password)
{
   // QMutexLocker locker(&m_mutex);
     QWriteLocker locker(&m_rwLock);
    if (m_state.load() == STATE_CONNECTED ||
        m_state.load() == STATE_CONNECTING) {
        logConnectionAttempt("Already connected or connecting");//连接日志
        return m_state.load() == STATE_CONNECTED;//原子读保证线程安全
    }

    updateState(STATE_CONNECTING);//更新为链接状态状态
    clearError();  // 开始新连接时清除旧错误

    m_endpointUrl = endpointUrl;
    m_username = username;
    m_password = password;
    m_reconnectAttempt.store(0);

    bool success = performConnection();//连接执行函数，链接成功
    if (success) {
        recordConnectionSuccess();//记录连接
        updateState(STATE_CONNECTED);//更新连接状态

        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();//获得当前时间
        m_lastKeepaliveTime.store(currentTime);// 保存最后心跳时间
        m_lastActivityTime.store(currentTime);// 保存最后活动时间
        m_keepaliveTimer->start();// 启动心跳定时器
        QString message="The server is connected";
        logConnectionAttempt(message);//连接日志
        emit connected();//发送链接的信号
    } else {
        recordConnectionFailure();//记录链接故障的日子
        updateState(STATE_ERROR);//更新连接状态为ERR
        QString message="Link server error";
        logConnectionAttempt(message);//记录连接尝试的日志
        scheduleReconnect();//安排延迟重连
    }

    return success;
}

void OPCUAConnectionManager::disconnect()//断开链接
{
     QWriteLocker locker(&m_rwLock);  // 写锁，独占

    if (m_state.load() == STATE_DISCONNECTED) {//如果链接状态为断开，那么返回
         logConnectionAttempt("Server disconnected");//连接日志
        return;
    }
    ConnectionState oldState = m_state.load();//连接状态
    updateState(STATE_DISCONNECTED);//更新连接状态
    logConnectionAttempt("Server disconnected");//连接日志
    // 停止定时器
    m_keepaliveTimer->stop();//停止心跳
    m_reconnectTimer->stop();// 重连定时器

    // 断开连接
    if (m_client) {
        UA_Client_disconnect(m_client);//断开链接
    }
    m_stats.lastDisconnectTime = QDateTime::currentDateTime();
    emit disconnected();
}

bool OPCUAConnectionManager::isConnected() const//返回连接状态
{
    return m_state.load() == STATE_CONNECTED;
}

UA_Client* OPCUAConnectionManager::client() const//返回客户端
{
    QReadLocker locker(&m_rwLock);
    return m_client;
}

void OPCUAConnectionManager::setReconnectPolicy(const ReconnectPolicy &policy)//设定重链策略
{
    QMutexLocker locker(&m_mutex);
    m_policy = policy;
    m_keepaliveTimer->setInterval(policy.keepaliveInterval);
    logConnectionAttempt("Set the ReconnectPolicy");//连接日志
}

SessionStatistics OPCUAConnectionManager::statistics() const//获得统计信息
{
    QMutexLocker locker(&m_mutex);
    return m_stats;
}

void OPCUAConnectionManager::resetStatistics()//重置连接统计信息
{
    QMutexLocker locker(&m_mutex);
    m_stats = SessionStatistics();
    logConnectionAttempt("reset the ReconnectPolicy");//连接日志
}

QString OPCUAConnectionManager::endpointUrl() const//返回url
{
    //QMutexLocker locker(&m_mutex);
    QReadLocker locker(&m_rwLock);
    return m_endpointUrl;
}

QString OPCUAConnectionManager::username() const//返回用户名
{
    //QMutexLocker locker(&m_mutex);
    QReadLocker locker(&m_rwLock);
    return m_username;
}

bool OPCUAConnectionManager::isClientValid() const//检查OPC UA客户端是否有效。
{
    //QMutexLocker locker(&m_mutex);
    QReadLocker locker(&m_rwLock);
    if (!m_client) {
        return false;
    }

    // 检查客户端配置是否有效
    UA_ClientConfig *config = UA_Client_getConfig(m_client);
    if (!config) {
        return false;
    }

    return true;

}

QString OPCUAConnectionManager::getLastError() const//获得最后的错误
{
    QMutexLocker locker(&m_errorMutex);
    return m_lastError.isEmpty() ? "No error" : m_lastError;
}

QString OPCUAConnectionManager::connectionStateName() const//将输出状态转为字符串
{
    switch (m_state.load()) {
    case STATE_DISCONNECTED:
        return "Disconnected";
    case STATE_CONNECTING:
        return "Connecting";
    case STATE_CONNECTED:
        return "Connected";
    case STATE_RECONNECTING:
        return "Reconnecting";
    case STATE_ERROR:
        return "Error";
    default:
        return "Unknown";
    }
}

//-------------------------------------私有函数-------------------------------------

void OPCUAConnectionManager::onKeepaliveTimer()// onKeepaliveTimer 使用读锁
{
    QReadLocker locker(&m_rwLock);  // 读锁，多个读可以并发

    if (m_state.load() != STATE_CONNECTED) {//当检测到状态不是链接状态时，心跳退出，保证不会影响迟滞链接
        return;
    }
    //isKeepaliveExpired()以前调用检查心跳是否超时，但是起始就是按超时时间链接发送心跳，无意义，故取消
    sendKeepalive();  // sendKeepalive 内部使用写锁
}

void OPCUAConnectionManager::onReconnectTimer()//从新连接
{
    reconnect();//重新连接
}

bool OPCUAConnectionManager::sendKeepalive()//心跳发送
{
    QMutexLocker locker(&m_mutex);  // 添加互斥锁
    if (!m_client || m_state.load() != STATE_CONNECTED) {
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    try {

        UA_DateTime serverTime;
        bool success = getServerTime(serverTime);//读取服务器时间
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (success) {
            m_lastKeepaliveTime.store(currentTime);// 最后心跳时间
            m_lastActivityTime.store(currentTime);// 最后活动时间timer.elapsed()
            QString message =QString("Keepalive successful in%1ms")
                                    .arg(timer.elapsed());
            emit keepaliveReceived();//连接好着
            return true;
        } else {
            recordError("Keepalive failed - cannot read server time");
            updateState(STATE_RECONNECTING);
            QString message =QString("Keepalive failed, connection may be lost");
            logConnectionAttempt(message);
            emit connectionLost();
            scheduleReconnect();//如果链接断开，启动迟滞
            return false;
        }
    } catch (const std::exception &e) {//异常处理
        QString message =QString("Exception during keepalive: %1").arg(e.what());
        recordError(message);
        logConnectionAttempt(message);
        return false;
    } catch (...) {
        recordError("Unknown exception during keepalive");
        logConnectionAttempt("Unknown exception during keepalive");
        return false;
    }
}

bool OPCUAConnectionManager::isKeepaliveExpired() const//检查心跳是否已超时已弃用。
{//当前时间减去-最后时间一次时间，大于心跳策略，那么返回真
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 lastKeepalive = m_lastKeepaliveTime.load();

    return (currentTime - lastKeepalive) > m_policy.keepaliveTimeout;
}

void OPCUAConnectionManager::scheduleReconnect()//延迟重连
{
    // 统一检查最大重试次数（无论是否指数退避）
    if (m_policy.maxRetries > 0 && m_reconnectAttempt >= m_policy.maxRetries) {
        qWarning() << "Maximum reconnection attempts reached (" << m_policy.maxRetries << ")";
        updateState(STATE_ERROR);
        recordError(QString("Maximum reconnection attempts (%1) reached").arg(m_policy.maxRetries));
        emit connectionError("Maximum reconnection attempts reached");
        return;
    }

    int delay = calculateReconnectDelay();

    QString message=QString(" reconnection attempt: %1  delay:%2ms")
                          .arg(m_reconnectAttempt + 1)
                          .arg(delay);
    logConnectionAttempt(message);//连接日志
    m_reconnectTimer->start(delay);//重连
    emit reconnecting(m_reconnectAttempt + 1, delay);
}

int OPCUAConnectionManager::calculateReconnectDelay()//智能重连延迟计算函数指数退避
{
    if (!m_policy.exponentialBackoff) {
        return m_policy.initialDelay;
    }

    // 获取原子值
    int attempts = m_reconnectAttempt.load();
    // 指数退避算法
    int delay = m_policy.initialDelay;
    for (int i = 0; i < attempts && delay <= m_policy.maxDelay / m_policy.delayMultiplier; i++) {
        delay *= m_policy.delayMultiplier;
    }

    // 限制最大延迟
    if (delay > m_policy.maxDelay) {
        delay = m_policy.maxDelay;
    }

    // 添加随机抖动（±10%）避免多个客户端同时重连
    if (delay > 0) {
        int jitter = delay / 10;  // 10% 抖动
        if (jitter > 0) {
            delay +=  QRandomGenerator::global()->bounded(2 * jitter) - jitter;
        }
    }

    // 确保最小延迟
    if (delay < m_policy.initialDelay) {
        delay = m_policy.initialDelay;
    }

    return delay;
}

bool OPCUAConnectionManager::reconnect()//从新连接
{
    QMutexLocker locker(&m_mutex);

    if (m_endpointUrl.isEmpty()) {//如果url为空那么返回
        qWarning() << "No endpoint URL specified for reconnection";
        return false;
    }

    if (m_state.load() == STATE_CONNECTED) {//如果状态位断开，呢么返回真
        logConnectionAttempt("Already connected, no need to reconnect");
        return true;
    }

    updateState(STATE_RECONNECTING);//安全的更新链接状态
    emit reconnecting(m_reconnectAttempt.load() + 1, calculateReconnectDelay());//发送重连信号

    bool success = performConnection();

    if (success) {
        m_reconnectAttempt = 0;
        recordConnectionSuccess();//记录连接
        updateState(STATE_CONNECTED);//更新连接状态为已连接，正常工作
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();//获取当前时间
        m_lastKeepaliveTime.store(currentTime);
        m_lastActivityTime.store(currentTime);
        m_keepaliveTimer->start();//链接成功后启动心跳检测
        emit connected();//发送链接ok信号
    } else {
        recordConnectionFailure();
        updateState(STATE_ERROR);

        m_reconnectAttempt++;
        scheduleReconnect();
    }

    return success;
}

bool OPCUAConnectionManager::performConnection()//连接执行函数
{
    clearError();  // 清除之前的错误

    if (!m_client) {
        recordError("Client not initialized");
        return false;
    }

    UA_StatusCode status = UA_STATUSCODE_BAD;
    QElapsedTimer timer;
    timer.start();

    try {
        // 获取客户端配置
        UA_ClientConfig *config = UA_Client_getConfig(m_client);
        if (!config) {
            recordError("Failed to get client config");
            return false;
        }

        // 清除之前的认证信息
        UA_ExtensionObject_clear(&config->userIdentityToken);

        // 设置认证信息
        if (!m_username.isEmpty()) {
            UA_UserNameIdentityToken *token = UA_UserNameIdentityToken_new();//创建用户名/密码认证令牌
            UA_UserNameIdentityToken_init(token);

            token->policyId = UA_STRING_ALLOC("username");
            token->userName = UA_STRING_ALLOC(m_username.toUtf8().constData());
            if (!m_password.isEmpty()) {
                token->password = UA_STRING_ALLOC(m_password.toUtf8().constData());
            }
            UA_ByteString_init(&token->encryptionAlgorithm);

            config->userIdentityToken.encoding = UA_EXTENSIONOBJECT_DECODED;
            config->userIdentityToken.content.decoded.type = &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN];
            config->userIdentityToken.content.decoded.data = token;
        }

        // 连接服务器
        status = UA_Client_connect(m_client, m_endpointUrl.toUtf8().constData());
        logConnectionAttempt(QString("Connection attempt completed in %1 ms with status:%2")
                                       .arg(timer.elapsed()).arg(UA_StatusCode_name(status)));
        if (status == UA_STATUSCODE_GOOD) {
            // 验证连接
            UA_DateTime serverTime;
            bool readSuccess = getServerTime(serverTime);//如果链接成功，读取服务器时间

            if (readSuccess) {//如果读取成功
                qInfo() << "Successfully connected to OPC UA server:" << m_endpointUrl;
                return true;
            } else {
                recordError("Connected but failed to read server time");
                UA_Client_disconnect(m_client);//连接失败，断开链接
                return false;
            }
        } else {
            recordError(QString("Failed to connect to OPC UA server:%1").arg(status));  // 记录具体的状态码错误
            logConnectionAttempt(QString("Failed to connect to OPC UA server:%1").arg(status));
            return false;
        }
    } catch (const std::exception &e) {//异常捕获
        recordError(QString("Exception during connection: %1").arg(e.what()));
        return false;
    } catch (...) {
        recordError("Unknown exception during connection");
        return false;
    }
}

void OPCUAConnectionManager::updateState(ConnectionState newState)//安全地更新连接状态
{
    ConnectionState oldState = m_state.load();
    if (oldState != newState) {
        m_state.store(newState);
        emit stateChanged(newState, oldState);
        QString messgae=QString("Connection state changed from %1 to %2").arg(oldState).arg(newState);
        logConnectionAttempt(messgae);
    }
}

void OPCUAConnectionManager::recordError(const QString &error)//记录错误描述
{
    QMutexLocker locker(&m_errorMutex);
    m_lastError = error;
    qWarning() << "OPCUA Error:" << error;
}

void OPCUAConnectionManager::clearError()//清楚错误
{
    QMutexLocker locker(&m_errorMutex);
    m_lastError.clear();
}

void OPCUAConnectionManager::recordConnectionFailure()//记录连接失败的统计信息
{
    m_stats.failedConnections++;
}

void OPCUAConnectionManager::recordConnectionSuccess()//记录连接成功的统计信息。
{
    m_stats.totalConnections++;
    m_stats.currentReconnectAttempt = 0;
    m_stats.lastConnectTime = QDateTime::currentDateTime();

    if (m_stats.firstConnectTime.isNull()) {
        m_stats.firstConnectTime = m_stats.lastConnectTime;
    }
}

void OPCUAConnectionManager::logConnectionAttempt(const QString &details)// 记录操作的日志
{
    QString message = QString("Connection %1: %2 :%3" )
    .arg(QDateTime::currentDateTime().toString())
        .arg(connectionStateName())
        .arg(m_endpointUrl);

    if (!details.isEmpty()) {
        message += " - " + details;
    }
     qDebug()<< message;
     logAttemptChanged(message);
}

bool OPCUAConnectionManager::getServerTime(UA_DateTime  &serverTime)
{
    if (!m_client) {
        return false;
    }
    //UA_NodeId serverStatusNode = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS);
    UA_NodeId currentTimeNode = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    UA_Variant value;
    UA_Variant_init(&value);

    UA_StatusCode status = UA_Client_readValueAttribute(m_client, currentTimeNode, &value);

    if (status == UA_STATUSCODE_GOOD && value.type == &UA_TYPES[UA_TYPES_DATETIME]) {
        serverTime = *(UA_DateTime*)value.data;
        UA_Variant_clear(&value);
        return true;
    }

    UA_Variant_clear(&value);
    return false;
}


} // namespace Industrial
// ==================== OPCUAConnectionManager 的信号 ====================


// ==================== OPCUAVariableManager 实现 ====================
namespace Industrial {

//操作顺序连接服务器 → 注册变量 → 读写数据
OPCUAVariableManager::OPCUAVariableManager(QObject *parent)
    : QObject(parent)
    , m_threadPool(nullptr)
    , m_maxThreadCount(4)
    , m_subscriptionMode(SUBSCRIPTION_MONITORED)
    , m_subscriptionId(0)
    , m_pollingInterval(1000)
    , m_requestIdCounter(0)
    , m_successfulReads(0)
    , m_failedReads(0)
    , m_successfulWrites(0)
    , m_failedWrites(0)
    , m_isInitialized(false)
{
    // 初始化订阅配置
    m_subscriptionConfig.publishingInterval = 1000.0;// 发布间隔(ms)服务器向客户端发送不变化数据的间隔时间，变化数据，数据变化发送，不是每1s推送一次
    m_subscriptionConfig.lifetimeCount = 60;//客户端允许服务器最多连续错过多少次心跳/数据更新后，就认为订阅已失效
    m_subscriptionConfig.maxKeepAliveCount = 10;//服务器在没有数据变化时，最多可以"沉默"多少次，就必须强制发送一次心跳
    m_subscriptionConfig.priority = 0;//订阅优先级 参数，用于控制订阅在服务器资源分配中的相对重要性。0-255

    // 初始化监控项配置
    m_monitoredItemConfig.samplingInterval = 100.0;//服务器检查变量值变化的频率（单位：毫秒）
    m_monitoredItemConfig.queueSize = 1;//服务器为每个监控项（每个变量）维护的一个数据队列的大小。保存最多10个数据变化事件（变化的值）
    m_monitoredItemConfig.discardOldest = true;//队列已满时，丢弃最旧的数据变化，
    m_monitoredItemConfig.clientHandle = generateClientHandle();//客户端自己生成一个唯一的ID给每个监控的变量分配一个唯一的"身份证号"，用于识别哪个变量发送了数据

    // 初始化连接管理器
    m_connectionManager = std::make_unique<OPCUAConnectionManager>();//智能指针,不用new和delete

    // 初始化线程池
    m_threadPool = new QThreadPool(this);//创建线程
    m_threadPool->setMaxThreadCount(m_maxThreadCount);//设置线程池中最多可以同时运行的线程数量

    //初始化定时器，当模式设置为轮训模式时定时读取所有注册变量
    m_pollingTimer = new QTimer(this);
    m_pollingTimer->setSingleShot(false);//设置定时器为周期性的=1单次的
    QObject::connect(m_pollingTimer, &QTimer::timeout, this, &OPCUAVariableManager::onPollingTimer);

    QObject::connect(m_connectionManager.get(), &OPCUAConnectionManager::stateChanged,
                     this, &OPCUAVariableManager::onConnectionStateChanged);//状态改变
    QObject::connect(m_connectionManager.get(), &OPCUAConnectionManager::connectionLost,
                     this, &OPCUAVariableManager::connectionLost);//连接丢失
    QObject::connect(m_connectionManager.get(), &OPCUAConnectionManager::reconnecting,
                     this, &OPCUAVariableManager::reconnecting);//重连
    QObject::connect(m_connectionManager.get(), &OPCUAConnectionManager::keepaliveReceived,
                     this, &OPCUAVariableManager::heartbeatReceived); //心跳信号
    QObject::connect(m_connectionManager.get(), &OPCUAConnectionManager::connected,
                     this, &OPCUAVariableManager::connected); //心跳信号
    m_processTimer=new QTimer(this);
    QObject::connect(m_processTimer,&QTimer::timeout,this,&OPCUAVariableManager::startProcessing);


    m_isInitialized = true;//初始化完成
    qDebug() << "OPCUAVariableManager initialized successfully";
}


OPCUAVariableManager::~OPCUAVariableManager()
{

    stopSubscription();
    m_pollingTimer->stop();

    // 断开连接
    disconnect();

    // 等待所有任务完成
    if (m_threadPool) {
        m_threadPool->waitForDone(3000);
    }

    // 清理同步等待数据
    {
        QMutexLocker locker(&m_syncWaitMutex);
        qDeleteAll(m_syncWaitMap);
        m_syncWaitMap.clear();
    }

    // 清理变量句柄
    {
        QWriteLocker locker(&m_variablesLock);
        m_variables.clear();
    }

    qDebug() << "OPCUAVariableManager destroyed";
}

// ==================== 连接管理 ====================
bool OPCUAVariableManager::connect(const QString &endpointUrl,
                                   const QString &username,
                                   const QString &password)//连接服务器
{
    QMutexLocker locker(&m_mutex);

    // 检查管理器是否已初始化
    if (!m_isInitialized) {
        recordError("Manager not initialized");
        return false;
    }

    // 检查连接管理器是否存在
    if (!m_connectionManager) {
        recordError("Connection manager is null");
        return false;
    }

    // 检查客户端是否有效
    if (!m_connectionManager->isClientValid()) {
        recordError("OPC UA client is not properly initialized");
        return false;
    }

    // 执行连接
    bool success = m_connectionManager->connectToserver(endpointUrl, username, password);

    return success;
}


void OPCUAVariableManager::disconnect()//断开链接
{
    if (!m_isInitialized) {
        return;
    }
    m_pollingTimer->stop();
    m_connectionManager->disconnect();
}

bool OPCUAVariableManager::reconnect()//返回OPCUAConnectionManager从新连接状态
{
    if (!m_isInitialized) {
        return false;
    }

    return m_connectionManager->reconnect();
}

ConnectionState OPCUAVariableManager::connectionState() const//返回OPCUAConnectionManager链接状态具体的枚举常亮
{
    if (!m_isInitialized) {
        return STATE_DISCONNECTED;
    }

    return m_connectionManager->state();
}

bool OPCUAVariableManager::isConnected() const//如果链接的，返回真
{
    if (!m_isInitialized) {
        return false;
    }

    return m_connectionManager->isConnected();
}

// ==================== 配置管理 ====================
void OPCUAVariableManager::setReconnectPolicy(const ReconnectPolicy &policy)//设置OPCUAConnectionManager重连策略
{
    if (m_isInitialized) {
        m_connectionManager->setReconnectPolicy(policy);
    }
}

ReconnectPolicy OPCUAVariableManager::reconnectPolicy() const//返回OPCUAConnectionManager重连策略设置
{
    if (!m_isInitialized) {
        return ReconnectPolicy();
    }

    return m_connectionManager->reconnectPolicy();
}

void OPCUAVariableManager::setRequestTimeout(int timeoutMs)//设置异步操作的超时时间,相对异步来说的
{
    QMutexLocker locker(&m_mutex);
    if (timeoutMs < 100) {
        timeoutMs = 100; // 最小100ms
    }
    m_requestTimeout = timeoutMs;
}

void OPCUAVariableManager::setRetryCount(int count)//设置失败操作的重试次数
{
    QMutexLocker locker(&m_mutex);
    if (count < 0) {
        count = 0; // 0表示不重试
    }
    if (count > 10) {
        count = 10; // 最大重试10次
    }
    m_retryCount = count;
}

void OPCUAVariableManager::setMaxThreadCount(int count)//设置最大线程数
{
    QMutexLocker locker(&m_mutex);

    if (count < 2) {
        count = 1;  // 最少1个线程
    }
    if (count > 10) {
        count = 20; // 最多20个线程（可根据需要调整）
    }

    m_maxThreadCount = count;

    if (m_threadPool) {
        m_threadPool->setMaxThreadCount(count);
    }
}

// ==================== 订阅配置 ====================
void OPCUAVariableManager::setSubscriptionConfig(const SubscriptionConfig &config)//设置阅订模式
{
    m_subscriptionConfig = config;
}

SubscriptionConfig OPCUAVariableManager::subscriptionConfig() const//读取阅订模式
{
    return m_subscriptionConfig;
}

void OPCUAVariableManager::setMonitoredItemConfig(const MonitoredItemConfig &config)//设定监控模式
{
    m_monitoredItemConfig = config;
}

MonitoredItemConfig OPCUAVariableManager::monitoredItemConfig() const//读取监控模式
{
    return m_monitoredItemConfig;
}

// ==================== 变量管理 ====================


bool OPCUAVariableManager::registerVariable(VariableDefinition* variable)
{
    // 1. 参数验证（你的代码正确）
    if (!variable) {
        recordError("Attempting to register null variable");
        return false;
    }

    QString tagName = variable->tagName();
    if (tagName.isEmpty()) {
        recordError("Variable tag name cannot be empty");
        return false;
    }

    if (variable->address().isEmpty()) {
        recordError("Variable tag address cannot be empty");
        return false;
    }

    QWriteLocker locker(&m_variablesLock);

    // 2. 检查是否已注册
    if (m_variables.contains(tagName)) {
        recordError(QString("Variable already registered: %1").arg(tagName));
        return false;
    }

    // 3. 创建变量句柄
    auto handle = std::make_shared<OPCUAVariableHandle>();

    // 4. 关键修改：直接解析到handle->nodeId创建变量解析
    if (!parseNodeId(variable->address(), handle->nodeId)) {
        // 修复调试信息
        qDebug() << "Failed to parse address for" << tagName
                 << ":" << variable->address();
        recordError(QString("Failed to parse NodeId: %1").arg(variable->address()));
        return false;
    }

    // 5. 验证解析结果（重要！）
    if (UA_NodeId_isNull(&handle->nodeId)) {
        recordError(QString("Parsed NodeId is null for: %1").arg(tagName));
        return false;
    }

    // 6. 设置句柄的其他属性
    handle->tagName = tagName;
    handle->variableDef = variable;

    // 7. 初始化状态信息
    handle->lastStatus.isConnected = m_connectionManager->isConnected();
    handle->lastStatus.quality = handle->lastStatus.isConnected ?
                                     QUALITY_GOOD : QUALITY_COMM_FAIL;

    // 8. 存储到容器
    m_variables.insert(tagName, handle);
    recordSuccess(QString("Registered variable: %1").arg(tagName));

    return true;
}

bool OPCUAVariableManager::registerVariables(const QList<VariableDefinition*> &variables)//批量注册多个变量，调用registerVarable方法
{
    bool allSuccess = true;

    for (VariableDefinition *var : variables) {
        if (!registerVariable(var)) {
            allSuccess = false;
        }
    }

    return allSuccess;
}

bool OPCUAVariableManager::unregisterVariable(const QString &tagName)//取消注册（删除）一个已注册的变量
{
    QWriteLocker locker(&m_variablesLock);

    if (!m_variables.contains(tagName)) {//先查询有没有这个变量
        recordError(QString("Variable not registered: %1").arg(tagName));
        return false;
    }

    // 获取句柄（不移除）
    auto it = m_variables.find(tagName);
    if (it != m_variables.end() && (*it)->isSubscribed) {
        deleteMonitoredItem(it->get());
    }

    int removedCount = m_variables.remove(tagName);  // ✅ 使用 remove()

    qDebug() << "Variable unregistered successfully:" << tagName;
    recordSuccess(QString("Unregistered variable: %1").arg(tagName));

    return removedCount > 0;
}

void OPCUAVariableManager::clearVariables()//清除所有已注册的变量
{
    QWriteLocker locker(&m_variablesLock);

    // 删除所有监控项 - 使用显式迭代器
    auto it = m_variables.constBegin();
    auto end = m_variables.constEnd();
    for (; it != end; ++it) {
        const auto &handle = it.value();
        if (handle && handle->isSubscribed) {
            deleteMonitoredItem(handle.get());
        }
    }
    m_variables.clear();

    qDebug() << "All variables cleared";
    recordSuccess("Cleared all variables");
}

bool OPCUAVariableManager::browseVariableNode(const QString &tagName)//异步查询已注册变量的OPC UA节点详细信息，验证节点是否存在并获取节点属性
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected, cannot browse node");
        return false;
    }

    // 异步执行浏览任务
    int requestId = generateRequestId();

    OPCUATask *task = new OPCUATask(OP_BROWSE, tagName, QVariant(), requestId, this);
    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, &OPCUAVariableManager::onTaskCompleted,
                     Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_BROWSE, tagName, requestId);
    addPendingRequest(request);

    return true;
}

bool OPCUAVariableManager::browseAllVariables()//批量浏览所有已注册变量的 OPC UA 节点信息
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected, cannot browse nodes");
        return false;
    }

    QReadLocker locker(&m_variablesLock);

    int successCount = 0;
    int failureCount = 0;
    \
        // 使用显式迭代器
        auto it = m_variables.constBegin();
    auto end = m_variables.constEnd();
    for (; it != end; ++it) {
        const auto &handle = it.value();
        if (browseVariableNode(handle->tagName)) {
            successCount++;
        } else {
            failureCount++;
        }

    }
    emit allVariablesBrowsed(successCount, failureCount);

    return (failureCount == 0);
}


// ==================== 异步同步读写操作，尽量用异步读写 ====================
int OPCUAVariableManager::readVariableAsync(const QString &tagName)//异步读取单个已注册的变量
{//
    // 验证连接
    if (!m_connectionManager->isConnected()) {
        int requestId = generateRequestId();//生成请求标识符，唯一的
        emit readCompleted(requestId, tagName, QVariant(), false,"Not connected to server");
        return requestId;
    }

    int requestId = generateRequestId();

    OPCUATask *task = new OPCUATask(OP_READ_SINGLE, tagName, QVariant(), requestId, this);//创建一个读取任务
    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, &OPCUAVariableManager::onTaskCompleted,
                     Qt::QueuedConnection);//当taskCompleted任务完成时调用onTaskCompleted任务处理完成函数

    m_threadPool->start(task);

    OperationRequest request(OP_READ_SINGLE, tagName, requestId);//生成请求ID，请求类型、变量名、请求ID
    addPendingRequest(request);//请求记录，写入QMap中

    return requestId;//返回请求ID
}

int OPCUAVariableManager::readAllVariablesAsync()//异步读取全部已注册的变量
{
    if (!m_connectionManager->isConnected()) {
        int requestId = generateRequestId();
        emit batchReadCompleted(requestId, QVariantMap(), false, "Not connected to server");
        return requestId;
    }

    int requestId = generateRequestId();

    // 获取所有标签名
    QStringList tagNames;
    {
        QReadLocker locker(&m_variablesLock);
        // tagNames.append(handle->tagName); 这行代码需要保留
        for (const auto &handle : m_variables) {
            tagNames.append(handle->tagName);
        }
    }

    // 创建任务时传递 QVariant 包装的 QStringList
    OPCUATask *task = new OPCUATask(OP_READ_BATCH, "",
                                    QVariant(tagNames),  // 改为 QVariant
                                    requestId, this);
    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, &OPCUAVariableManager::onTaskCompleted,
                     Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_READ_BATCH, "", requestId);
    request.data = tagNames;  // 可以直接存储 QStringList
    addPendingRequest(request);

    return requestId;
}

int OPCUAVariableManager::writeVariableAsync(const QString &tagName,
                                             const QVariant &value)//异步写入单个已注册的变量
{
    if (!m_connectionManager->isConnected()) {
        int requestId = generateRequestId();
        emit writeCompleted(requestId, tagName, false,
                            "Not connected to server");
        return requestId;
    }

    // 检查变量是否存在且可写
    {
        QReadLocker locker(&m_variablesLock);
        auto it = m_variables.find(tagName);
        if (it == m_variables.end() || !(*it)->variableDef ||
            !(*it)->variableDef->writable()) {
            int requestId = generateRequestId();
            emit writeCompleted(requestId, tagName, false,
                                "Variable not found or not writable");
            return requestId;
        }
    }

    int requestId = generateRequestId();

    OPCUATask *task = new OPCUATask(OP_WRITE_SINGLE, tagName, value,
                                    requestId, this);
    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, &OPCUAVariableManager::onTaskCompleted,
                     Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_WRITE_SINGLE, tagName, requestId);
    request.data = value;
    addPendingRequest(request);

    return requestId;
}

QVariant OPCUAVariableManager::readVariableSync(const QString &tagName,
                                              bool *ok,
                                              int timeoutMs)//同步读取单个已注册的变量,调用readVariableAsync异步，完成同步
{
    QElapsedTimer timer;
    timer.start();

    if (!m_connectionManager->isConnected()) {
        if (ok) *ok = false;
        recordError("Connection validation failed before read");
        return 0.0;
    }

    int requestId = readVariableAsync(tagName);

    // 等待完成
    QVariant result;
    QString error;
    bool success = waitForCompletion(requestId, timeoutMs, result, error);

    if (ok) {
        *ok = success;
    }

    if (!success && !error.isEmpty()) {
        recordError(QString("Read failed for %1: %2").arg(tagName).arg(error));

        // 如果是连接问题，尝试重连
        if (error.contains("Connection", Qt::CaseInsensitive) ||
            error.contains("timeout", Qt::CaseInsensitive)) {
            qDebug() << "Connection issue detected during read, attempting reconnection";
            QTimer::singleShot(0, this, &OPCUAVariableManager::forceReconnect);
        }
    }

    qDebug() << "Read operation completed in" << timer.elapsed() << "ms";
    return result;
}

QVariantMap OPCUAVariableManager::readAllVariablesSync()//同步读取单个已注册的变量,调用readVariableAsync异步，完成同步
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected to server");
        return QVariantMap();
    }

    // 调用异步版本
    int requestId = readAllVariablesAsync();

    // 等待完成
    QVariant result;
    QString error;
    bool success = waitForCompletion(requestId, 10000, result, error);

    if (success && result.type() == QVariant::Map) {
        return result.toMap();  // 直接返回 QVariantMap
    }

    if (!error.isEmpty()) {
        recordError(QString("Read all variables failed: %1").arg(error));
    }

    return QVariantMap();
}

bool OPCUAVariableManager::writeVariableSync(const QString &tagName,
                                             const QVariant &value,
                                             int timeoutMs)
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected to server");
        return false;
    }

    int requestId = writeVariableAsync(tagName, value);

    QVariant result;
    QString error;
    bool success = waitForCompletion(requestId, timeoutMs, result, error);

    if (!success && !error.isEmpty()) {
        recordError(QString("Write failed for %1: %2").arg(tagName).arg(error));
    }

    return success;
}



// ==================== 订阅管理 ====================

bool OPCUAVariableManager::startSubscription(SubscriptionMode mode)//启动数据订阅
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected, cannot start subscription");
        return false;
    }

    m_subscriptionMode = mode;

    if (mode == SUBSCRIPTION_POLLING) {//轮询模式
        // 轮询模式
        m_pollingTimer->start(m_pollingInterval);
        qInfo() << "Started polling subscription with interval" << m_pollingInterval << "ms";
        return true;
    }
    else if (mode == SUBSCRIPTION_MONITORED) {//监控模式，初始化时默认为监控模式了
        // 监控项模式
        if (createSubscription()) {
            qInfo() << "Created monitored subscription with ID:" << m_subscriptionId;

            // 为所有已注册变量创建监控项
            QWriteLocker locker(&m_variablesLock);
            for (const auto &handle : m_variables) {
                if (!handle->isSubscribed) {
                    createMonitoredItem(handle.get());
                }
            }
            m_processTimer->start(1000);
            return true;
        } else {
            qWarning() << "Failed to create monitored subscription";
            return false;
        }
    }

    return false;
}

void OPCUAVariableManager::stopSubscription()//停止阅订
{
    if (m_subscriptionMode == SUBSCRIPTION_POLLING) {
        m_pollingTimer->stop();//如果是轮训模式停止轮训定时器
    }
    else if (m_subscriptionMode == SUBSCRIPTION_MONITORED && m_subscriptionId > 0) {
        // 如果是监控模式，删除订阅
        deleteSubscription();
        m_subscriptionId = 0;

        // 更新所有句柄的订阅状态
        QWriteLocker locker(&m_variablesLock);
        for (const auto &handle : m_variables) {
            handle->isSubscribed = false;
        }
    }

    qInfo() << "Stopped subscription";
}

bool OPCUAVariableManager::isSubscribed() const// 检查当前是否启用了数据订阅功能
{
    if (m_subscriptionMode == SUBSCRIPTION_POLLING) {
        return m_pollingTimer->isActive();
    } else {
        return m_subscriptionId > 0;
    }
}

void OPCUAVariableManager::setPollingInterval(int intervalMs)//设置轮询订阅模式的时间间隔
{
    if (intervalMs < 100) {
        intervalMs = 100; // 最小100ms
    }

    m_pollingInterval = intervalMs;
    m_pollingTimer->setInterval(intervalMs);
}

int OPCUAVariableManager::pollingInterval() const//查询轮询订阅模式的时间间隔
{
    return m_pollingInterval;
}

// ==================== 查询方法 ====================

VariableDefinition* OPCUAVariableManager::getVariable(const QString &tagName) const//获取已注册变量的定义对象
{
    QReadLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return (*it)->variableDef;
    }

    return nullptr;
}

QList<VariableDefinition*> OPCUAVariableManager::getAllVariables() const//获取所有已注册变量的定义对象
{
    QList<VariableDefinition*> variables;

    QReadLocker locker(&m_variablesLock);
    for (const auto &handle : m_variables) {
        if (handle->variableDef) {
            variables.append(handle->variableDef);
        }
    }

    return variables;
}

QList<QString> OPCUAVariableManager::getRegisteredTagNames() const//获取所有已注册变量的标签名列表。
{
    QReadLocker locker(&m_variablesLock);
    return m_variables.keys();
}

NodeStatus OPCUAVariableManager::getVariableStatus(const QString &tagName) const//获取已注册变量的最新状态信息
{
    QReadLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return (*it)->lastStatus;
    }

    NodeStatus status;
    status.isConnected = false;
    status.quality = QUALITY_BAD;
    return status;
}

QVariant OPCUAVariableManager::getLastValue(const QString &tagName) const
{
    QReadLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return (*it)->lastValue;  // 返回 QVariant
    }

    return QVariant();  // 返回空 QVariant
}

OPCUAVariableHandle* OPCUAVariableManager::getVariableHandle(const QString &tagName) const//根据tagName查找句柄
{
    QReadLocker locker(&m_variablesLock);

    for (auto it = m_variables.begin(); it != m_variables.end(); ++it) {
        QString key = it.key();
        qDebug() << "  - Key:" << key
                 << "长度:" << key.length()
                 << "相等比较:" << (key == tagName)
                 << "相等运算符:" << (key == tagName)
                 << "Qt比较:" << (QString::compare(key, tagName, Qt::CaseSensitive) == 0);

        // 显示十六进制，检查隐藏字符
        qDebug() << "    Key十六进制:" << key.toUtf8().toHex();
        qDebug() << "    tagName十六进制:" << tagName.toUtf8().toHex();
    }

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        OPCUAVariableHandle* handle = it->get();
        if (handle->variableDef) {
        }
        return handle;
    } else {
        qWarning() << "OPCUAVariableManager: 未找到变量" << tagName;
        return nullptr;
    }
}

/*
OPCUAVariableHandle* OPCUAVariableManager::getVariableHandle(const QString &tagName) const// 获取已注册变量的内部句柄对象。
{
    QReadLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return it->get();
    }
    qWarning() << "OPCUAVariableManager: 未找到变量" << tagName;
    return nullptr;
}
*/


// ==================== 统计信息 ====================

SessionStatistics OPCUAVariableManager::connectionStatistics() const//获取连接和操作的统计信息
{
    SessionStatistics stats = m_connectionManager->statistics();

    {
        QMutexLocker locker(&m_statsMutex);
        stats.successfulReads = m_successfulReads.load();
        stats.failedReads = m_failedReads.load();
        stats.successfulWrites = m_successfulWrites.load();
        stats.failedWrites = m_failedWrites.load();
    }

    return stats;
}

int OPCUAVariableManager::pendingRequests() const//获取当前正在处理的异步请求数量。
{
    QMutexLocker locker(&m_requestsMutex);  // 需要 m_requestsMutex 是 mutable
    return m_pendingRequests.size();
}

int OPCUAVariableManager::activeThreads() const//获取当前活动的线程数量
{
    if (m_threadPool) {
        return m_threadPool->activeThreadCount();
    }
    return 0;
}

double OPCUAVariableManager::averageResponseTime() const//计算操作的平均响应时间。
{
    QMutexLocker locker(&m_statsMutex);
    if (m_responseTimes.isEmpty()) {
        return 0.0;
    }

    int sum = 0;
    for (int time : m_responseTimes) {
        sum += time;
    }

    return static_cast<double>(sum) / m_responseTimes.size();
}

void OPCUAVariableManager::resetStatistics()//重置所有统计信息。
{
    m_connectionManager->resetStatistics();

    {
        QMutexLocker locker(&m_statsMutex);
        m_successfulReads = 0;
        m_failedReads = 0;
        m_successfulWrites = 0;
        m_failedWrites = 0;
        m_responseTimes.clear();
    }

    {
        QMutexLocker locker(&m_errorMutex);
        m_errorLog.clear();
    }
}

// ==================== 服务器信息 ====================
QString OPCUAVariableManager::serverName() const//获取服务器名称，功能未实现
{
    if (!m_connectionManager->isConnected() || !m_connectionManager->client()) {
        return QString();
    }

    // 简化实现，实际应该从服务器读取名称
    return "OPC UA Server";
}

QString OPCUAVariableManager::serverEndPointUrl() const
{
    return m_connectionManager->endpointUrl();
}

QString OPCUAVariableManager::serverVersion() const
{
    if (!m_connectionManager->isConnected()) {
        return QString();
    }

    // 简化实现
    return "1.0.0";
}

UA_DateTime OPCUAVariableManager::serverTime() const//获取服务器时间
{
    if (!m_connectionManager->isConnected() || !m_connectionManager->client()) {
        return 0;
    }
    return  UA_DateTime_now();
}

// ==================== 诊断功能 ====================
bool OPCUAVariableManager::testConnection(int timeoutMs)//测试连接
{
    if (!m_connectionManager->isConnected()) {
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    try {
        // 使用读取服务器状态的方式测试连接
        UA_NodeId currentTimeNode = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
        UA_Variant value;
        UA_Variant_init(&value);

        UA_Client* client = m_connectionManager->client();
        if (!client) {
            return false;
        }

        UA_StatusCode status = UA_Client_readValueAttribute(client, currentTimeNode, &value);
        UA_Variant_clear(&value);

        bool success = (status == UA_STATUSCODE_GOOD && timer.elapsed() < timeoutMs);

        if (success) {
            qDebug() << "Connection test passed in" << timer.elapsed() << "ms";
        } else {
            qWarning() << "Connection test failed:" << UA_StatusCode_name(status);
            recordError(QString("Connection test failed: %1").arg(UA_StatusCode_name(status)));
        }

        return success;
    } catch (const std::exception &e) {
        recordError(QString("Connection test exception: %1").arg(e.what()));
        return false;
    }
}

QString OPCUAVariableManager::lastError() const//最后的错误
{
    QMutexLocker locker(&m_errorMutex);
    if (m_errorLog.isEmpty()) {
        return QString();
    }
    return m_errorLog.last().second;
}

QList<QString> OPCUAVariableManager::recentErrors(int count) const
{
    QList<QString> errors;
    QMutexLocker locker(&m_errorMutex);

    int start = qMax(0, m_errorLog.size() - count);
    for (int i = start; i < m_errorLog.size(); ++i) {
        errors.append(QString("[%1] %2")
                          .arg(m_errorLog[i].first.toString("hh:mm:ss"))
                          .arg(m_errorLog[i].second));
    }

    return errors;
}

// ==================== 批量操作 ====================

bool OPCUAVariableManager::batchRead(const QList<QString> &tagNames,
                                     QVariantMap &results,
                                     int timeoutMs)//批量读取
{
    if (!m_connectionManager->isConnected()) {
        return false;
    }

    int requestId = generateRequestId();

    // 直接传递 QVariant 列表
    OPCUATask *task = new OPCUATask(OP_READ_BATCH, "", QVariant(tagNames),
                                    requestId, this);

    SyncWaitData *waitData = new SyncWaitData;
    {
        QMutexLocker locker(&m_syncWaitMutex);
        m_syncWaitMap.insert(requestId, waitData);
    }

    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, [this, requestId, &results, waitData](
                         int id, bool success,
                         const QVariant &result,
                         const QString &error) {
                         if (id == requestId && success && result.type() == QVariant::Map) {
                             results = result.toMap();
                         }

                         QMutexLocker locker(&m_syncWaitMutex);
                         if (m_syncWaitMap.contains(requestId)) {
                             waitData->completed = true;
                             waitData->result = result;
                             waitData->error = error;
                             waitData->condition.wakeAll();
                         }
                     }, Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_READ_BATCH, "", requestId);
    request.data = tagNames;
    addPendingRequest(request);

    // 等待完成
    bool success = false;
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker(&m_syncWaitMutex);
        while (!waitData->completed && timer.elapsed() < timeoutMs) {
            waitData->condition.wait(&m_syncWaitMutex, 100);
        }

        if (waitData->completed) {
            success = waitData->result.toBool();
        }

        m_syncWaitMap.remove(requestId);
    }

    delete waitData;
    return success;
}

bool OPCUAVariableManager::batchWrite(const QVariantMap &values, int timeoutMs)//批量写入
{
    if (!m_connectionManager->isConnected()) {
        recordError("Not connected to server");
        return false;
    }

    if (values.isEmpty()) {
        qDebug() << "Empty write operation, considered successful";
        return true;  // 空操作视为成功
    }

    int requestId = generateRequestId();

    // 直接传递 QVariantMap，不再需要转换
    OPCUATask *task = new OPCUATask(OP_WRITE_BATCH, "",
                                    QVariant(values),  // 包装为 QVariant
                                    requestId, this);

    SyncWaitData *waitData = new SyncWaitData;
    {
        QMutexLocker locker(&m_syncWaitMutex);
        m_syncWaitMap.insert(requestId, waitData);
    }

    // Lambda 处理完成信号
    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, [this, requestId, waitData](int id, bool success,
                                                 const QVariant &result,
                                                 const QString &error) {
                         QMutexLocker locker(&m_syncWaitMutex);
                         if (m_syncWaitMap.contains(requestId)) {
                             waitData->completed = true;
                             waitData->result = QVariant(success);  // 存储成功状态
                             waitData->error = error;
                             waitData->condition.wakeAll();
                         }
                     }, Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_WRITE_BATCH, "", requestId);
    request.data = values;  // 直接存储 QVariantMap
    addPendingRequest(request);

    // 等待完成
    bool success = false;
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker(&m_syncWaitMutex);
        while (!waitData->completed && timer.elapsed() < timeoutMs) {
            waitData->condition.wait(&m_syncWaitMutex, 100);
        }

        if (waitData->completed) {
            success = waitData->result.toBool();
            if (!success && waitData->error.isEmpty()) {
                waitData->error = "Batch write operation failed";
            }
        } else {
            // 超时处理
            waitData->error = "Operation timeout";
            removePendingRequest(requestId);
            qWarning() << "Batch write timeout after" << timeoutMs << "ms";
        }

        m_syncWaitMap.remove(requestId);
    }

    delete waitData;

    if (!success && !waitData->error.isEmpty()) {
        recordError(QString("Batch write failed: %1").arg(waitData->error));
    }

    return success;
}



/*
bool OPCUAVariableManager::batchRead(const QList<QString> &tagNames, QMap<QString, double> &results,int timeoutMs)//读取多个变量不查询注册表，直接尝试解析每个标签名为节点地址
{
    if (!m_connectionManager->isConnected()) {
        return false;
    }

    int requestId = generateRequestId();

    // 直接传递 QVariant 列表
    OPCUATask *task = new OPCUATask(OP_READ_BATCH, "", QVariant(tagNames),requestId, this);


    SyncWaitData *waitData = new SyncWaitData;
    {
        QMutexLocker locker(&m_syncWaitMutex);
        m_syncWaitMap.insert(requestId, waitData);
    }

    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, [this, requestId, &results, waitData](int id, bool success,
                                                           const QVariant &result,
                                                           const QString &error) {
                         // 使用辅助函数转换
                         if (id == requestId && success && result.type() == QVariant::Map) {
                             results = result.toMap());
                         }

                         QMutexLocker locker(&m_syncWaitMutex);
                         if (m_syncWaitMap.contains(requestId)) {
                             waitData->completed = true;
                             waitData->result = result;
                             waitData->error = error;
                             waitData->condition.wakeAll();
                         }
                     }, Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_READ_BATCH, "", requestId);
    request.data = tagNames;
    addPendingRequest(request);

    // 等待完成
    bool success = false;
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker(&m_syncWaitMutex);
        while (!waitData->completed && timer.elapsed() < timeoutMs) {
            waitData->condition.wait(&m_syncWaitMutex, 100);
        }

        if (waitData->completed) {
            success = waitData->result.toBool();
        }

        m_syncWaitMap.remove(requestId);
    }

    delete waitData;
    return success;
}
*/

/*
bool OPCUAVariableManager::batchWrite(const QVariantMap &values, int timeoutMs)//写入多个变量不查询注册表，直接尝试解析每个标签名为节点地址
{
    if (!m_connectionManager->isConnected()) {
        return false;
    }

    int requestId = generateRequestId();

    // 使用辅助函数转换
    QVariantMap variantMap = doubleMapToVariantMap(values);

    OPCUATask *task = new OPCUATask(OP_WRITE_BATCH, "", variantMap, requestId, this);

    SyncWaitData *waitData = new SyncWaitData;
    {
        QMutexLocker locker(&m_syncWaitMutex);
        m_syncWaitMap.insert(requestId, waitData);
    }

    QObject::connect(task, &OPCUATask::taskCompleted,
                     this, [this, requestId, waitData](int id, bool success,
                                                 const QVariant &result,
                                                 const QString &error) {
                         QMutexLocker locker(&m_syncWaitMutex);
                         if (m_syncWaitMap.contains(requestId)) {
                             waitData->completed = true;
                             waitData->result = result;
                             waitData->error = error;
                             waitData->condition.wakeAll();
                         }
                     }, Qt::QueuedConnection);

    m_threadPool->start(task);

    OperationRequest request(OP_WRITE_BATCH, "", requestId);
    request.data = variantMap;  // 使用转换后的 QVariantMap
    addPendingRequest(request);

    // 等待完成
    bool success = false;
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker(&m_syncWaitMutex);
        while (!waitData->completed && timer.elapsed() < timeoutMs) {
            waitData->condition.wait(&m_syncWaitMutex, 100);
        }

        if (waitData->completed) {
            success = waitData->result.toBool();
        }

        m_syncWaitMap.remove(requestId);
    }

    delete waitData;
    return success;
}
*/





// ==================== 刷新操作 主动获取变量的最新值 ====================
void OPCUAVariableManager::refreshAllVariablesAsync()//异步刷新所有变量
{
    readAllVariablesAsync();
}

void OPCUAVariableManager::refreshVariableAsync(const QString &tagName)
{
    readVariableAsync(tagName);
}

void OPCUAVariableManager::refreshAllVariablesSync()//同步刷新单个变量
{
    readAllVariablesSync();
}

void OPCUAVariableManager::refreshVariableSync(const QString &tagName)//同步刷新单个变量
{
    readVariableSync(tagName);
}

 // ==================== 连接操作 ====================
void OPCUAVariableManager::forceReconnect()//强制立即重连的功能
{
    qDebug() << "Forcing reconnection...";
    // 先断开
    disconnect();
    // 延迟后重连
    QTimer::singleShot(1000, this, &OPCUAVariableManager::onInternalReconnect);
}

void OPCUAVariableManager::testConnectionAsync()
{
    QTimer::singleShot(0, this, [this]() {
        bool success = testConnection(3000);
        qDebug() << "Connection test result:" << (success ? "Success" : "Failed");
    });
}


 // ==================== 订阅操作 ====================
void OPCUAVariableManager::restartSubscription()//重启阅订
{
    stopSubscription();
    startSubscription(m_subscriptionMode);
}

 // ==================== 工具操作 ====================

void OPCUAVariableManager::clearErrorLog()
{
    QMutexLocker locker(&m_errorMutex);
    m_errorLog.clear();
    qDebug() << "Error log cleared";
}

void OPCUAVariableManager::dumpStatusToLog()
{
    qDebug() << "=== OPCUAVariableManager Status ===";
    qDebug() << "Connection state:" << m_connectionManager->connectionStateName();
    qDebug() << "Connected:" << isConnected();
    qDebug() << "Registered variables:" << getRegisteredTagNames().size();
    qDebug() << "Subscription mode:" << (m_subscriptionMode == SUBSCRIPTION_POLLING ? "Polling" : "Monitored");
    qDebug() << "Subscription active:" << isSubscribed();
    qDebug() << "Pending requests:" << pendingRequests();
    qDebug() << "Active threads:" << activeThreads();
    qDebug() << "================================";
}

void OPCUAVariableManager::onSubscriptionDeleted(UA_UInt32 subId)
{
    // 只有监控模式才需要处理订阅删除
    if (m_subscriptionMode != SUBSCRIPTION_MONITORED) {
        qDebug() << "Ignoring subscription delete in polling mode";
        return;
    }

    qWarning() << "Subscription" << subId << "has been deleted by server";

    // 清理状态
    m_subscriptionId = 0;

    QWriteLocker locker(&m_variablesLock);
    for (auto &handle : m_variables) {
        handle->isSubscribed = false;
        handle->monitoredItemId = 0;
    }

    recordError(QString("Subscription %1 was deleted").arg(subId));

    // 尝试重新订阅
    QTimer::singleShot(2000, this, [this]() {
        if (m_connectionManager->isConnected()) {
            createSubscription();
        }
    });
}

void OPCUAVariableManager::startProcessing()
{
 UA_StatusCode status = UA_Client_run_iterate(m_connectionManager->client(), 0);  // 10ms超时
}

void OPCUAVariableManager::stopProcessing()
{
    if (m_processTimer) {
        m_processTimer->stop();
        qDebug() << "OPC UA客户端处理已停止";
    }
}

// ==================== 定时器槽 ====================
void OPCUAVariableManager::onPollingTimer()//询定时器的回调函数，用于实现轮询模式的订阅
{
    if (!m_connectionManager->isConnected() ||
        m_subscriptionMode != SUBSCRIPTION_POLLING) {
        return;
    }

    // 异步读取所有变量
    refreshAllVariablesAsync();
}


// ==================== 连接管理槽 ====================
void OPCUAVariableManager::onConnectionStateChanged(ConnectionState newState,
                                                    ConnectionState oldState)
{
    Q_UNUSED(oldState);

    emit connectionStateChanged(newState);

    switch (newState) {
    case STATE_CONNECTED:
        qDebug() << "OPC UA connection established";
        emit connected();

        // 启动轮询（如果是轮询模式）
        if (m_subscriptionMode == SUBSCRIPTION_POLLING) {
            m_pollingTimer->start(m_pollingInterval);
        }

        // 更新变量状态
        {
            QWriteLocker locker(&m_variablesLock);
            auto it = m_variables.constBegin();
            auto end = m_variables.constEnd();
            for (; it != end; ++it) {
                const auto &handle = it.value();
                if (handle) {
                    handle->lastStatus.isConnected = true;
                    handle->lastStatus.quality = QUALITY_GOOD;
                }
            }
        }

        // 通知连接恢复
        emit connectionRestored();
        break;

    case STATE_DISCONNECTED:
        qDebug() << "OPC UA connection disconnected";
        emit disconnected();

        // 停止轮询
        m_pollingTimer->stop();

        // 更新变量状态
        {
            QWriteLocker locker(&m_variablesLock);
            auto it = m_variables.constBegin();
            auto end = m_variables.constEnd();
            for (; it != end; ++it) {
                const auto &handle = it.value();
                if (handle) {
                    handle->lastStatus.isConnected = false;
                    handle->lastStatus.quality = QUALITY_BAD;
                }
            }
        }
        break;

    case STATE_RECONNECTING:
        qDebug() << "OPC UA reconnecting...";

        // 停止轮询，等待重连
        m_pollingTimer->stop();
        break;

    case STATE_ERROR:
        qWarning() << "OPC UA connection error";
        stopSubscription();
        // 停止轮询
        m_pollingTimer->stop();
        break;

    default:
        break;
    }
}



// ==================== 任务完成槽 ====================

//-------------------------------线程执行完，执行这个函数---------------------------------
void OPCUAVariableManager::onTaskCompleted(int requestId, bool success,
                                           const QVariant &result,
                                           const QString &error)
{
    // 查找对应的请求
    OperationRequest request;
    {
        QMutexLocker locker(&m_requestsMutex);
        request = m_pendingRequests.value(requestId);
        m_pendingRequests.remove(requestId);//完成后移除请求ID
    }

    if (request.requestId == 0) {
        qWarning() << "Received task completion signal for unknown request ID:" << requestId;
        return;
    }

    // 根据操作类型发出信号
    switch (request.type) {
    case OP_READ_SINGLE:
        emit readCompleted(requestId, request.tagName,
                           result, success, error);  // result 是 QVariant
        break;

    case OP_WRITE_SINGLE:
        emit writeCompleted(requestId, request.tagName, success, error);
        break;

    case OP_READ_BATCH:
        if (result.type() == QVariant::Map) {
            emit batchReadCompleted(requestId, result.toMap(), success, error);
        } else {
            emit batchReadCompleted(requestId, QVariantMap(), false,
                                    "Invalid result format");
        }
        break;

    case OP_WRITE_BATCH:
        emit batchWriteCompleted(requestId, success, error);
        break;

    case OP_BROWSE:
        emit variableNodeBrowsed(request.tagName, success, error);
        break;
    }

    // 通知等待的同步操作
    {
        QMutexLocker locker(&m_syncWaitMutex);
        if (m_syncWaitMap.contains(requestId)) {
            SyncWaitData *waitData = m_syncWaitMap.value(requestId);
            waitData->completed = true;
            waitData->result = result;
            waitData->error = error;
            waitData->condition.wakeAll();
        }
    }

    // 记录错误
    if (!success && !error.isEmpty()) {
        recordError(QString("Operation failed (request ID:%1): %2").arg(requestId).arg(error));
    }
}



// ==================== 内部槽 ====================
void OPCUAVariableManager::onInternalReconnect()//强制重连的内部回调函数
{
    reconnect();
}


//------------------------open62541回调函数及处理-----------------------------------





//========================工业现场优化版本折中板保证同一变量在同一线程= 固定线程数量============

void OPCUAVariableManager::dataChangeNotificationCallback(
    UA_Client* client, UA_UInt32 subId, void* subContext,
    UA_UInt32 monId, void* monContext, UA_DataValue* value)
{
    // ========== 简单统计（放在最前面）==========
    static std::atomic<int> totalCount{0};
    static QElapsedTimer statTimer;

    // 线程安全的计数
    int currentCount = totalCount.fetch_add(1, std::memory_order_relaxed);
    if (currentCount == 0) {
        statTimer.start();
    }

    if (statTimer.elapsed() >= 5000) {
        int count = totalCount.exchange(0);  // 获取并清零

        qDebug() << "=== OPC UA 回调统计 ===";
        qDebug() << "5秒内回调次数:" << count;
        qDebug() << "平均频率:" << (count / 5.0) << "Hz";

        statTimer.restart();
    }

    // 1. 快速参数检查（工业现场要求快速响应）
    if (!value || value->status != UA_STATUSCODE_GOOD) {
        return;  // 静默失败
    }

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!manager || !handle) return;

    // 2. 数据拷贝（必须深拷贝）
    UA_DataValue* valueCopy = UA_DataValue_new();
    UA_DataValue_init(valueCopy);
    UA_DataValue_copy(value, valueCopy);

    // 3. 优化线程池配置（结合两者优点）
    static QVector<QThreadPool*> threadPools;
    static std::once_flag poolInitFlag;

    std::call_once(poolInitFlag, []() {
        // 工业现场推荐配置
        int coreCount = QThread::idealThreadCount();
        int poolCount = qMax(2, coreCount - 2);  // 保留2个核心给系统

        threadPools.resize(poolCount);
        for (int i = 0; i < poolCount; i++) {
            QThreadPool* pool = new QThreadPool();
            pool->setMaxThreadCount(1);  // 关键：每个池只有1个线程
            pool->setExpiryTimeout(30000);
            pool->setStackSize(128 * 1024);  // 工业级栈大小
            threadPools[i] = pool;
        }

        qDebug() << "创建" << poolCount << "个专用线程池，每个池1个线程";
    });

    // 4. 按变量分组处理（保证同一变量顺序）
    QString tagName = QString(handle->tagName);
    uint poolIndex = qHash(tagName) % threadPools.size();

    // 5. 提交到专用线程池
    QtConcurrent::run(threadPools[poolIndex],
                      [manager, handle, valueCopy, tagName]() {
                          // 关键：异常安全的数据处理
                          try {
                              if (manager && handle && valueCopy) {
                                  manager->updateVariableFromCallback(handle, valueCopy);
                              }
                          } catch (...) {
                              qWarning() << "处理变量" << tagName << "时发生异常";
                          }

                          // 确保资源释放
                          UA_DataValue_delete(valueCopy);
                      });
}



// ==========================工业现场优化版本 动态配置======================

/*
void OPCUAVariableManager::dataChangeNotificationCallback(
    UA_Client* client, UA_UInt32 subId, void* subContext,
    UA_UInt32 monId, void* monContext, UA_DataValue* value)
{
    // 1. 性能监控
    static std::atomic<int> totalCount{0};
    static QElapsedTimer statTimer;
    if (totalCount++ == 0) statTimer.start();

    if (statTimer.elapsed() >= 5000) {
        qDebug() << "=== OPC UA 统计 ===";
        qDebug() << "5秒内回调次数:" << totalCount.load();
        qDebug() << "平均频率:" << (totalCount.load() / 5.0) << "Hz";
        qDebug() << "线程池活跃线程:" << QThreadPool::globalInstance()->activeThreadCount();
        qDebug() << "线程池最大线程:" << QThreadPool::globalInstance()->maxThreadCount();
        totalCount = 0;
        statTimer.restart();
    }

    // 2. 快速参数检查（工业现场要求快速响应）
    if (!value || value->status != UA_STATUSCODE_GOOD) {
        return;  // 静默失败，不记录日志避免影响性能
    }

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!manager || !handle) return;

    // 3. 数据拷贝（必须深拷贝，因为回调在OPC UA线程）
    UA_DataValue* valueCopy = UA_DataValue_new();
    UA_DataValue_init(valueCopy);
    UA_DataValue_copy(value, valueCopy);

    // 4. 优化线程池配置
    static std::once_flag poolInitFlag;
    std::call_once(poolInitFlag, []() {
        // 工业现场推荐配置
        QThreadPool* pool = QThreadPool::globalInstance();
        int coreCount = QThread::idealThreadCount();

        // 根据工业场景调整
        if (coreCount >= 4) {
            // 多核系统：保留2个核心给系统和其他任务
            pool->setMaxThreadCount(qMax(2, coreCount - 2));
        } else {
            // 少核系统：使用一半核心
            pool->setMaxThreadCount(qMax(1, coreCount / 2));
        }

        pool->setExpiryTimeout(30000);  // 30秒空闲后回收线程
        pool->setStackSize(128 * 1024); // 128KB栈，适合工业数据处理
    });

    // 5. 按变量分组处理（保证同一变量顺序）
    QString tagName = QString(handle->tagName);
    uint groupId = qHash(tagName) % QThreadPool::globalInstance()->maxThreadCount();

    // 6. 提交到线程池
    QtConcurrent::run(QThreadPool::globalInstance(),
                      [manager, handle, valueCopy, groupId, tagName]() {
                          Q_UNUSED(groupId);  // 分组用于负载均衡

                          // 记录处理线程（调试用）
                          // qDebug() << "处理变量" << tagName << "在线程" << QThread::currentThreadId();

                          // 关键：异常安全的数据处理
                          try {
                              if (manager && handle && valueCopy) {
                                  manager->updateVariableFromCallback(handle, valueCopy);
                              }
                          } catch (...) {
                              qWarning() << "处理变量" << tagName << "时发生异常";
                          }

                          // 确保资源释放
                          UA_DataValue_delete(valueCopy);
                      });
}
*/


//最初版在主线程中运行，可以保留

/*
void OPCUAVariableManager::dataChangeNotificationCallback(
    UA_Client* client, UA_UInt32 subId, void* subContext,
    UA_UInt32 monId, void* monContext, UA_DataValue* value)//阅订回调，当数据变化是调用
{
    qDebug() << "\n=== 收到数据变化通知 ===";
    qDebug() << "订阅ID:" << subId;
    qDebug() << "监控项ID:" << monId;
    qDebug() << "数据值指针:" << value;

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    if (!manager) {
        qDebug() << "错误: manager为空";
        return;
    }

    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!handle) {
        qDebug() << "错误: handle为空";
        return;
    }

    qDebug() << "变量名称:" << handle->tagName;
    qDebug() << "数据状态:" << (value ? UA_StatusCode_name(value->status) : "NULL");

    // 关键修复：复制 DataValue 而不是传递指针
    if (value && value->status == UA_STATUSCODE_GOOD) {
        // 创建数据的深拷贝
        UA_DataValue* valueCopy = UA_DataValue_new();
        UA_DataValue_init(valueCopy);

        // 复制整个 DataValue
        UA_DataValue_copy(value, valueCopy);

        qDebug() << "复制DataValue完成，新指针:" << valueCopy;

        // 重要：使用 Q_ARG 传递指针时，需要确保内存不被释放
        // 使用 lambda 捕获拷贝，并在完成后清理
        QMetaObject::invokeMethod(manager, [manager, handle, valueCopy]() {
            qDebug() << "在事件循环中处理复制的数据";

            // 处理数据
            if (manager && handle && valueCopy) {
                manager->updateVariableFromCallback(handle, valueCopy);
            }

            //  清理复制的数据
            if (valueCopy) {
                UA_DataValue_delete(valueCopy);
            }
        }, Qt::QueuedConnection);

    } else {
        qDebug() << "状态不佳或不处理";
    }
}

*/

//------------------------------在工作线程中解析--------------------------------

/*
void OPCUAVariableManager::dataChangeNotificationCallback(
    UA_Client* client, UA_UInt32 subId, void* subContext,
    UA_UInt32 monId, void* monContext, UA_DataValue* value)
{
   qDebug() << "[回调] 时间:" << QTime::currentTime().toString("hh:mm:ss.zzz");

   // 统计信息
   static int totalCount = 0;
   static QElapsedTimer statTimer;
   if (totalCount++ == 0) statTimer.start();

   if (statTimer.elapsed() >= 5000) {  // 每5秒
       qDebug() << "=== 统计 ===";
       qDebug() << "5秒内回调次数:" << totalCount;
       qDebug() << "平均频率:" << (totalCount / 5.0) << "Hz";
       qDebug() << "线程池活跃线程:" << QThreadPool::globalInstance()->activeThreadCount();
       totalCount = 0;
       statTimer.restart();
   }

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    if (!manager) {
        qDebug() << "错误: manager为空";
        return;
    }

    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!handle) {
        qDebug() << "错误: handle为空";
        return;
    }

   // qDebug() << "变量名称:" << handle->tagName;
   // qDebug() << "数据状态:" << (value ? UA_StatusCode_name(value->status) : "NULL");

    // 关键修复：复制 DataValue 而不是传递指针
    if (value && value->status == UA_STATUSCODE_GOOD) {
        // 创建数据的深拷贝
        UA_DataValue* valueCopy = UA_DataValue_new();
        UA_DataValue_init(valueCopy);
        UA_DataValue_copy(value, valueCopy);

       // qDebug() << "复制DataValue完成，新指针:" << valueCopy;
        qDebug()<<"dataChangeNotificationCallback函数线程号"<<QThread::currentThreadId();
        // 直接在工作线程中执行，移除QMetaObject::invokeMethod
        QtConcurrent::run([manager, handle, valueCopy]() {
            if (manager && handle && valueCopy) {
                manager->updateVariableFromCallback(handle, valueCopy); }
            // 清理复制的数据
            UA_DataValue_delete(valueCopy);
        });

    } else {
        qDebug() << "状态不佳或不处理";
    }
}
*/

//------------------------------版本跟跌--------------------------------

/*
void OPCUAVariableManager::dataChangeNotificationCallback(    UA_Client* client, UA_UInt32 subId, void* subContext,
                                                          UA_UInt32 monId, void* monContext, UA_DataValue* value)
{
    // 使用原子操作避免竞争
    static std::atomic<int> totalCount{0};
    static QElapsedTimer statTimer;

    // 线程安全的统计开始
    int currentCount = totalCount.fetch_add(1, std::memory_order_relaxed);
    if (currentCount == 0) {
        statTimer.start();
    }

    // 检查时间
    if (statTimer.elapsed() >= 5000) {
        int count = totalCount.load(std::memory_order_relaxed);
        QThreadPool* pool = QThreadPool::globalInstance();

        // 使用可用的方法
        int activeThreads = pool->activeThreadCount();
        int maxThreads = pool->maxThreadCount();

        qDebug() << "=== OPC UA 统计 ===";
        qDebug() << "5秒内回调次数:" << count;
        qDebug() << "平均频率:" << (count / 5.0) << "Hz";
        qDebug() << "线程池活跃线程:" << activeThreads;
        qDebug() << "线程池最大线程:" << maxThreads;

        // 调试信息
        if (activeThreads == 0 && count > 100) {
            qDebug() << "注意: 高频率回调但活跃线程显示为0，可能是:";
            qDebug() << "  1. 任务执行太快，线程立即结束";
            qDebug() << "  2. 统计时机正好在任务间隙";
            qDebug() << "  3. Qt线程池的工作方式特性";
        }

        totalCount = 0;
        statTimer.restart();
    }

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    if (!manager) {
        qDebug() << "错误: manager为空";
        return;
    }

    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!handle) {
        qDebug() << "错误: handle为空";
        return;
    }

    // qDebug() << "变量名称:" << handle->tagName;
    // qDebug() << "数据状态:" << (value ? UA_StatusCode_name(value->status) : "NULL");

    // 关键修复：复制 DataValue 而不是传递指针
    if (value && value->status == UA_STATUSCODE_GOOD) {
        // 创建数据的深拷贝
        UA_DataValue* valueCopy = UA_DataValue_new();
        UA_DataValue_init(valueCopy);
        UA_DataValue_copy(value, valueCopy);

        // qDebug() << "复制DataValue完成，新指针:" << valueCopy;
        qDebug()<<"dataChangeNotificationCallback函数线程号"<<QThread::currentThreadId();
        // 直接在工作线程中执行，移除QMetaObject::invokeMethod
        QtConcurrent::run([manager, handle, valueCopy]() {
            if (manager && handle && valueCopy) {
                manager->updateVariableFromCallback(handle, valueCopy); }
            // 清理复制的数据
            UA_DataValue_delete(valueCopy);
        });

    } else {
        qDebug() << "状态不佳或不处理";
    }
}
*/

//======================================可以多核==========================================

/*
void OPCUAVariableManager::dataChangeNotificationCallback(    UA_Client* client, UA_UInt32 subId, void* subContext,
                                                          UA_UInt32 monId, void* monContext, UA_DataValue* value)
{
    qDebug() << "[回调] 时间:" << QTime::currentTime().toString("hh:mm:ss.zzz");

    // 统计信息
    static int totalCount = 0;
    static QElapsedTimer statTimer;
    if (totalCount++ == 0) statTimer.start();

    if (statTimer.elapsed() >= 5000) {  // 每5秒
        qDebug() << "=== 统计 ===";
        qDebug() << "5秒内回调次数:" << totalCount;
        qDebug() << "平均频率:" << (totalCount / 5.0) << "Hz";
        qDebug() << "线程池活跃线程:" << QThreadPool::globalInstance()->activeThreadCount();
        totalCount = 0;
        statTimer.restart();
    }

    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    if (!manager) {
        qDebug() << "错误: manager为空";
        return;
    }

    OPCUAVariableHandle* handle = static_cast<OPCUAVariableHandle*>(monContext);
    if (!handle) {
        qDebug() << "错误: handle为空";
        return;
    }

    // 快速复制数据
    UA_DataValue* valueCopy = UA_DataValue_new();
    UA_DataValue_init(valueCopy);
    UA_DataValue_copy(value, valueCopy);

    // 使用QtConcurrent的特定线程池
    static QVector<QThreadPool*> threadPools;
    static std::once_flag initFlag;

    std::call_once(initFlag, [&]() {
        // 为每个CPU核心创建线程池
        int coreCount = QThread::idealThreadCount();
        for (int i = 0; i < coreCount; ++i) {
            QThreadPool* pool = new QThreadPool();
            pool->setMaxThreadCount(1);  // 每个池1个线程
            pool->setExpiryTimeout(-1);  // 线程常驻
            threadPools.append(pool);
        }
    });

    // 根据变量名哈希选择线程池
    QString tagName = QString(handle->tagName);
    int poolIndex = qHash(tagName) % threadPools.size();

    // 提交到特定线程池
    QtConcurrent::run(threadPools[poolIndex],
                      [manager, handle, valueCopy]() {
                          if (manager && handle && valueCopy) {
                              manager->updateVariableFromCallback(handle, valueCopy);
                          }
                          UA_DataValue_delete(valueCopy);
                      });
}

*/



void OPCUAVariableManager::updateVariableFromCallback(OPCUAVariableHandle* handle,
                                                      UA_DataValue* value)
{
   // qDebug() << "\n=== 处理回调数据 (使用复制数据) ===";
   // qDebug() << "变量:" << (handle ? handle->tagName : "NULL");
   // qDebug() << "DataValue指针:" << value;
    //qDebug()<<"updateVariableFromCallback函数线程号"<<QThread::currentThreadId();
    if (!handle || !value || !handle->variableDef) {
        qDebug() << "错误: 参数无效";
        return;
    }

    // 直接检查状态
    if (value->status != UA_STATUSCODE_GOOD) {
        qDebug() << "状态不佳:" << UA_StatusCode_name(value->status);
        return;
    }

    QVariant qtValue;

    // 检查是否有数据
    if (!UA_Variant_isEmpty(&value->value)) {
        qtValue = uaVariantToQVariant(value->value);
       // qDebug() << "转换结果 - 有效:" << qtValue.isValid()
       //          << "值:" << qtValue.toString()
        //         << "类型:" << qtValue.typeName();
    } else {
        qDebug() << "Variant为空";
    }

    if (qtValue.isValid()) {
        // 更新时间戳
        QDateTime timestamp = QDateTime::currentDateTime();
        if (value->hasSourceTimestamp) {
            timestamp = QDateTime::fromMSecsSinceEpoch(
                UA_DateTime_toUnixTime(value->sourceTimestamp) * 1000);
        }

        // 更新变量定义
        handle->variableDef->setValue(qtValue,
                                      timestamp,
                                      statusCodeToQuality(value->status));

        // 更新缓存
        handle->lastValue = qtValue;
        handle->lastStatus.quality = statusCodeToQuality(value->status);

        // 发出信号
        emit variableValueChanged(handle->tagName, qtValue,
                                  timestamp,
                                  statusCodeToQuality(value->status));

       // qDebug() << "✅ 数据更新成功";
    } else {
        qDebug() << "❌ 数据无效，跳过更新";
    }
}



void OPCUAVariableManager::deleteSubscriptionCallback(
    UA_Client *client, UA_UInt32 subId, void *subContext)//OPC UA 订阅被删除时的回调函数
{
    Q_UNUSED(client);
    Q_UNUSED(client);

    // 1. 获取管理器实例
    OPCUAVariableManager* manager = static_cast<OPCUAVariableManager*>(subContext);
    if (!manager) {
        return;
    }

    // 2. 验证是否是我们关心的订阅
    if (subId != manager->m_subscriptionId) {
        qDebug() << "Received delete callback for unrelated subscription:" << subId;
        return;
    }

    // 3. 在主线程中处理（线程安全）
    QMetaObject::invokeMethod(manager, "onSubscriptionDeleted",
                              Qt::QueuedConnection,
                              Q_ARG(UA_UInt32, subId));
}



/*
void OPCUAVariableManager::updateVariableFromCallback(OPCUAVariableHandle* handle,
                                                      UA_DataValue* value)
{
    qDebug() << "\n=== 处理回调数据 ===";
    qDebug() << "变量:" << (handle ? handle->tagName : "NULL");

    qDebug() << "DataValue指针:" << value;

    if (!handle || !value || !handle->variableDef) {
        qDebug() << "错误: 参数无效";
        if (value) {
            qDebug() << "值状态:" << UA_StatusCode_name(value->status);
            qDebug() << "是否有值:" << value->hasValue;
        }
        return;
    }

    // 转换数据
    QVariant qtValue = uaVariantToQVariant(value->value);
    qDebug() << "转换后的QVariant:";
    qDebug() << "  是否有效:" << qtValue.isValid();
    qDebug() << "  值:" << qtValue.toString();
    qDebug() << "  类型:" << qtValue.typeName();

   // if (!qtValue.isValid()) {
     //   qDebug() << "错误: QVariant转换失败";
    ///    qDebug() << "原始数据类型:" << value->value.type->typeName;
   //     return;
  //  }

    // 调用VariableDefinition的setValue
    qDebug() << "调用 setValue...";
    handle->variableDef->setValue(qtValue,
                                  QDateTime::currentDateTime(),
                                  statusCodeToQuality(value->status));

    // 检查设置后的值
    QVariant checkValue = handle->variableDef->value();
    qDebug() << "设置后检查值:";
    qDebug() << "  是否有效:" << checkValue.isValid();
    qDebug() << "  值:" << checkValue.toString();
    qDebug() << "  类型:" << checkValue.typeName();

    // 继续原有逻辑...
    handle->lastValue = qtValue;
    handle->lastStatus.quality = statusCodeToQuality(value->status);

    emit variableValueChanged(handle->tagName, qtValue,
                              QDateTime::currentDateTime(),
                              statusCodeToQuality(value->status));

    qDebug() << "信号已发出";
}
*/

/*
void OPCUAVariableManager::updateVariableFromCallback(OPCUAVariableHandle *handle,UA_DataValue *value) //数据处理
{
    if (!handle || !value || !handle->variableDef) {
        return;
    }

    // 转换为 QVariant
    QVariant qtValue = uaVariantToQVariant(value->value);
    if (!qtValue.isValid()) {
        qWarning() << "Failed to convert value for" << handle->tagName;
        return;
    }

    // 时间戳
    QDateTime timestamp = QDateTime::currentDateTime();
    if (value->hasSourceTimestamp) {
        timestamp = QDateTime::fromMSecsSinceEpoch(
            UA_DateTime_toUnixTime(value->sourceTimestamp) * 1000);
    }

    // 质量
    DataQuality quality = statusCodeToQuality(value->status);

    // 更新 VariableDefinition（现在接受 QVariant）
    handle->variableDef->setValue(qtValue, timestamp, quality);

    // 更新句柄缓存
    handle->lastValue = qtValue;
    handle->lastStatus.quality = quality;

    // 发出信号
    emit variableValueChanged(handle->tagName, qtValue, timestamp, quality);

    qDebug() << QString("订阅更新: %1 = %2 [%3]")
                    .arg(handle->tagName)
                    .arg(qtValue.toString())
                    .arg(dataQualityToString(quality));
}
*/
// ==================== 私有方法实现 ====================

bool OPCUAVariableManager::initialize()
{
    if (m_isInitialized) {
        return true;
    }

    // 这里可以添加额外的初始化逻辑

    m_isInitialized = true;
    return true;
}

void OPCUAVariableManager::cleanup()
{
    if (!m_isInitialized) {
        return;
    }

    // 清理逻辑
    m_isInitialized = false;
}

/*
bool OPCUAVariableManager::initialize()
{
    if (m_isInitialized) {
        return true;
    }

    try {

        // 初始化连接管理器
        m_connectionManager = std::make_unique<OPCUAConnectionManager>();//智能指针,不用new和delete

        // 初始化线程池
        m_threadPool = new QThreadPool(this);//创建线程
        m_threadPool->setMaxThreadCount(m_maxThreadCount);//设置线程池中最多可以同时运行的线程数量

        // 初始化定时器，当模式设置为轮训模式时定时读取所有注册变量
        m_pollingTimer = new QTimer(this);
        m_pollingTimer->setSingleShot(false);//设置定时器为周期性的=1单次的
        QObject::connect(m_pollingTimer, &QTimer::timeout, this, &OPCUAVariableManager::onPollingTimer);


        // 5. 初始化数据结构
        m_variables.clear();
        resetStatistics();


        m_isInitialized = true;
        return true;

    } catch (...) {
        recordError("Exception during initialization");
        cleanup();  // 清理部分初始化的资源
        return false;
    }
}

void OPCUAVariableManager::cleanup()
{
    if (!m_isInitialized) {
        return;
    }

    // 停止所有活动
    stopSubscription();
    m_pollingTimer->stop();
    disconnect();

    // 清理资源
    if (m_threadPool) {
        m_threadPool->waitForDone(3000);
        delete m_threadPool;
        m_threadPool = nullptr;
    }

    // 清理变量
    {
        QWriteLocker locker(&m_variablesLock);
        m_variables.clear();
    }

    // 清理同步等待
    {
        QMutexLocker locker(&m_syncWaitMutex);
        qDeleteAll(m_syncWaitMap);
        m_syncWaitMap.clear();
    }

    // 清理连接管理器
    m_connectionManager.reset();

    m_isInitialized = false;
    emit cleanedUp();
}
*/

int OPCUAVariableManager::generateRequestId()//生成唯一的请求标识符
{
    return ++m_requestIdCounter;
}

void OPCUAVariableManager::addPendingRequest(const OperationRequest &request)//记录请求
{
    QMutexLocker locker(&m_requestsMutex);
    m_pendingRequests.insert(request.requestId, request);
}

void OPCUAVariableManager::removePendingRequest(int requestId)//移除请求
{
    QMutexLocker locker(&m_requestsMutex);
    m_pendingRequests.remove(requestId);
}

bool OPCUAVariableManager::waitForCompletion(int requestId, int timeoutMs,
                                             QVariant &result, QString &error)//同步操作的等待机制
{
    // 创建等待数据
    SyncWaitData *waitData = new SyncWaitData;

    {
        QMutexLocker locker(&m_syncWaitMutex);
        m_syncWaitMap.insert(requestId, waitData);
    }

    // 等待完成
    bool success = false;
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker(&m_syncWaitMutex);
        while (!waitData->completed && timer.elapsed() < timeoutMs) {
            waitData->condition.wait(&m_syncWaitMutex, 100);
        }

        if (waitData->completed) {
            success = true;
            result = waitData->result;
            error = waitData->error;
        } else {
            error = "Operation timeout";
            removePendingRequest(requestId);
        }

        m_syncWaitMap.remove(requestId);
    }

    delete waitData;
    return success;
}

void OPCUAVariableManager::recordError(const QString &error)
{
    QMutexLocker locker(&m_errorMutex);

    // 限制错误日志大小
    if (m_errorLog.size() > 1000) {
        m_errorLog.removeFirst();
    }

    m_errorLog.append(qMakePair(QDateTime::currentDateTime(), error));

    qDebug() << "Error recorded:" << error;
}

void OPCUAVariableManager::recordSuccess(const QString &operation)
{
    qDebug() << "Success:" << operation;
}

bool OPCUAVariableManager::attemptGracefulReconnect()
{
    qDebug() << "Attempting graceful reconnection...";

    // 先尝试优雅地断开
    try {
        // 保存当前订阅状态
        bool hadSubscription = isSubscribed();

        // 停止订阅
        stopSubscription();

        // 断开连接
        disconnect();

        // 等待一会儿
        QThread::msleep(500);

        // 重新连接
        bool success = reconnect();

        if (success && hadSubscription) {
            // 恢复订阅
            startSubscription(m_subscriptionMode);
        }

        return success;
    } catch (const std::exception &e) {
        qCritical() << "Graceful reconnection failed:" << e.what();
        return false;
    }
}

bool OPCUAVariableManager::parseNodeId(const QString &address, UA_NodeId &nodeId)//解析NodeId
{
    qDebug() << "\n======parseNodeId开始 ===";
    qDebug() << "输入地址:" << address;
    UA_NodeId_clear(&nodeId);
    UA_NodeId_init(&nodeId);
    if (address.isEmpty()) {
        qDebug() << " 地址为空";
        return false;
    }

    QString finalAddress = address;
    if (!address.contains("ns=") && !address.contains("i=") &&
        !address.contains("s=") && !address.contains("g=")) {
        // 如果只是字符串标识符，假设 namespace 为 2
        finalAddress = QString("ns=2;s=%1").arg(address);
        qDebug() << "转换为标准格式:" << finalAddress;
    }

    UA_String uaAddress = qStringToUAString(finalAddress);//将QString转换为字符数组接受的类型const char*类型
    qDebug() << "UA_String长度:" << uaAddress.length;
    //将地址解析为NodeId，核心转换，输入的opc地址转换为open62541的地址
    UA_StatusCode status = UA_NodeId_parse(&nodeId, uaAddress);
    qDebug() << "解析状态码:" << UA_StatusCode_name(status);//状态解码

    UA_String_clear(&uaAddress);

    if (status == UA_STATUSCODE_GOOD) {
        qDebug() << "解析成功";
        qDebug() << "  Namespace:" << nodeId.namespaceIndex;
        qDebug() << "  IdentifierType:" << nodeId.identifierType;

        // 打印标识符详情
        switch (nodeId.identifierType) {
        case UA_NODEIDTYPE_STRING:
            if (nodeId.identifier.string.data) {
                QString strId = QString::fromUtf8(
                    (char*)nodeId.identifier.string.data,
                    nodeId.identifier.string.length);
                qDebug() << "  字符串标识符:" << strId;
            }
            break;
        case UA_NODEIDTYPE_NUMERIC:
            qDebug() << "  数字标识符:" << nodeId.identifier.numeric;
            break;
        case UA_NODEIDTYPE_GUID:
            qDebug() << "  GUID标识符";
            break;
        case UA_NODEIDTYPE_BYTESTRING:
            qDebug() << "  字节串标识符，长度:" << nodeId.identifier.byteString.length;
            break;
        }
        return true;
    } else {
        qDebug() << "解析失败";
        return false;
    }
}

QString OPCUAVariableManager::nodeIdToString(const UA_NodeId &nodeId) const//将OPC UA 内部的 UA_NodeId 结构地址转换为字符串形式的节点
{
    UA_String uaStr = UA_STRING_NULL;
    UA_StatusCode status = UA_NodeId_print(&nodeId, &uaStr);

    if (status != UA_STATUSCODE_GOOD || uaStr.length == 0) {
        return QString();
    }

    QString result = QString::fromUtf8(reinterpret_cast<const char*>(uaStr.data),
                                       static_cast<int>(uaStr.length));

    // 清理 UA_String 分配的内存
    UA_String_clear(&uaStr);
    return result;
}

DataQuality OPCUAVariableManager::statusCodeToQuality(UA_StatusCode statusCode) const//将 OPC UA 状态码转换为应用层的质量标志
{
    if (statusCode == UA_STATUSCODE_GOOD) {
        return QUALITY_GOOD;
    }
    else if (statusCode == UA_STATUSCODE_BADNOTCONNECTED ||
             statusCode == UA_STATUSCODE_BADCONNECTIONCLOSED) {
        return QUALITY_COMM_FAIL;
    }
    else if (statusCode == UA_STATUSCODE_BADOUTOFRANGE) {
        return QUALITY_OUT_RANGE;
    }
    else if (statusCode == UA_STATUSCODE_BADSENSORFAILURE) {
        return QUALITY_SENSOR_FAIL;
    }
    else {
        return QUALITY_BAD;
    }
}

OPCUAVariableHandle* OPCUAVariableManager::getOrCreateHandle(const QString &tagName)//获取或创建"变量句柄
{
    QWriteLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return it->get();
    }

    return nullptr;
}

const OPCUAVariableHandle* OPCUAVariableManager::getHandle(const QString &tagName) const//获取句柄
{
    QReadLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it != m_variables.end()) {
        return it->get();
    }

    return nullptr;
}

bool OPCUAVariableManager::createSubscription()//创建阅订
{
    if (!m_connectionManager->isConnected() || !m_connectionManager->client()) {
        recordError("Cannot create subscription: connection manager is null or not connected");
        return false;
    }
    // 准备订阅请求
    UA_CreateSubscriptionRequest request;
    UA_CreateSubscriptionRequest_init(&request);

    request.requestedPublishingInterval = m_subscriptionConfig.publishingInterval;
    request.requestedLifetimeCount = m_subscriptionConfig.lifetimeCount;// 生命周期计数60
    request.requestedMaxKeepAliveCount = m_subscriptionConfig.maxKeepAliveCount; // 最大保活计数10
    request.maxNotificationsPerPublish = 100; // 无限制
    request.publishingEnabled = true; // 启用发布
    request.priority = m_subscriptionConfig.priority;// 订阅优先级
     //  创建订阅
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(
        m_connectionManager->client(), request,(void*)this, nullptr, deleteSubscriptionCallback);
    // 处理响应 清理资源
    if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
        m_subscriptionId = response.subscriptionId;
        UA_CreateSubscriptionResponse_clear(&response);
        return true;
    } else {
        qWarning() << "Failed to create subscription:"
                   << UA_StatusCode_name(response.responseHeader.serviceResult);
        UA_CreateSubscriptionResponse_clear(&response);
        return false;
    }
}

bool OPCUAVariableManager::deleteSubscription()//删除阅订
{
    if (m_subscriptionId == 0 || !m_connectionManager->client()) {
        return false;
    }

    UA_StatusCode status = UA_Client_Subscriptions_deleteSingle(
        m_connectionManager->client(), m_subscriptionId);

    if (status == UA_STATUSCODE_GOOD) {
        m_subscriptionId = 0;
        return true;
    } else {
        qWarning() << "Failed to delete subscription:" << UA_StatusCode_name(status);
        return false;
    }
}


//原版不要删除

/*
bool OPCUAVariableManager::createMonitoredItem(OPCUAVariableHandle *handle)//OPC UA 变量创建监控项
{
    if (m_subscriptionId == 0 || !handle || !m_connectionManager->client()) {
        return false;
    }

    // 创建监控项请求
    UA_MonitoredItemCreateRequest monRequest;
    UA_MonitoredItemCreateRequest_init(&monRequest);

    monRequest.itemToMonitor.nodeId = handle->nodeId;
    monRequest.itemToMonitor.attributeId = UA_ATTRIBUTEID_VALUE;
    monRequest.monitoringMode = UA_MONITORINGMODE_REPORTING;
    monRequest.requestedParameters.samplingInterval = m_monitoredItemConfig.samplingInterval;
    monRequest.requestedParameters.discardOldest = m_monitoredItemConfig.discardOldest;
    monRequest.requestedParameters.queueSize = m_monitoredItemConfig.queueSize;
    monRequest.requestedParameters.clientHandle = m_monitoredItemConfig.clientHandle;

    UA_MonitoredItemCreateResult result = UA_Client_MonitoredItems_createDataChange(
        m_connectionManager->client(), m_subscriptionId, UA_TIMESTAMPSTORETURN_BOTH,
        monRequest, (void*)handle, dataChangeNotificationCallback, nullptr);

    if (result.statusCode == UA_STATUSCODE_GOOD) {
        handle->monitoredItemId = result.monitoredItemId;
        handle->isSubscribed = true;

        qDebug() << "Created monitored item for variable:" << handle->tagName
                 << "Item ID:" << handle->monitoredItemId;

        UA_MonitoredItemCreateResult_clear(&result);
        return true;
    } else {
        qWarning() << "Failed to create monitored item for" << handle->tagName
                   << ":" << UA_StatusCode_name(result.statusCode);

        UA_MonitoredItemCreateResult_clear(&result);
        return false;
    }
}
*/


bool OPCUAVariableManager::createMonitoredItem(OPCUAVariableHandle *handle)
{
    if (m_subscriptionId == 0 || !handle || !m_connectionManager->client()) {
        qDebug() << "创建监控项失败：参数无效";
        return false;
    }

    qDebug() << "创建监控项：" << handle->tagName;

    // 创建监控项请求（保持你的专业初始化）
    UA_MonitoredItemCreateRequest monRequest;
    UA_MonitoredItemCreateRequest_init(&monRequest);

    monRequest.itemToMonitor.nodeId = handle->nodeId;
    monRequest.itemToMonitor.attributeId = UA_ATTRIBUTEID_VALUE;
    monRequest.monitoringMode = UA_MONITORINGMODE_REPORTING;

    // 使用配置（这是你的优点）
    monRequest.requestedParameters.samplingInterval = m_monitoredItemConfig.samplingInterval;//1000
    monRequest.requestedParameters.discardOldest = m_monitoredItemConfig.discardOldest;//true
    monRequest.requestedParameters.queueSize = m_monitoredItemConfig.queueSize;//1


    // 可选：添加clientHandle
    if (m_monitoredItemConfig.clientHandle != 0) {
        monRequest.requestedParameters.clientHandle = m_monitoredItemConfig.clientHandle;
    }

    qDebug() << "  采样间隔：" << monRequest.requestedParameters.samplingInterval << "ms"
             << "队列大小：" << monRequest.requestedParameters.queueSize;

    UA_MonitoredItemCreateResult result = UA_Client_MonitoredItems_createDataChange(
        m_connectionManager->client(), m_subscriptionId, UA_TIMESTAMPSTORETURN_BOTH,
        monRequest, (void*)handle, dataChangeNotificationCallback, nullptr);

    if (result.statusCode == UA_STATUSCODE_GOOD) {
        handle->monitoredItemId = result.monitoredItemId;
        handle->isSubscribed = true;

        qDebug() << "监控项创建成功：" << handle->tagName
                 << "ID：" << handle->monitoredItemId
                 << "实际间隔：" << result.revisedSamplingInterval << "ms";

        UA_MonitoredItemCreateResult_clear(&result);
        return true;
    } else {
        qWarning() << "监控项创建失败：" << handle->tagName
                   << "错误：" << UA_StatusCode_name(result.statusCode);

        UA_MonitoredItemCreateResult_clear(&result);
        return false;
    }
}


bool OPCUAVariableManager::deleteMonitoredItem(OPCUAVariableHandle *handle)//删除监控项
{
    if (!handle || !handle->isSubscribed || m_subscriptionId == 0) {
        return false;
    }

    if (m_subscriptionId > 0 && handle->monitoredItemId > 0) {
        UA_StatusCode status = UA_Client_MonitoredItems_deleteSingle(
            m_connectionManager->client(), m_subscriptionId, handle->monitoredItemId);

        if (status == UA_STATUSCODE_GOOD) {
            handle->isSubscribed = false;
            handle->monitoredItemId = 0;

            qDebug() << "Deleted monitored item for variable:" << handle->tagName;
            return true;
        }
    }

    return false;
}

QString OPCUAVariableManager::connectionStateToString(ConnectionState state) const//将链接状态转换为字符串
{
   return  m_connectionManager->connectionStateName();

}

void OPCUAVariableManager::executeBrowseTask(const QString &tagName)
{
    // 浏览节点的实现
    QWriteLocker locker(&m_variablesLock);

    auto it = m_variables.find(tagName);
    if (it == m_variables.end()) {
        emit variableNodeBrowsed(tagName, false, "Variable not registered");
        return;
    }

    OPCUAVariableHandle *handle = it->get();
    VariableDefinition *var = handle->variableDef;

    if (!var) {
        emit variableNodeBrowsed(tagName, false, "Variable definition is null");
        return;
    }

    QString address = var->address();
    if (address.isEmpty()) {
        // 如果没有指定地址，使用标签名作为节点ID
        address = QString("ns=2;s=%1").arg(tagName);
    }

    UA_NodeId nodeId;
    if (parseNodeId(address, nodeId)) {
        UA_NodeId_clear(&handle->nodeId);
        handle->nodeId = nodeId;
        handle->isBrowsed = true;

        emit variableNodeBrowsed(tagName, true, QString("Node browsed successfully: %1").arg(address));
        recordSuccess(QString("Browsed variable node: %1 -> %2").arg(tagName).arg(address));
    } else {
        handle->isBrowsed = false;

        emit variableNodeBrowsed(tagName, false,
                                 QString("Failed to parse node address: %1").arg(address));
        recordError(QString("Failed to browse variable node: %1 -> %2").arg(tagName).arg(address));
    }
}



//简化版
QVariant OPCUAVariableManager::uaVariantToQVariant(const UA_Variant &variant) const
{
    if (!variant.data || !variant.type) {
        return QVariant();
    }

    const UA_DataType* type = variant.type;

    // 处理标量数据
    if (variant.arrayLength == 0 && variant.arrayDimensionsSize == 0) {
        if (type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
            return QVariant(*(UA_Boolean*)variant.data != 0);
        }
        else if (type == &UA_TYPES[UA_TYPES_SBYTE]) {
            return QVariant((int)*(UA_SByte*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_BYTE]) {
            return QVariant((uint)*(UA_Byte*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_INT16]) {
            return QVariant((int)*(UA_Int16*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_UINT16]) {
            return QVariant((uint)*(UA_UInt16*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_INT32]) {
            return QVariant(*(UA_Int32*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_UINT32]) {
            return QVariant(*(UA_UInt32*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_INT64]) {
            return QVariant((qlonglong)*(UA_Int64*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_UINT64]) {
            return QVariant((qulonglong)*(UA_UInt64*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_FLOAT]) {
            return QVariant((double)*(UA_Float*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {
            return QVariant(*(UA_Double*)variant.data);
        }
        else if (type == &UA_TYPES[UA_TYPES_STRING]) {
            UA_String* str = (UA_String*)variant.data;
            return QVariant(QString::fromUtf8((char*)str->data, str->length));
        }
        else if (type == &UA_TYPES[UA_TYPES_DATETIME]) {
            UA_DateTime dt = *(UA_DateTime*)variant.data;
            qint64 unixTime = UA_DateTime_toUnixTime(dt) * 1000;
            return QVariant(QDateTime::fromMSecsSinceEpoch(unixTime));
        }
    }
    // 单元素数组特殊情况
    else if (variant.arrayLength == 1) {
        // 递归处理第一个元素
        UA_Variant singleElement;
        UA_Variant_init(&singleElement);
        singleElement.type = variant.type;
        singleElement.data = variant.data;
        singleElement.arrayLength = 0;
        singleElement.arrayDimensionsSize = 0;
        return uaVariantToQVariant(singleElement);
    }

    qWarning() << "Unsupported OPC UA type:" << (type ? type->typeName : "null");
    return QVariant();
}

UA_Variant OPCUAVariableManager::qVariantToUAVariant(const QVariant &qtVariant,
                                                     const UA_DataType* expectedType) const
{
    UA_Variant uaVariant;
    UA_Variant_init(&uaVariant);

    if (!qtVariant.isValid()) {
        return uaVariant;
    }

    // 如果有期望类型，优先使用
    if (expectedType) {
        if (expectedType == &UA_TYPES[UA_TYPES_BOOLEAN]) {
            UA_Boolean value = qtVariant.toBool();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_FLOAT]) {
            UA_Float value = qtVariant.toFloat();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_DOUBLE]) {
            UA_Double value = qtVariant.toDouble();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_INT32]) {
            UA_Int32 value = qtVariant.toInt();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_UINT32]) {
            UA_UInt32 value = qtVariant.toUInt();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_STRING]) {
            QString str = qtVariant.toString();
            UA_String uaStr = UA_STRING_ALLOC(str.toUtf8().constData());
            UA_Variant_setScalarCopy(&uaVariant, &uaStr, expectedType);
            UA_String_clear(&uaStr);
        }
        else if (expectedType == &UA_TYPES[UA_TYPES_DATETIME]) {
            QDateTime dt = qtVariant.toDateTime();
            if (dt.isValid()) {
                UA_DateTime uaDt = UA_DateTime_fromUnixTime(dt.toMSecsSinceEpoch() / 1000);
                UA_Variant_setScalarCopy(&uaVariant, &uaDt, expectedType);
            }
        }
        return uaVariant;
    }

    // 自动检测（简化版）
    switch (qtVariant.userType()) {
    case QMetaType::Bool:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_BOOLEAN]);
    case QMetaType::Float:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_FLOAT]);
    case QMetaType::Double:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_DOUBLE]);
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::Short:
    case QMetaType::UShort:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_INT32]);
    case QMetaType::QString:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_STRING]);
    case QMetaType::QDateTime:
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_DATETIME]);
    }

    // 默认尝试转换为double
    if (qtVariant.canConvert<double>()) {
        return qVariantToUAVariant(qtVariant, &UA_TYPES[UA_TYPES_DOUBLE]);
    }

    return uaVariant;
}


//以前的调试版，如果简化版有哦问题，可以退回调试版

/*
QVariant OPCUAVariableManager::uaVariantToQVariant(const UA_Variant &variant) const
{
   // qDebug() << "\n=== uaVariantToQVariant 详细调试 ===";
   // qDebug() << "variant.data:" << variant.data;
   // qDebug() << "variant.type 指针:" << variant.type;

    if (variant.type) {
        //qDebug() << "variant.type->typeName:" << variant.type->typeName;
       // qDebug() << "variant.type->typeId:" << variant.type->typeId;
        //qDebug() << "variant.type->memSize:" << variant.type->memSize;
    }

   // qDebug() << "variant.arrayLength:" << variant.arrayLength;
   // qDebug() << "variant.arrayDimensionsSize:" << variant.arrayDimensionsSize;

    if (!variant.data || !variant.type) {
        qDebug() << "错误: 数据或类型指针为空";
        return QVariant();
    }

    // 🔴 关键修复：检查数组维度
    if (variant.arrayLength > 0 || variant.arrayDimensionsSize > 0) {
        qDebug() << "警告: 检测到数组或维度数据";
        qDebug() << "arrayLength:" << variant.arrayLength;
        qDebug() << "arrayDimensionsSize:" << variant.arrayDimensionsSize;

        // 尝试处理单元素数组
        if (variant.arrayLength == 1) {
            qDebug() << "尝试提取单元素数组的第一个元素";

            // 获取数组指针
            void* arrayData = variant.data;

            if (variant.type == &UA_TYPES[UA_TYPES_FLOAT]) {
                UA_Float* floatArray = (UA_Float*)arrayData;
                UA_Float floatValue = floatArray[0];
                qDebug() << "提取的UA_Float值:" << floatValue;
                return QVariant(static_cast<double>(floatValue));
            }
            else if (variant.type == &UA_TYPES[UA_TYPES_DOUBLE]) {
                UA_Double* doubleArray = (UA_Double*)arrayData;
                UA_Double doubleValue = doubleArray[0];
                qDebug() << "提取的UA_Double值:" << doubleValue;
                return QVariant(doubleValue);
            }
        }

       // qWarning() << "Array type not supported:"
        //           << (variant.type ? variant.type->typeName : "unknown");
        return QVariant();
    }

    // 处理标量数据
    const UA_DataType* type = variant.type;

   // qDebug() << "处理标量数据，类型:" << type->typeName;

    // 🔴 改进的 float 处理
    if (type == &UA_TYPES[UA_TYPES_FLOAT]) {
        try {
            UA_Float rawValue = *(UA_Float*)variant.data;
            //qDebug() << "原始UA_Float值:" << rawValue;

            // 转换为 double
            double convertedValue = static_cast<double>(rawValue);
            QVariant result = QVariant(convertedValue);

           // qDebug() << "转换后double值:" << convertedValue;
            //qDebug() << "QVariant有效性:" << result.isValid();
           // qDebug() << "QVariant类型:" << result.typeName();

            return result;
        } catch (...) {
            qWarning() << "处理UA_Float时发生异常";
            return QVariant();
        }
    }
    else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {
        try {
            UA_Double rawValue = *(UA_Double*)variant.data;
            //qDebug() << "原始UA_Double值:" << rawValue;

            QVariant result = QVariant(rawValue);
           // qDebug() << "QVariant有效性:" << result.isValid();
           // qDebug() << "QVariant类型:" << result.typeName();

            return result;
        } catch (...) {
            qWarning() << "处理UA_Double时发生异常";
            return QVariant();
        }
    }
    // ... 其他类型处理

    qWarning() << "未支持的OPC UA类型:" << type->typeName;
    return QVariant();
}

UA_Variant OPCUAVariableManager::qVariantToUAVariant(const QVariant &qtVariant,
                                                     const UA_DataType* expectedType) const
{
    UA_Variant uaVariant;
    UA_Variant_init(&uaVariant);

   // qDebug() << "\n=== qVariantToUAVariant ===";
   // qDebug() << "Input QVariant:" << qtVariant << "type:" << qtVariant.typeName()
    //         << "type id:" << qtVariant.userType();
    //qDebug() << "Expected OPC UA type:" << (expectedType ? expectedType->typeName : "null");

    if (!qtVariant.isValid()) {
        qDebug() << "Invalid QVariant";
        return uaVariant;
    }

    // 如果有期望类型，按期望类型转换
    if (expectedType) {
        bool converted = false;

        // 布尔类型
        if (expectedType == &UA_TYPES[UA_TYPES_BOOLEAN]) {
            UA_Boolean value = qtVariant.toBool();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Boolean:" << value << "(from" << qtVariant << ")";
        }
        // 双精度浮点数
        else if (expectedType == &UA_TYPES[UA_TYPES_DOUBLE]) {
            UA_Double value = qtVariant.toDouble();
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Double:" << value << "(from" << qtVariant << ")";
        }
        // 单精度浮点数 - 关键修复！
        else if (expectedType == &UA_TYPES[UA_TYPES_FLOAT]) {
            // 支持从各种类型转换为 Float
            if (qtVariant.canConvert<float>()) {
                UA_Float value = qtVariant.toFloat();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to Float:" << value << "(from" << qtVariant << ")";
            } else {
                // 尝试从 double 转换
                double doubleValue = qtVariant.toDouble();
                UA_Float value = static_cast<UA_Float>(doubleValue);
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted double to Float:" << value << "(from" << doubleValue << ")";
            }
        }
        // 32位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT32]) {
            if (qtVariant.canConvert<int>()) {
                UA_Int32 value = qtVariant.toInt();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to Int32:" << value << "(from" << qtVariant << ")";
            } else {
                // 尝试从 double 转换
                double doubleValue = qtVariant.toDouble();
                UA_Int32 value = static_cast<UA_Int32>(doubleValue);
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted double to Int32:" << value << "(from" << doubleValue << ")";
            }
        }
        // 无符号32位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_UINT32]) {
            if (qtVariant.canConvert<uint>()) {
                UA_UInt32 value = qtVariant.toUInt();
                UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                converted = true;
                qDebug() << "Converted to UInt32:" << value << "(from" << qtVariant << ")";
            }
        }
        // 16位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT16]) {
            UA_Int16 value = static_cast<UA_Int16>(qtVariant.toInt());
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Int16:" << value << "(from" << qtVariant << ")";
        }
        // 64位整数
        else if (expectedType == &UA_TYPES[UA_TYPES_INT64]) {
            UA_Int64 value = static_cast<UA_Int64>(qtVariant.toLongLong());
            UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
            converted = true;
            qDebug() << "Converted to Int64:" << value << "(from" << qtVariant << ")";
        }
        // 字符串
        else if (expectedType == &UA_TYPES[UA_TYPES_STRING]) {
            QString str = qtVariant.toString();
            QByteArray utf8 = str.toUtf8();

            // 安全地分配字符串内存
            UA_String *uaStr = (UA_String*)UA_malloc(sizeof(UA_String));
            if (uaStr) {
                uaStr->length = utf8.length();
                uaStr->data = (UA_Byte*)UA_malloc(uaStr->length);
                if (uaStr->data) {
                    memcpy(uaStr->data, utf8.constData(), uaStr->length);
                    UA_Variant_setScalar(&uaVariant, uaStr, expectedType);
                    converted = true;
                    qDebug() << "Converted to String:" << str << "(from" << qtVariant << ")";
                } else {
                    UA_free(uaStr);
                }
            }
        }
        // 字节串
        else if (expectedType == &UA_TYPES[UA_TYPES_BYTESTRING]) {
            QByteArray bytes = qtVariant.toByteArray();
            UA_ByteString *uaBytes = (UA_ByteString*)UA_malloc(sizeof(UA_ByteString));
            if (uaBytes) {
                uaBytes->length = bytes.length();
                uaBytes->data = (UA_Byte*)UA_malloc(uaBytes->length);
                if (uaBytes->data) {
                    memcpy(uaBytes->data, bytes.constData(), uaBytes->length);
                    UA_Variant_setScalar(&uaVariant, uaBytes, expectedType);
                    converted = true;
                    qDebug() << "Converted to ByteString, length:" << bytes.length();
                } else {
                    UA_free(uaBytes);
                }
            }
        }
        // 日期时间
        else if (expectedType == &UA_TYPES[UA_TYPES_DATETIME]) {
            QDateTime dt = qtVariant.toDateTime();
            if (dt.isValid()) {
                UA_DateTime uaDt = UA_DateTime_fromUnixTime(dt.toMSecsSinceEpoch() / 1000);
                UA_Variant_setScalarCopy(&uaVariant, &uaDt, expectedType);
                converted = true;
                qDebug() << "Converted to DateTime:" << dt.toString();
            }
        }

        if (!converted) {
            qWarning() << "Cannot convert QVariant to expected OPC UA type:"
                       << (expectedType ? expectedType->typeName : "null");
            qDebug() << "QVariant value:" << qtVariant << "type:" << qtVariant.typeName();

            // 尝试最后的自动转换
            qDebug() << "Attempting fallback conversion...";
            switch (qtVariant.userType()) {
            case QMetaType::Double:
            case QMetaType::Float:
                if (expectedType == &UA_TYPES[UA_TYPES_INT32]) {
                    UA_Int32 value = static_cast<UA_Int32>(qtVariant.toDouble());
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted float/double to Int32:" << value;
                }
                else if (expectedType == &UA_TYPES[UA_TYPES_FLOAT]) {
                    UA_Float value = qtVariant.toFloat();
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted to Float:" << value;
                }
                else if (expectedType == &UA_TYPES[UA_TYPES_DOUBLE]) {
                    UA_Double value = qtVariant.toDouble();
                    UA_Variant_setScalarCopy(&uaVariant, &value, expectedType);
                    converted = true;
                    qDebug() << "Fallback: Converted to Double:" << value;
                }
                break;
            }
        }

        qDebug() << "Conversion result:" << (converted ? "success" : "failed");
        return uaVariant;
    }

    // ========== 自动检测类型（无期望类型时）==========
   // qDebug() << "No expected type specified, auto-detecting...";

    int variantType = qtVariant.userType();

    // 布尔类型
    if (variantType == QMetaType::Bool) {
        UA_Boolean value = qtVariant.toBool();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
       // qDebug() << "Auto-converted to Boolean:" << value;
    }
    // 双精度浮点数
    else if (variantType == QMetaType::Double) {
        UA_Double value = qtVariant.toDouble();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
       // qDebug() << "Auto-converted to Double:" << value;
    }
    // 单精度浮点数 - 关键修复！
    else if (variantType == QMetaType::Float) {
        UA_Float value = qtVariant.toFloat();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_FLOAT]);
       // qDebug() << "Auto-converted to Float:" << value;
    }
    // 整数类型
    else if (variantType == QMetaType::Int ||
             variantType == QMetaType::UInt ||
             variantType == QMetaType::Short ||
             variantType == QMetaType::UShort) {
        UA_Int32 value = qtVariant.toInt();
        UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_INT32]);
      //  qDebug() << "Auto-converted to Int32:" << value;
    }
    // 大整数类型
    else if (variantType == QMetaType::Long ||
             variantType == QMetaType::LongLong ||
             variantType == QMetaType::ULong ||
             variantType == QMetaType::ULongLong) {
        qint64 value = qtVariant.toLongLong();
        if (value >= INT32_MIN && value <= INT32_MAX) {
            UA_Int32 val32 = static_cast<UA_Int32>(value);
            UA_Variant_setScalarCopy(&uaVariant, &val32, &UA_TYPES[UA_TYPES_INT32]);
            qDebug() << "Auto-converted long to Int32:" << val32;
        } else {
            UA_Int64 val64 = static_cast<UA_Int64>(value);
            UA_Variant_setScalarCopy(&uaVariant, &val64, &UA_TYPES[UA_TYPES_INT64]);
           // qDebug() << "Auto-converted to Int64:" << val64;
        }
    }
    // 字符串
    else if (variantType == QMetaType::QString) {
        QString str = qtVariant.toString();
        QByteArray utf8 = str.toUtf8();

        UA_String *uaStr = (UA_String*)UA_malloc(sizeof(UA_String));
        if (uaStr) {
            uaStr->length = utf8.length();
            uaStr->data = (UA_Byte*)UA_malloc(uaStr->length);
            if (uaStr->data) {
                memcpy(uaStr->data, utf8.constData(), uaStr->length);
                UA_Variant_setScalar(&uaVariant, uaStr, &UA_TYPES[UA_TYPES_STRING]);
                qDebug() << "Auto-converted to String:" << str;
            } else {
                UA_free(uaStr);
            }
        }
    }
    // 字节数组
    else if (variantType == QMetaType::QByteArray) {
        QByteArray bytes = qtVariant.toByteArray();
        UA_ByteString *uaBytes = (UA_ByteString*)UA_malloc(sizeof(UA_ByteString));
        if (uaBytes) {
            uaBytes->length = bytes.length();
            uaBytes->data = (UA_Byte*)UA_malloc(uaBytes->length);
            if (uaBytes->data) {
                memcpy(uaBytes->data, bytes.constData(), uaBytes->length);
                UA_Variant_setScalar(&uaVariant, uaBytes, &UA_TYPES[UA_TYPES_BYTESTRING]);
                qDebug() << "Auto-converted to ByteString, length:" << bytes.length();
            } else {
                UA_free(uaBytes);
            }
        }
    }
    // 日期时间
    else if (variantType == QMetaType::QDateTime) {
        QDateTime dt = qtVariant.toDateTime();
        if (dt.isValid()) {
            UA_DateTime uaDt = UA_DateTime_fromUnixTime(dt.toMSecsSinceEpoch() / 1000);
            UA_Variant_setScalarCopy(&uaVariant, &uaDt, &UA_TYPES[UA_TYPES_DATETIME]);
            qDebug() << "Auto-converted to DateTime:" << dt.toString();
        }
    }
    // 未知类型
    else {
        qWarning() << "Cannot auto-convert QVariant type:" << qtVariant.typeName()
        << "(type id:" << variantType << ")";
        qDebug() << "QVariant value:" << qtVariant;

        // 尝试通用转换
        if (qtVariant.canConvert<double>()) {
            UA_Double value = qtVariant.toDouble();
            UA_Variant_setScalarCopy(&uaVariant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
            qDebug() << "Generic conversion to Double:" << value;
        }
    }

    qDebug() << "Auto-conversion result:" << (uaVariant.data ? "success" : "failed");
    return uaVariant;
}

*/


} // namespace Industrial


// ==================== OPCUATask 实现 ====================
namespace Industrial {

// 修改构造函数以接受 QVariant
OPCUATask::OPCUATask(OperationType type, const QString &tagName,
                     const QVariant &data, int requestId,  // 改为 QVariant
                     OPCUAVariableManager *manager)
    : m_type(type)
    , m_tagName(tagName)
    , m_data(data)  // 直接存储 QVariant
    , m_requestId(requestId)
    , m_manager(manager)
{
    setAutoDelete(true);
}


OPCUATask::~OPCUATask() {

}


bool OPCUATask::connectTemporaryClient(UA_Client *client) {
    if (!client || !m_manager) {
        return false;
    }

    // 从管理器获取连接信息
    QString endpoint = m_manager->serverEndPointUrl();
    if (endpoint.isEmpty()) {
        return false;
    }

    UA_StatusCode status = UA_Client_connect(client, endpoint.toUtf8().constData());
    return status == UA_STATUSCODE_GOOD;
}


// 在 OPCUATask 类中修改所有执行函数
QVariant OPCUATask::executeReadSingle()
{
    if (m_tagName.isEmpty()) {
        qDebug() << "Read failed: tagName is empty";
        return QVariant();
    }
    //判断句柄和变量定义是否为空
    OPCUAVariableHandle* handle = m_manager->getVariableHandle(m_tagName);//获得变量句柄
    if (!handle || !handle->variableDef) {
        qDebug() << "Read failed: variable not found:" << m_tagName;
        return QVariant();
    }
    //判断opc地址是否为空
    QString address = handle->variableDef->address();
    if (address.isEmpty()) {
        qDebug() << "Read failed: address is empty for:" << m_tagName;
        return QVariant();
    }
    //判断client是否为空
    UA_Client* mainClient = m_manager->m_connectionManager->client();
    if (!mainClient) {
        qDebug() << "Read failed: client not available";
        return QVariant();
    }

    UA_Variant value;
    UA_Variant_init(&value);
     UA_StatusCode status = UA_Client_readValueAttribute(mainClient, handle->nodeId, &value);//读取变量值

    QVariant result;
    if (status == UA_STATUSCODE_GOOD) {
        result = m_manager->uaVariantToQVariant(value);
        updateVariableDirectly(handle,result,status,m_manager);//跟新变量值
        qDebug() << "Read success:" << m_tagName << "=" << result;
    } else {
        qDebug() << "Read failed:" << m_tagName << "error:" << UA_StatusCode_name(status);
    }

    UA_Variant_clear(&value);
    //UA_NodeId_clear(&nodeId);

    return result;//返回了值
}

QVariant OPCUATask::executeReadBatch()
{
    if (!m_data.canConvert<QStringList>()) {
        qDebug() << "Batch read failed: data is not QStringList";
        return QVariant();
    }

    QStringList tagNames = m_data.toStringList();
    if (tagNames.isEmpty()) {
        qDebug() << "Batch read: empty tag list";
        return QVariantMap();
    }

    // 使用主客户端
    if (!m_manager->m_connectionManager) {
        qDebug() << "Batch read failed: connection manager is null";
        return QVariant();
    }

    UA_Client* mainClient = m_manager->m_connectionManager->client();
    if (!mainClient) {
        qDebug() << "Batch read failed: client is null";
        return QVariant();
    }

    QVariantMap results;

    for (const QString &tagName : tagNames) {
        // 通过tagName获取变量句柄
        OPCUAVariableHandle* handle = m_manager->getVariableHandle(tagName);

        if (!handle || !handle->variableDef) {
            qDebug() << "Batch read: variable not found:" << tagName;
            results[tagName] = QVariant();
            continue;
        }

        // 获取address
        QString address = handle->variableDef->address();
        if (address.isEmpty()) {
            qDebug() << "Batch read: address is empty for:" << tagName;
            results[tagName] = QVariant();
            continue;
        }

        // 读取值
        UA_Variant value;
        UA_Variant_init(&value);

        UA_StatusCode status = UA_Client_readValueAttribute(mainClient,handle->nodeId, &value);

        if (status == UA_STATUSCODE_GOOD) {
            results[tagName] = m_manager->uaVariantToQVariant(value);
             updateVariableDirectly(handle,results[tagName],status,m_manager);//跟新变量值
            qDebug() << "Batch read success:" << tagName << "=" << results[tagName];
        } else {
            qDebug() << "Batch read failed:" << tagName << "error:" << UA_StatusCode_name(status);
            results[tagName] = QVariant();
        }

        UA_Variant_clear(&value);
        //UA_NodeId_clear(&nodeId);
    }

    return results;
}

QVariant OPCUATask::executeWriteSingle(){

    if (m_tagName.isEmpty()) {
        qDebug() << "Write failed: tagName is empty";
        return QVariant(false);
    }

    // 通过tagName获取变量句柄
    OPCUAVariableHandle* handle = m_manager->getVariableHandle(m_tagName);
    if (!handle || !handle->variableDef) {
        qDebug() << "Write failed: variable not found:" << m_tagName;
        return QVariant(false);
    }

    // 检查变量是否可写
    if (!handle->variableDef->writable()) {
        qDebug() << "Write failed: variable is not writable:" << m_tagName;
        return QVariant(false);
    }

    // 获取address
    QString address = handle->variableDef->address();
    if (address.isEmpty()) {
        qDebug() << "Write failed: address is empty for:" << m_tagName;
        return QVariant(false);
    }

    // 使用主客户端
    if (!m_manager->m_connectionManager) {
        qDebug() << "Write failed: connection manager is null";
        return QVariant(false);
    }

    UA_Client* mainClient = m_manager->m_connectionManager->client();
    if (!mainClient) {
        qDebug() << "Write failed: client is null";
        return QVariant(false);
    }

    // 将 QVariant 转换为 UA_Variant
    // 需要根据变量的数据类型进行转换
    UA_Variant uaVariant = m_manager->qVariantToUAVariant(m_data);
    if (!uaVariant.data) {
        qDebug() << "Write failed: cannot convert value for:" << m_tagName;
        //UA_NodeId_clear(&nodeId);
        return QVariant(false);
    }

    // 执行写入
    UA_StatusCode status = UA_Client_writeValueAttribute(mainClient, handle->nodeId, &uaVariant);

    bool success = (status == UA_STATUSCODE_GOOD);
    if (success) {
        qDebug() << "Write success:" << m_tagName << "=" << m_data;
    } else {
        qDebug() << "Write failed:" << m_tagName << "error:" << UA_StatusCode_name(status);
    }

    UA_Variant_clear(&uaVariant);
   // UA_NodeId_clear(&nodeId);

    return QVariant(success);
}

QVariant OPCUATask::executeWriteBatch()
{
    if (!m_data.canConvert<QVariantMap>()) {
        qDebug() << "Batch write failed: data is not QVariantMap";
        return QVariant(false);
    }

    QVariantMap variantMap = m_data.toMap();
    if (variantMap.isEmpty()) {
        qDebug() << "Batch write: empty write map";
        return QVariant(true);
    }

    // 使用主客户端
    if (!m_manager->m_connectionManager) {
        qDebug() << "Batch write failed: connection manager is null";
        return QVariant(false);
    }

    UA_Client* mainClient = m_manager->m_connectionManager->client();
    if (!mainClient) {
        qDebug() << "Batch write failed: client is null";
        return QVariant(false);
    }

    bool allSuccess = true;

    for (auto it = variantMap.begin(); it != variantMap.end(); ++it) {
        const QString &tagName = it.key();
        const QVariant &value = it.value();

        // 通过tagName获取变量句柄
        OPCUAVariableHandle* handle = m_manager->getVariableHandle(tagName);
        if (!handle || !handle->variableDef) {
            qDebug() << "Batch write: variable not found:" << tagName;
            allSuccess = false;
            continue;
        }

        // 检查变量是否可写
        if (!handle->variableDef->writable()) {
            qDebug() << "Batch write: variable is not writable:" << tagName;
            allSuccess = false;
            continue;
        }

        // 获取address
        QString address = handle->variableDef->address();
        if (address.isEmpty()) {
            qDebug() << "Batch write: address is empty for:" << tagName;
            allSuccess = false;
            continue;
        }

        // 将 QVariant 转换为 UA_Variant
        UA_Variant uaVariant = m_manager->qVariantToUAVariant(value);
        if (!uaVariant.data) {
            qDebug() << "Batch write: cannot convert value for:" << tagName;
            //UA_NodeId_clear(&nodeId);
            allSuccess = false;
            continue;
        }

        // 执行写入
        UA_StatusCode status = UA_Client_writeValueAttribute(mainClient, handle->nodeId, &uaVariant);

        if (status == UA_STATUSCODE_GOOD) {
            qDebug() << "Batch write success:" << tagName << "=" << value;
        } else {
            qDebug() << "Batch write failed:" << tagName << "error:" << UA_StatusCode_name(status);
            allSuccess = false;
        }

        UA_Variant_clear(&uaVariant);
        //UA_NodeId_clear(&nodeId);
    }

    return QVariant(allSuccess);
}

QVariant OPCUATask::executeBrowse()
{
    if (m_tagName.isEmpty()) {
        qDebug() << "Browse failed: tagName is empty";
        return QVariant(false);
    }

    // 通过tagName获取变量句柄
    OPCUAVariableHandle* handle = m_manager->getVariableHandle(m_tagName);
    if (!handle || !handle->variableDef) {
        qDebug() << "Browse failed: variable not found:" << m_tagName;
        return QVariant(false);
    }

    // 获取address
    QString address = handle->variableDef->address();
    if (address.isEmpty()) {
        qDebug() << "Browse failed: address is empty for:" << m_tagName;
        return QVariant(false);
    }

    // 使用主客户端
    if (!m_manager->m_connectionManager) {
        qDebug() << "Browse failed: connection manager is null";
        return QVariant(false);
    }

    UA_Client* mainClient = m_manager->m_connectionManager->client();
    if (!mainClient) {
        qDebug() << "Browse failed: client is null";
        return QVariant(false);
    }

    // 执行浏览（这里可以添加更复杂的浏览逻辑）
    // 简单实现：验证节点存在即可
    UA_Variant value;
    UA_Variant_init(&value);

    UA_StatusCode status = UA_Client_readValueAttribute(mainClient, handle->nodeId, &value);

    bool success = (status == UA_STATUSCODE_GOOD || status == UA_STATUSCODE_BADNOTREADABLE);

    if (success) {
        qDebug() << "Browse success: node exists" << m_tagName << "->" << address;
        if (status == UA_STATUSCODE_GOOD) {
            UA_Variant_clear(&value);
        }
    } else {
        qDebug() << "Browse failed:" << m_tagName << "error:" << UA_StatusCode_name(status);
    }


    //UA_NodeId_clear(&nodeId);
    return QVariant(success);
}




void OPCUATask::run() {
    QElapsedTimer timer;
    timer.start();

    bool success = false;
    QVariant result;
    QString error;

    try {
        switch (m_type) {
        case OP_READ_SINGLE:
            result = executeReadSingle();
            success = !result.isNull();
            break;

        case OP_READ_BATCH:
            result = executeReadBatch();
            success = !result.isNull();
            break;

        case OP_WRITE_SINGLE:
            result = executeWriteSingle();
            success = result.toBool();
            break;

        case OP_WRITE_BATCH:
            result = executeWriteBatch();
            success = result.toBool();
            break;

        case OP_BROWSE:
            result = executeBrowse();
            success = result.toBool();
            break;

        default:
            error = QString("Unknown operation type: %1").arg(static_cast<int>(m_type));
            break;
        }

        if (!success && error.isEmpty()) {
            error = "Operation execution failed";
        }
    } catch (const std::exception &e) {
        error = QString("Exception: %1").arg(e.what());
        qCritical() << "OPCUATask exception:" << e.what();
    } catch (...) {
        error = "Unknown exception";
        qCritical() << "OPCUATask unknown exception";
    }


    int elapsed = timer.elapsed();
    qDebug() << "OPCUATask" << m_requestId << "(" << m_type << "," << m_tagName
             << ") completed in" << elapsed << "ms, success:" << success;

    // 发出完成信号（注意：需要在主线程处理）
    QMetaObject::invokeMethod(this, "taskCompleted", Qt::QueuedConnection,
                              Q_ARG(int, m_requestId),
                              Q_ARG(bool, success),
                              Q_ARG(QVariant, result),
                              Q_ARG(QString, error));

    }
} // namespace Industrial


namespace Industrial {

DataParse::DataParse(QObject *parent):QObject(parent)
{

}



}

