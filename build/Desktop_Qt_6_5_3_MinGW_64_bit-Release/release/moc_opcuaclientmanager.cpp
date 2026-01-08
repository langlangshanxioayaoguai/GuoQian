/****************************************************************************
** Meta object code from reading C++ file 'opcuaclientmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../industrial/opcuaclientmanager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'opcuaclientmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS = QtMocHelpers::stringData(
    "Industrial::OPCUAConnectionManager",
    "stateChanged",
    "",
    "ConnectionState",
    "newState",
    "oldState",
    "connected",
    "disconnected",
    "connectionLost",
    "reconnecting",
    "attempt",
    "delay",
    "connectionError",
    "error",
    "keepaliveReceived",
    "keepaliveFailed",
    "logAttemptChanged",
    "details",
    "onKeepaliveTimer",
    "onReconnectTimer"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS_t {
    uint offsetsAndSizes[40];
    char stringdata0[35];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[9];
    char stringdata5[9];
    char stringdata6[10];
    char stringdata7[13];
    char stringdata8[15];
    char stringdata9[13];
    char stringdata10[8];
    char stringdata11[6];
    char stringdata12[16];
    char stringdata13[6];
    char stringdata14[18];
    char stringdata15[16];
    char stringdata16[18];
    char stringdata17[8];
    char stringdata18[17];
    char stringdata19[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 34),  // "Industrial::OPCUAConnectionMa..."
        QT_MOC_LITERAL(35, 12),  // "stateChanged"
        QT_MOC_LITERAL(48, 0),  // ""
        QT_MOC_LITERAL(49, 15),  // "ConnectionState"
        QT_MOC_LITERAL(65, 8),  // "newState"
        QT_MOC_LITERAL(74, 8),  // "oldState"
        QT_MOC_LITERAL(83, 9),  // "connected"
        QT_MOC_LITERAL(93, 12),  // "disconnected"
        QT_MOC_LITERAL(106, 14),  // "connectionLost"
        QT_MOC_LITERAL(121, 12),  // "reconnecting"
        QT_MOC_LITERAL(134, 7),  // "attempt"
        QT_MOC_LITERAL(142, 5),  // "delay"
        QT_MOC_LITERAL(148, 15),  // "connectionError"
        QT_MOC_LITERAL(164, 5),  // "error"
        QT_MOC_LITERAL(170, 17),  // "keepaliveReceived"
        QT_MOC_LITERAL(188, 15),  // "keepaliveFailed"
        QT_MOC_LITERAL(204, 17),  // "logAttemptChanged"
        QT_MOC_LITERAL(222, 7),  // "details"
        QT_MOC_LITERAL(230, 16),  // "onKeepaliveTimer"
        QT_MOC_LITERAL(247, 16)   // "onReconnectTimer"
    },
    "Industrial::OPCUAConnectionManager",
    "stateChanged",
    "",
    "ConnectionState",
    "newState",
    "oldState",
    "connected",
    "disconnected",
    "connectionLost",
    "reconnecting",
    "attempt",
    "delay",
    "connectionError",
    "error",
    "keepaliveReceived",
    "keepaliveFailed",
    "logAttemptChanged",
    "details",
    "onKeepaliveTimer",
    "onReconnectTimer"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   80,    2, 0x06,    1 /* Public */,
       6,    0,   85,    2, 0x06,    4 /* Public */,
       7,    0,   86,    2, 0x06,    5 /* Public */,
       8,    0,   87,    2, 0x06,    6 /* Public */,
       9,    2,   88,    2, 0x06,    7 /* Public */,
      12,    1,   93,    2, 0x06,   10 /* Public */,
      14,    0,   96,    2, 0x06,   12 /* Public */,
      15,    0,   97,    2, 0x06,   13 /* Public */,
      16,    1,   98,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      18,    0,  101,    2, 0x08,   16 /* Private */,
      19,    0,  102,    2, 0x08,   17 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::OPCUAConnectionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OPCUAConnectionManager, std::true_type>,
        // method 'stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionState, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionState, std::false_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectionLost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reconnecting'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'connectionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'keepaliveReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'keepaliveFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'logAttemptChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString, std::false_type>,
        // method 'onKeepaliveTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReconnectTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::OPCUAConnectionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OPCUAConnectionManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<ConnectionState>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ConnectionState>>(_a[2]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->connectionLost(); break;
        case 4: _t->reconnecting((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->connectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->keepaliveReceived(); break;
        case 7: _t->keepaliveFailed(); break;
        case 8: _t->logAttemptChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->onKeepaliveTimer(); break;
        case 10: _t->onReconnectTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OPCUAConnectionManager::*)(ConnectionState , ConnectionState );
            if (_t _q_method = &OPCUAConnectionManager::stateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)();
            if (_t _q_method = &OPCUAConnectionManager::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)();
            if (_t _q_method = &OPCUAConnectionManager::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)();
            if (_t _q_method = &OPCUAConnectionManager::connectionLost; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)(int , int );
            if (_t _q_method = &OPCUAConnectionManager::reconnecting; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)(const QString & );
            if (_t _q_method = &OPCUAConnectionManager::connectionError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)();
            if (_t _q_method = &OPCUAConnectionManager::keepaliveReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)();
            if (_t _q_method = &OPCUAConnectionManager::keepaliveFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (OPCUAConnectionManager::*)(const QString );
            if (_t _q_method = &OPCUAConnectionManager::logAttemptChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::OPCUAConnectionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::OPCUAConnectionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAConnectionManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::OPCUAConnectionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Industrial::OPCUAConnectionManager::stateChanged(ConnectionState _t1, ConnectionState _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::OPCUAConnectionManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Industrial::OPCUAConnectionManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Industrial::OPCUAConnectionManager::connectionLost()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Industrial::OPCUAConnectionManager::reconnecting(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Industrial::OPCUAConnectionManager::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Industrial::OPCUAConnectionManager::keepaliveReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Industrial::OPCUAConnectionManager::keepaliveFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Industrial::OPCUAConnectionManager::logAttemptChanged(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS = QtMocHelpers::stringData(
    "Industrial::OPCUAVariableManager",
    "connectionStateChanged",
    "",
    "ConnectionState",
    "state",
    "connected",
    "disconnected",
    "connectionLost",
    "reconnecting",
    "attempt",
    "delay",
    "connectionError",
    "error",
    "connectionRestored",
    "cleanedUp",
    "readCompleted",
    "requestId",
    "tagName",
    "value",
    "success",
    "writeCompleted",
    "batchReadCompleted",
    "values",
    "batchWriteCompleted",
    "variableValueChanged",
    "timestamp",
    "DataQuality",
    "quality",
    "variableStatusChanged",
    "NodeStatus",
    "status",
    "batchValuesUpdated",
    "alarmTriggered",
    "AlarmLevel",
    "level",
    "alarmCleared",
    "heartbeatReceived",
    "heartbeatTimeout",
    "variableNodeBrowsed",
    "allVariablesBrowsed",
    "successCount",
    "failureCount",
    "refreshAllVariablesAsync",
    "refreshVariableAsync",
    "refreshAllVariablesSync",
    "refreshVariableSync",
    "forceReconnect",
    "testConnectionAsync",
    "restartSubscription",
    "clearErrorLog",
    "dumpStatusToLog",
    "onSubscriptionDeleted",
    "UA_UInt32",
    "subId",
    "startProcessing",
    "stopProcessing",
    "onPollingTimer",
    "onConnectionStateChanged",
    "newState",
    "oldState",
    "onTaskCompleted",
    "result",
    "onInternalReconnect",
    "updateVariableFromCallback",
    "OPCUAVariableHandle*",
    "handle",
    "UA_DataValue*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS_t {
    uint offsetsAndSizes[134];
    char stringdata0[33];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[6];
    char stringdata5[10];
    char stringdata6[13];
    char stringdata7[15];
    char stringdata8[13];
    char stringdata9[8];
    char stringdata10[6];
    char stringdata11[16];
    char stringdata12[6];
    char stringdata13[19];
    char stringdata14[10];
    char stringdata15[14];
    char stringdata16[10];
    char stringdata17[8];
    char stringdata18[6];
    char stringdata19[8];
    char stringdata20[15];
    char stringdata21[19];
    char stringdata22[7];
    char stringdata23[20];
    char stringdata24[21];
    char stringdata25[10];
    char stringdata26[12];
    char stringdata27[8];
    char stringdata28[22];
    char stringdata29[11];
    char stringdata30[7];
    char stringdata31[19];
    char stringdata32[15];
    char stringdata33[11];
    char stringdata34[6];
    char stringdata35[13];
    char stringdata36[18];
    char stringdata37[17];
    char stringdata38[20];
    char stringdata39[20];
    char stringdata40[13];
    char stringdata41[13];
    char stringdata42[25];
    char stringdata43[21];
    char stringdata44[24];
    char stringdata45[20];
    char stringdata46[15];
    char stringdata47[20];
    char stringdata48[20];
    char stringdata49[14];
    char stringdata50[16];
    char stringdata51[22];
    char stringdata52[10];
    char stringdata53[6];
    char stringdata54[16];
    char stringdata55[15];
    char stringdata56[15];
    char stringdata57[25];
    char stringdata58[9];
    char stringdata59[9];
    char stringdata60[16];
    char stringdata61[7];
    char stringdata62[20];
    char stringdata63[27];
    char stringdata64[21];
    char stringdata65[7];
    char stringdata66[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 32),  // "Industrial::OPCUAVariableManager"
        QT_MOC_LITERAL(33, 22),  // "connectionStateChanged"
        QT_MOC_LITERAL(56, 0),  // ""
        QT_MOC_LITERAL(57, 15),  // "ConnectionState"
        QT_MOC_LITERAL(73, 5),  // "state"
        QT_MOC_LITERAL(79, 9),  // "connected"
        QT_MOC_LITERAL(89, 12),  // "disconnected"
        QT_MOC_LITERAL(102, 14),  // "connectionLost"
        QT_MOC_LITERAL(117, 12),  // "reconnecting"
        QT_MOC_LITERAL(130, 7),  // "attempt"
        QT_MOC_LITERAL(138, 5),  // "delay"
        QT_MOC_LITERAL(144, 15),  // "connectionError"
        QT_MOC_LITERAL(160, 5),  // "error"
        QT_MOC_LITERAL(166, 18),  // "connectionRestored"
        QT_MOC_LITERAL(185, 9),  // "cleanedUp"
        QT_MOC_LITERAL(195, 13),  // "readCompleted"
        QT_MOC_LITERAL(209, 9),  // "requestId"
        QT_MOC_LITERAL(219, 7),  // "tagName"
        QT_MOC_LITERAL(227, 5),  // "value"
        QT_MOC_LITERAL(233, 7),  // "success"
        QT_MOC_LITERAL(241, 14),  // "writeCompleted"
        QT_MOC_LITERAL(256, 18),  // "batchReadCompleted"
        QT_MOC_LITERAL(275, 6),  // "values"
        QT_MOC_LITERAL(282, 19),  // "batchWriteCompleted"
        QT_MOC_LITERAL(302, 20),  // "variableValueChanged"
        QT_MOC_LITERAL(323, 9),  // "timestamp"
        QT_MOC_LITERAL(333, 11),  // "DataQuality"
        QT_MOC_LITERAL(345, 7),  // "quality"
        QT_MOC_LITERAL(353, 21),  // "variableStatusChanged"
        QT_MOC_LITERAL(375, 10),  // "NodeStatus"
        QT_MOC_LITERAL(386, 6),  // "status"
        QT_MOC_LITERAL(393, 18),  // "batchValuesUpdated"
        QT_MOC_LITERAL(412, 14),  // "alarmTriggered"
        QT_MOC_LITERAL(427, 10),  // "AlarmLevel"
        QT_MOC_LITERAL(438, 5),  // "level"
        QT_MOC_LITERAL(444, 12),  // "alarmCleared"
        QT_MOC_LITERAL(457, 17),  // "heartbeatReceived"
        QT_MOC_LITERAL(475, 16),  // "heartbeatTimeout"
        QT_MOC_LITERAL(492, 19),  // "variableNodeBrowsed"
        QT_MOC_LITERAL(512, 19),  // "allVariablesBrowsed"
        QT_MOC_LITERAL(532, 12),  // "successCount"
        QT_MOC_LITERAL(545, 12),  // "failureCount"
        QT_MOC_LITERAL(558, 24),  // "refreshAllVariablesAsync"
        QT_MOC_LITERAL(583, 20),  // "refreshVariableAsync"
        QT_MOC_LITERAL(604, 23),  // "refreshAllVariablesSync"
        QT_MOC_LITERAL(628, 19),  // "refreshVariableSync"
        QT_MOC_LITERAL(648, 14),  // "forceReconnect"
        QT_MOC_LITERAL(663, 19),  // "testConnectionAsync"
        QT_MOC_LITERAL(683, 19),  // "restartSubscription"
        QT_MOC_LITERAL(703, 13),  // "clearErrorLog"
        QT_MOC_LITERAL(717, 15),  // "dumpStatusToLog"
        QT_MOC_LITERAL(733, 21),  // "onSubscriptionDeleted"
        QT_MOC_LITERAL(755, 9),  // "UA_UInt32"
        QT_MOC_LITERAL(765, 5),  // "subId"
        QT_MOC_LITERAL(771, 15),  // "startProcessing"
        QT_MOC_LITERAL(787, 14),  // "stopProcessing"
        QT_MOC_LITERAL(802, 14),  // "onPollingTimer"
        QT_MOC_LITERAL(817, 24),  // "onConnectionStateChanged"
        QT_MOC_LITERAL(842, 8),  // "newState"
        QT_MOC_LITERAL(851, 8),  // "oldState"
        QT_MOC_LITERAL(860, 15),  // "onTaskCompleted"
        QT_MOC_LITERAL(876, 6),  // "result"
        QT_MOC_LITERAL(883, 19),  // "onInternalReconnect"
        QT_MOC_LITERAL(903, 26),  // "updateVariableFromCallback"
        QT_MOC_LITERAL(930, 20),  // "OPCUAVariableHandle*"
        QT_MOC_LITERAL(951, 6),  // "handle"
        QT_MOC_LITERAL(958, 13)   // "UA_DataValue*"
    },
    "Industrial::OPCUAVariableManager",
    "connectionStateChanged",
    "",
    "ConnectionState",
    "state",
    "connected",
    "disconnected",
    "connectionLost",
    "reconnecting",
    "attempt",
    "delay",
    "connectionError",
    "error",
    "connectionRestored",
    "cleanedUp",
    "readCompleted",
    "requestId",
    "tagName",
    "value",
    "success",
    "writeCompleted",
    "batchReadCompleted",
    "values",
    "batchWriteCompleted",
    "variableValueChanged",
    "timestamp",
    "DataQuality",
    "quality",
    "variableStatusChanged",
    "NodeStatus",
    "status",
    "batchValuesUpdated",
    "alarmTriggered",
    "AlarmLevel",
    "level",
    "alarmCleared",
    "heartbeatReceived",
    "heartbeatTimeout",
    "variableNodeBrowsed",
    "allVariablesBrowsed",
    "successCount",
    "failureCount",
    "refreshAllVariablesAsync",
    "refreshVariableAsync",
    "refreshAllVariablesSync",
    "refreshVariableSync",
    "forceReconnect",
    "testConnectionAsync",
    "restartSubscription",
    "clearErrorLog",
    "dumpStatusToLog",
    "onSubscriptionDeleted",
    "UA_UInt32",
    "subId",
    "startProcessing",
    "stopProcessing",
    "onPollingTimer",
    "onConnectionStateChanged",
    "newState",
    "oldState",
    "onTaskCompleted",
    "result",
    "onInternalReconnect",
    "updateVariableFromCallback",
    "OPCUAVariableHandle*",
    "handle",
    "UA_DataValue*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      21,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  242,    2, 0x06,    1 /* Public */,
       5,    0,  245,    2, 0x06,    3 /* Public */,
       6,    0,  246,    2, 0x06,    4 /* Public */,
       7,    0,  247,    2, 0x06,    5 /* Public */,
       8,    2,  248,    2, 0x06,    6 /* Public */,
      11,    1,  253,    2, 0x06,    9 /* Public */,
      13,    0,  256,    2, 0x06,   11 /* Public */,
      14,    0,  257,    2, 0x06,   12 /* Public */,
      15,    5,  258,    2, 0x06,   13 /* Public */,
      20,    4,  269,    2, 0x06,   19 /* Public */,
      21,    4,  278,    2, 0x06,   24 /* Public */,
      23,    3,  287,    2, 0x06,   29 /* Public */,
      24,    4,  294,    2, 0x06,   33 /* Public */,
      28,    2,  303,    2, 0x06,   38 /* Public */,
      31,    1,  308,    2, 0x06,   41 /* Public */,
      32,    3,  311,    2, 0x06,   43 /* Public */,
      35,    1,  318,    2, 0x06,   47 /* Public */,
      36,    0,  321,    2, 0x06,   49 /* Public */,
      37,    0,  322,    2, 0x06,   50 /* Public */,
      38,    3,  323,    2, 0x06,   51 /* Public */,
      39,    2,  330,    2, 0x06,   55 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      42,    0,  335,    2, 0x0a,   58 /* Public */,
      43,    1,  336,    2, 0x0a,   59 /* Public */,
      44,    0,  339,    2, 0x0a,   61 /* Public */,
      45,    1,  340,    2, 0x0a,   62 /* Public */,
      46,    0,  343,    2, 0x0a,   64 /* Public */,
      47,    0,  344,    2, 0x0a,   65 /* Public */,
      48,    0,  345,    2, 0x0a,   66 /* Public */,
      49,    0,  346,    2, 0x0a,   67 /* Public */,
      50,    0,  347,    2, 0x0a,   68 /* Public */,
      51,    1,  348,    2, 0x0a,   69 /* Public */,
      54,    0,  351,    2, 0x0a,   71 /* Public */,
      55,    0,  352,    2, 0x0a,   72 /* Public */,
      56,    0,  353,    2, 0x08,   73 /* Private */,
      57,    2,  354,    2, 0x08,   74 /* Private */,
      60,    4,  359,    2, 0x08,   77 /* Private */,
      62,    0,  368,    2, 0x08,   82 /* Private */,
      63,    2,  369,    2, 0x08,   83 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QVariant, QMetaType::Bool, QMetaType::QString,   16,   17,   18,   19,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   16,   17,   19,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantMap, QMetaType::Bool, QMetaType::QString,   16,   22,   19,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::QString,   16,   19,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QDateTime, 0x80000000 | 26,   17,   18,   25,   27,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 29,   17,   30,
    QMetaType::Void, QMetaType::QVariantMap,   22,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 33, QMetaType::Double,   17,   34,   18,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   17,   19,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   40,   41,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 52,   53,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,   58,   59,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::QVariant, QMetaType::QString,   16,   19,   61,   12,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 64, 0x80000000 | 66,   65,   18,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::OPCUAVariableManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OPCUAVariableManager, std::true_type>,
        // method 'connectionStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionState, std::false_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectionLost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reconnecting'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'connectionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'connectionRestored'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cleanedUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'readCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'writeCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'batchReadCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'batchWriteCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableValueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'variableStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const NodeStatus &, std::false_type>,
        // method 'batchValuesUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'alarmTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<AlarmLevel, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'alarmCleared'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'heartbeatReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'heartbeatTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'variableNodeBrowsed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'allVariablesBrowsed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'refreshAllVariablesAsync'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshVariableAsync'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'refreshAllVariablesSync'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshVariableSync'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'forceReconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testConnectionAsync'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restartSubscription'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearErrorLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'dumpStatusToLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSubscriptionDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UA_UInt32, std::false_type>,
        // method 'startProcessing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopProcessing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPollingTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnectionStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionState, std::false_type>,
        QtPrivate::TypeAndForceComplete<ConnectionState, std::false_type>,
        // method 'onTaskCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onInternalReconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateVariableFromCallback'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<OPCUAVariableHandle *, std::false_type>,
        QtPrivate::TypeAndForceComplete<UA_DataValue *, std::false_type>
    >,
    nullptr
} };

void Industrial::OPCUAVariableManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OPCUAVariableManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connectionStateChanged((*reinterpret_cast< std::add_pointer_t<ConnectionState>>(_a[1]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->connectionLost(); break;
        case 4: _t->reconnecting((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->connectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->connectionRestored(); break;
        case 7: _t->cleanedUp(); break;
        case 8: _t->readCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5]))); break;
        case 9: _t->writeCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 10: _t->batchReadCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 11: _t->batchWriteCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 12: _t->variableValueChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[4]))); break;
        case 13: _t->variableStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<NodeStatus>>(_a[2]))); break;
        case 14: _t->batchValuesUpdated((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 15: _t->alarmTriggered((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<AlarmLevel>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 16: _t->alarmCleared((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->heartbeatReceived(); break;
        case 18: _t->heartbeatTimeout(); break;
        case 19: _t->variableNodeBrowsed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 20: _t->allVariablesBrowsed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 21: _t->refreshAllVariablesAsync(); break;
        case 22: _t->refreshVariableAsync((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->refreshAllVariablesSync(); break;
        case 24: _t->refreshVariableSync((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->forceReconnect(); break;
        case 26: _t->testConnectionAsync(); break;
        case 27: _t->restartSubscription(); break;
        case 28: _t->clearErrorLog(); break;
        case 29: _t->dumpStatusToLog(); break;
        case 30: _t->onSubscriptionDeleted((*reinterpret_cast< std::add_pointer_t<UA_UInt32>>(_a[1]))); break;
        case 31: _t->startProcessing(); break;
        case 32: _t->stopProcessing(); break;
        case 33: _t->onPollingTimer(); break;
        case 34: _t->onConnectionStateChanged((*reinterpret_cast< std::add_pointer_t<ConnectionState>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ConnectionState>>(_a[2]))); break;
        case 35: _t->onTaskCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 36: _t->onInternalReconnect(); break;
        case 37: _t->updateVariableFromCallback((*reinterpret_cast< std::add_pointer_t<OPCUAVariableHandle*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<UA_DataValue*>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OPCUAVariableManager::*)(ConnectionState );
            if (_t _q_method = &OPCUAVariableManager::connectionStateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::connectionLost; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , int );
            if (_t _q_method = &OPCUAVariableManager::reconnecting; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & );
            if (_t _q_method = &OPCUAVariableManager::connectionError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::connectionRestored; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::cleanedUp; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , const QString & , const QVariant & , bool , const QString & );
            if (_t _q_method = &OPCUAVariableManager::readCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , const QString & , bool , const QString & );
            if (_t _q_method = &OPCUAVariableManager::writeCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , const QVariantMap & , bool , const QString & );
            if (_t _q_method = &OPCUAVariableManager::batchReadCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , bool , const QString & );
            if (_t _q_method = &OPCUAVariableManager::batchWriteCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & , const QVariant & , const QDateTime & , DataQuality );
            if (_t _q_method = &OPCUAVariableManager::variableValueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & , const NodeStatus & );
            if (_t _q_method = &OPCUAVariableManager::variableStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QVariantMap & );
            if (_t _q_method = &OPCUAVariableManager::batchValuesUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & , AlarmLevel , double );
            if (_t _q_method = &OPCUAVariableManager::alarmTriggered; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & );
            if (_t _q_method = &OPCUAVariableManager::alarmCleared; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::heartbeatReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)();
            if (_t _q_method = &OPCUAVariableManager::heartbeatTimeout; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(const QString & , bool , const QString & );
            if (_t _q_method = &OPCUAVariableManager::variableNodeBrowsed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (OPCUAVariableManager::*)(int , int );
            if (_t _q_method = &OPCUAVariableManager::allVariablesBrowsed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::OPCUAVariableManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::OPCUAVariableManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEOPCUAVariableManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::OPCUAVariableManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void Industrial::OPCUAVariableManager::connectionStateChanged(ConnectionState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::OPCUAVariableManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Industrial::OPCUAVariableManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Industrial::OPCUAVariableManager::connectionLost()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Industrial::OPCUAVariableManager::reconnecting(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Industrial::OPCUAVariableManager::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Industrial::OPCUAVariableManager::connectionRestored()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Industrial::OPCUAVariableManager::cleanedUp()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Industrial::OPCUAVariableManager::readCompleted(int _t1, const QString & _t2, const QVariant & _t3, bool _t4, const QString & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Industrial::OPCUAVariableManager::writeCompleted(int _t1, const QString & _t2, bool _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Industrial::OPCUAVariableManager::batchReadCompleted(int _t1, const QVariantMap & _t2, bool _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Industrial::OPCUAVariableManager::batchWriteCompleted(int _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Industrial::OPCUAVariableManager::variableValueChanged(const QString & _t1, const QVariant & _t2, const QDateTime & _t3, DataQuality _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Industrial::OPCUAVariableManager::variableStatusChanged(const QString & _t1, const NodeStatus & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Industrial::OPCUAVariableManager::batchValuesUpdated(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Industrial::OPCUAVariableManager::alarmTriggered(const QString & _t1, AlarmLevel _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Industrial::OPCUAVariableManager::alarmCleared(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Industrial::OPCUAVariableManager::heartbeatReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void Industrial::OPCUAVariableManager::heartbeatTimeout()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void Industrial::OPCUAVariableManager::variableNodeBrowsed(const QString & _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void Industrial::OPCUAVariableManager::allVariablesBrowsed(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS = QtMocHelpers::stringData(
    "Industrial::OPCUATask",
    "taskCompleted",
    "",
    "requestId",
    "success",
    "result",
    "error"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[22];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[8];
    char stringdata5[7];
    char stringdata6[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21),  // "Industrial::OPCUATask"
        QT_MOC_LITERAL(22, 13),  // "taskCompleted"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 9),  // "requestId"
        QT_MOC_LITERAL(47, 7),  // "success"
        QT_MOC_LITERAL(55, 6),  // "result"
        QT_MOC_LITERAL(62, 5)   // "error"
    },
    "Industrial::OPCUATask",
    "taskCompleted",
    "",
    "requestId",
    "success",
    "result",
    "error"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEOPCUATaskENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    4,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::QVariant, QMetaType::QString,    3,    4,    5,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::OPCUATask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEOPCUATaskENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OPCUATask, std::true_type>,
        // method 'taskCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Industrial::OPCUATask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OPCUATask *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->taskCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OPCUATask::*)(int , bool , const QVariant & , const QString & );
            if (_t _q_method = &OPCUATask::taskCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::OPCUATask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::OPCUATask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEOPCUATaskENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QRunnable"))
        return static_cast< QRunnable*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::OPCUATask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Industrial::OPCUATask::taskCompleted(int _t1, bool _t2, const QVariant & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS = QtMocHelpers::stringData(
    "Industrial::DataParse"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21)   // "Industrial::DataParse"
    },
    "Industrial::DataParse"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEDataParseENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::DataParse::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEDataParseENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DataParse, std::true_type>
    >,
    nullptr
} };

void Industrial::DataParse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *Industrial::DataParse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::DataParse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEDataParseENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::DataParse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
