/****************************************************************************
** Meta object code from reading C++ file 'realtimevariablemanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../industrial/realtimevariablemanager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'realtimevariablemanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS = QtMocHelpers::stringData(
    "Industrial::RealTimeVariable",
    "valueChanged",
    "",
    "value",
    "timestampChanged",
    "timestamp",
    "qualityChanged",
    "DataQuality",
    "quality",
    "alarmChanged",
    "AlarmLevel",
    "level",
    "alarmAcknowledged",
    "acknowledged",
    "valueOutOfRange",
    "updateValue",
    "acknowledgeAlarm",
    "resetAlarm",
    "alarmLevel"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS_t {
    uint offsetsAndSizes[38];
    char stringdata0[29];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[17];
    char stringdata5[10];
    char stringdata6[15];
    char stringdata7[12];
    char stringdata8[8];
    char stringdata9[13];
    char stringdata10[11];
    char stringdata11[6];
    char stringdata12[18];
    char stringdata13[13];
    char stringdata14[16];
    char stringdata15[12];
    char stringdata16[17];
    char stringdata17[11];
    char stringdata18[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS = {
    {
        QT_MOC_LITERAL(0, 28),  // "Industrial::RealTimeVariable"
        QT_MOC_LITERAL(29, 12),  // "valueChanged"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 5),  // "value"
        QT_MOC_LITERAL(49, 16),  // "timestampChanged"
        QT_MOC_LITERAL(66, 9),  // "timestamp"
        QT_MOC_LITERAL(76, 14),  // "qualityChanged"
        QT_MOC_LITERAL(91, 11),  // "DataQuality"
        QT_MOC_LITERAL(103, 7),  // "quality"
        QT_MOC_LITERAL(111, 12),  // "alarmChanged"
        QT_MOC_LITERAL(124, 10),  // "AlarmLevel"
        QT_MOC_LITERAL(135, 5),  // "level"
        QT_MOC_LITERAL(141, 17),  // "alarmAcknowledged"
        QT_MOC_LITERAL(159, 12),  // "acknowledged"
        QT_MOC_LITERAL(172, 15),  // "valueOutOfRange"
        QT_MOC_LITERAL(188, 11),  // "updateValue"
        QT_MOC_LITERAL(200, 16),  // "acknowledgeAlarm"
        QT_MOC_LITERAL(217, 10),  // "resetAlarm"
        QT_MOC_LITERAL(228, 10)   // "alarmLevel"
    },
    "Industrial::RealTimeVariable",
    "valueChanged",
    "",
    "value",
    "timestampChanged",
    "timestamp",
    "qualityChanged",
    "DataQuality",
    "quality",
    "alarmChanged",
    "AlarmLevel",
    "level",
    "alarmAcknowledged",
    "acknowledged",
    "valueOutOfRange",
    "updateValue",
    "acknowledgeAlarm",
    "resetAlarm",
    "alarmLevel"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPERealTimeVariableENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       4,  102, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    5 /* Public */,
       4,    1,   77,    2, 0x06,    7 /* Public */,
       6,    1,   80,    2, 0x06,    9 /* Public */,
       9,    1,   83,    2, 0x06,   11 /* Public */,
      12,    1,   86,    2, 0x06,   13 /* Public */,
      14,    1,   89,    2, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    2,   92,    2, 0x0a,   17 /* Public */,
      15,    1,   97,    2, 0x2a,   20 /* Public | MethodCloned */,
      16,    0,  100,    2, 0x0a,   22 /* Public */,
      17,    0,  101,    2, 0x0a,   23 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QDateTime,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::QVariant,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant, 0x80000000 | 7,    3,    8,
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QVariant, 0x00015001, uint(0), 0,
       5, QMetaType::QDateTime, 0x00015001, uint(1), 0,
       8, 0x80000000 | 7, 0x00015009, uint(2), 0,
      18, 0x80000000 | 10, 0x00015009, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::RealTimeVariable::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPERealTimeVariableENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS_t,
        // property 'value'
        QtPrivate::TypeAndForceComplete<QVariant, std::true_type>,
        // property 'timestamp'
        QtPrivate::TypeAndForceComplete<QDateTime, std::true_type>,
        // property 'quality'
        QtPrivate::TypeAndForceComplete<DataQuality, std::true_type>,
        // property 'alarmLevel'
        QtPrivate::TypeAndForceComplete<AlarmLevel, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RealTimeVariable, std::true_type>,
        // method 'valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'timestampChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        // method 'qualityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'alarmChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<AlarmLevel, std::false_type>,
        // method 'alarmAcknowledged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'valueOutOfRange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'updateValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'updateValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'acknowledgeAlarm'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetAlarm'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::RealTimeVariable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RealTimeVariable *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 1: _t->timestampChanged((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1]))); break;
        case 2: _t->qualityChanged((*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[1]))); break;
        case 3: _t->alarmChanged((*reinterpret_cast< std::add_pointer_t<AlarmLevel>>(_a[1]))); break;
        case 4: _t->alarmAcknowledged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->valueOutOfRange((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 6: _t->updateValue((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[2]))); break;
        case 7: _t->updateValue((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 8: _t->acknowledgeAlarm(); break;
        case 9: _t->resetAlarm(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RealTimeVariable::*)(const QVariant & );
            if (_t _q_method = &RealTimeVariable::valueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RealTimeVariable::*)(const QDateTime & );
            if (_t _q_method = &RealTimeVariable::timestampChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RealTimeVariable::*)(DataQuality );
            if (_t _q_method = &RealTimeVariable::qualityChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RealTimeVariable::*)(AlarmLevel );
            if (_t _q_method = &RealTimeVariable::alarmChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RealTimeVariable::*)(bool );
            if (_t _q_method = &RealTimeVariable::alarmAcknowledged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RealTimeVariable::*)(const QVariant & );
            if (_t _q_method = &RealTimeVariable::valueOutOfRange; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RealTimeVariable *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariant*>(_v) = _t->value(); break;
        case 1: *reinterpret_cast< QDateTime*>(_v) = _t->timestamp(); break;
        case 2: *reinterpret_cast< DataQuality*>(_v) = _t->quality(); break;
        case 3: *reinterpret_cast< AlarmLevel*>(_v) = _t->alarmLevel(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Industrial::RealTimeVariable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::RealTimeVariable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::RealTimeVariable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Industrial::RealTimeVariable::valueChanged(const QVariant & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::RealTimeVariable::timestampChanged(const QDateTime & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::RealTimeVariable::qualityChanged(DataQuality _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::RealTimeVariable::alarmChanged(AlarmLevel _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Industrial::RealTimeVariable::alarmAcknowledged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Industrial::RealTimeVariable::valueOutOfRange(const QVariant & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS = QtMocHelpers::stringData(
    "Industrial::RealTimeVariableManager",
    "variableAdded",
    "",
    "tagName",
    "variableRemoved",
    "variableUpdated",
    "value",
    "alarmTriggered",
    "AlarmLevel",
    "level",
    "alarmAcknowledged",
    "communicationError",
    "error",
    "performanceStatsUpdated",
    "PerformanceStats",
    "stats",
    "processUpdates",
    "logHistoricalData",
    "cleanupOldData",
    "checkConnectionStatus",
    "onStatsTimerTimeout"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS_t {
    uint offsetsAndSizes[42];
    char stringdata0[36];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[16];
    char stringdata5[16];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[11];
    char stringdata9[6];
    char stringdata10[18];
    char stringdata11[19];
    char stringdata12[6];
    char stringdata13[24];
    char stringdata14[17];
    char stringdata15[6];
    char stringdata16[15];
    char stringdata17[18];
    char stringdata18[15];
    char stringdata19[22];
    char stringdata20[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 35),  // "Industrial::RealTimeVariableM..."
        QT_MOC_LITERAL(36, 13),  // "variableAdded"
        QT_MOC_LITERAL(50, 0),  // ""
        QT_MOC_LITERAL(51, 7),  // "tagName"
        QT_MOC_LITERAL(59, 15),  // "variableRemoved"
        QT_MOC_LITERAL(75, 15),  // "variableUpdated"
        QT_MOC_LITERAL(91, 5),  // "value"
        QT_MOC_LITERAL(97, 14),  // "alarmTriggered"
        QT_MOC_LITERAL(112, 10),  // "AlarmLevel"
        QT_MOC_LITERAL(123, 5),  // "level"
        QT_MOC_LITERAL(129, 17),  // "alarmAcknowledged"
        QT_MOC_LITERAL(147, 18),  // "communicationError"
        QT_MOC_LITERAL(166, 5),  // "error"
        QT_MOC_LITERAL(172, 23),  // "performanceStatsUpdated"
        QT_MOC_LITERAL(196, 16),  // "PerformanceStats"
        QT_MOC_LITERAL(213, 5),  // "stats"
        QT_MOC_LITERAL(219, 14),  // "processUpdates"
        QT_MOC_LITERAL(234, 17),  // "logHistoricalData"
        QT_MOC_LITERAL(252, 14),  // "cleanupOldData"
        QT_MOC_LITERAL(267, 21),  // "checkConnectionStatus"
        QT_MOC_LITERAL(289, 19)   // "onStatsTimerTimeout"
    },
    "Industrial::RealTimeVariableManager",
    "variableAdded",
    "",
    "tagName",
    "variableRemoved",
    "variableUpdated",
    "value",
    "alarmTriggered",
    "AlarmLevel",
    "level",
    "alarmAcknowledged",
    "communicationError",
    "error",
    "performanceStatsUpdated",
    "PerformanceStats",
    "stats",
    "processUpdates",
    "logHistoricalData",
    "cleanupOldData",
    "checkConnectionStatus",
    "onStatsTimerTimeout"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,    1 /* Public */,
       4,    1,   89,    2, 0x06,    3 /* Public */,
       5,    2,   92,    2, 0x06,    5 /* Public */,
       7,    2,   97,    2, 0x06,    8 /* Public */,
      10,    1,  102,    2, 0x06,   11 /* Public */,
      11,    2,  105,    2, 0x06,   13 /* Public */,
      13,    1,  110,    2, 0x06,   16 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    0,  113,    2, 0x0a,   18 /* Public */,
      17,    0,  114,    2, 0x0a,   19 /* Public */,
      18,    0,  115,    2, 0x0a,   20 /* Public */,
      19,    0,  116,    2, 0x0a,   21 /* Public */,
      20,    0,  117,    2, 0x0a,   22 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    3,    9,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   12,
    QMetaType::Void, 0x80000000 | 14,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::RealTimeVariableManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RealTimeVariableManager, std::true_type>,
        // method 'variableAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'alarmTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<AlarmLevel, std::false_type>,
        // method 'alarmAcknowledged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'communicationError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'performanceStatsUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const PerformanceStats &, std::false_type>,
        // method 'processUpdates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'logHistoricalData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cleanupOldData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkConnectionStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStatsTimerTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::RealTimeVariableManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RealTimeVariableManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->variableAdded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->variableRemoved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->variableUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[2]))); break;
        case 3: _t->alarmTriggered((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<AlarmLevel>>(_a[2]))); break;
        case 4: _t->alarmAcknowledged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->communicationError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->performanceStatsUpdated((*reinterpret_cast< std::add_pointer_t<PerformanceStats>>(_a[1]))); break;
        case 7: _t->processUpdates(); break;
        case 8: _t->logHistoricalData(); break;
        case 9: _t->cleanupOldData(); break;
        case 10: _t->checkConnectionStatus(); break;
        case 11: _t->onStatsTimerTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RealTimeVariableManager::*)(const QString & );
            if (_t _q_method = &RealTimeVariableManager::variableAdded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const QString & );
            if (_t _q_method = &RealTimeVariableManager::variableRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const QString & , const QVariant & );
            if (_t _q_method = &RealTimeVariableManager::variableUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const QString & , AlarmLevel );
            if (_t _q_method = &RealTimeVariableManager::alarmTriggered; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const QString & );
            if (_t _q_method = &RealTimeVariableManager::alarmAcknowledged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const QString & , const QString & );
            if (_t _q_method = &RealTimeVariableManager::communicationError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (RealTimeVariableManager::*)(const PerformanceStats & );
            if (_t _q_method = &RealTimeVariableManager::performanceStatsUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::RealTimeVariableManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::RealTimeVariableManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPERealTimeVariableManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::RealTimeVariableManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Industrial::RealTimeVariableManager::variableAdded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::RealTimeVariableManager::variableRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::RealTimeVariableManager::variableUpdated(const QString & _t1, const QVariant & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::RealTimeVariableManager::alarmTriggered(const QString & _t1, AlarmLevel _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Industrial::RealTimeVariableManager::alarmAcknowledged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Industrial::RealTimeVariableManager::communicationError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Industrial::RealTimeVariableManager::performanceStatsUpdated(const PerformanceStats & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
