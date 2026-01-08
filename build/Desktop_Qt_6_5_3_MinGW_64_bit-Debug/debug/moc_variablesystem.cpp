/****************************************************************************
** Meta object code from reading C++ file 'variablesystem.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../industrial/variablesystem.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'variablesystem.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableDefinition",
    "descriptionChanged",
    "",
    "description",
    "unitChanged",
    "EngineeringUnit",
    "unit",
    "rangeChanged",
    "min",
    "max",
    "rawRangeChanged",
    "rawMin",
    "rawMax",
    "deadbandChanged",
    "deadband",
    "updateRateChanged",
    "rate",
    "alarmLimitsChanged",
    "scalingChanged",
    "scaleFactor",
    "offset",
    "unitSuffixChanged",
    "suffix",
    "valueChanged",
    "newValue",
    "valueChangedWithInfo",
    "timestamp",
    "DataQuality",
    "quality",
    "qualityChanged",
    "newQuality",
    "timestampChanged",
    "newTimestamp",
    "setValue",
    "tagName",
    "type",
    "VariableType",
    "minValue",
    "maxValue",
    "rawMinValue",
    "rawMaxValue",
    "updateRate",
    "value",
    "doubleValue",
    "boolValue",
    "intValue",
    "stringValue"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS_t {
    uint offsetsAndSizes[94];
    char stringdata0[31];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[12];
    char stringdata5[16];
    char stringdata6[5];
    char stringdata7[13];
    char stringdata8[4];
    char stringdata9[4];
    char stringdata10[16];
    char stringdata11[7];
    char stringdata12[7];
    char stringdata13[16];
    char stringdata14[9];
    char stringdata15[18];
    char stringdata16[5];
    char stringdata17[19];
    char stringdata18[15];
    char stringdata19[12];
    char stringdata20[7];
    char stringdata21[18];
    char stringdata22[7];
    char stringdata23[13];
    char stringdata24[9];
    char stringdata25[21];
    char stringdata26[10];
    char stringdata27[12];
    char stringdata28[8];
    char stringdata29[15];
    char stringdata30[11];
    char stringdata31[17];
    char stringdata32[13];
    char stringdata33[9];
    char stringdata34[8];
    char stringdata35[5];
    char stringdata36[13];
    char stringdata37[9];
    char stringdata38[9];
    char stringdata39[12];
    char stringdata40[12];
    char stringdata41[11];
    char stringdata42[6];
    char stringdata43[12];
    char stringdata44[10];
    char stringdata45[9];
    char stringdata46[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS = {
    {
        QT_MOC_LITERAL(0, 30),  // "Industrial::VariableDefinition"
        QT_MOC_LITERAL(31, 18),  // "descriptionChanged"
        QT_MOC_LITERAL(50, 0),  // ""
        QT_MOC_LITERAL(51, 11),  // "description"
        QT_MOC_LITERAL(63, 11),  // "unitChanged"
        QT_MOC_LITERAL(75, 15),  // "EngineeringUnit"
        QT_MOC_LITERAL(91, 4),  // "unit"
        QT_MOC_LITERAL(96, 12),  // "rangeChanged"
        QT_MOC_LITERAL(109, 3),  // "min"
        QT_MOC_LITERAL(113, 3),  // "max"
        QT_MOC_LITERAL(117, 15),  // "rawRangeChanged"
        QT_MOC_LITERAL(133, 6),  // "rawMin"
        QT_MOC_LITERAL(140, 6),  // "rawMax"
        QT_MOC_LITERAL(147, 15),  // "deadbandChanged"
        QT_MOC_LITERAL(163, 8),  // "deadband"
        QT_MOC_LITERAL(172, 17),  // "updateRateChanged"
        QT_MOC_LITERAL(190, 4),  // "rate"
        QT_MOC_LITERAL(195, 18),  // "alarmLimitsChanged"
        QT_MOC_LITERAL(214, 14),  // "scalingChanged"
        QT_MOC_LITERAL(229, 11),  // "scaleFactor"
        QT_MOC_LITERAL(241, 6),  // "offset"
        QT_MOC_LITERAL(248, 17),  // "unitSuffixChanged"
        QT_MOC_LITERAL(266, 6),  // "suffix"
        QT_MOC_LITERAL(273, 12),  // "valueChanged"
        QT_MOC_LITERAL(286, 8),  // "newValue"
        QT_MOC_LITERAL(295, 20),  // "valueChangedWithInfo"
        QT_MOC_LITERAL(316, 9),  // "timestamp"
        QT_MOC_LITERAL(326, 11),  // "DataQuality"
        QT_MOC_LITERAL(338, 7),  // "quality"
        QT_MOC_LITERAL(346, 14),  // "qualityChanged"
        QT_MOC_LITERAL(361, 10),  // "newQuality"
        QT_MOC_LITERAL(372, 16),  // "timestampChanged"
        QT_MOC_LITERAL(389, 12),  // "newTimestamp"
        QT_MOC_LITERAL(402, 8),  // "setValue"
        QT_MOC_LITERAL(411, 7),  // "tagName"
        QT_MOC_LITERAL(419, 4),  // "type"
        QT_MOC_LITERAL(424, 12),  // "VariableType"
        QT_MOC_LITERAL(437, 8),  // "minValue"
        QT_MOC_LITERAL(446, 8),  // "maxValue"
        QT_MOC_LITERAL(455, 11),  // "rawMinValue"
        QT_MOC_LITERAL(467, 11),  // "rawMaxValue"
        QT_MOC_LITERAL(479, 10),  // "updateRate"
        QT_MOC_LITERAL(490, 5),  // "value"
        QT_MOC_LITERAL(496, 11),  // "doubleValue"
        QT_MOC_LITERAL(508, 9),  // "boolValue"
        QT_MOC_LITERAL(518, 8),  // "intValue"
        QT_MOC_LITERAL(527, 11)   // "stringValue"
    },
    "Industrial::VariableDefinition",
    "descriptionChanged",
    "",
    "description",
    "unitChanged",
    "EngineeringUnit",
    "unit",
    "rangeChanged",
    "min",
    "max",
    "rawRangeChanged",
    "rawMin",
    "rawMax",
    "deadbandChanged",
    "deadband",
    "updateRateChanged",
    "rate",
    "alarmLimitsChanged",
    "scalingChanged",
    "scaleFactor",
    "offset",
    "unitSuffixChanged",
    "suffix",
    "valueChanged",
    "newValue",
    "valueChangedWithInfo",
    "timestamp",
    "DataQuality",
    "quality",
    "qualityChanged",
    "newQuality",
    "timestampChanged",
    "newTimestamp",
    "setValue",
    "tagName",
    "type",
    "VariableType",
    "minValue",
    "maxValue",
    "rawMinValue",
    "rawMaxValue",
    "updateRate",
    "value",
    "doubleValue",
    "boolValue",
    "intValue",
    "stringValue"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableDefinitionENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
      17,  172, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  110,    2, 0x06,   18 /* Public */,
       4,    1,  113,    2, 0x06,   20 /* Public */,
       7,    2,  116,    2, 0x06,   22 /* Public */,
      10,    2,  121,    2, 0x06,   25 /* Public */,
      13,    1,  126,    2, 0x06,   28 /* Public */,
      15,    1,  129,    2, 0x06,   30 /* Public */,
      17,    0,  132,    2, 0x06,   32 /* Public */,
      18,    2,  133,    2, 0x06,   33 /* Public */,
      21,    1,  138,    2, 0x06,   36 /* Public */,
      23,    1,  141,    2, 0x06,   38 /* Public */,
      25,    3,  144,    2, 0x06,   40 /* Public */,
      29,    1,  151,    2, 0x06,   44 /* Public */,
      31,    1,  154,    2, 0x06,   46 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      33,    3,  157,    2, 0x02,   48 /* Public */,
      33,    2,  164,    2, 0x22,   52 /* Public | MethodCloned */,
      33,    1,  169,    2, 0x22,   55 /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    8,    9,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   11,   12,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   19,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QVariant,   24,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QDateTime, 0x80000000 | 27,   24,   26,   28,
    QMetaType::Void, 0x80000000 | 27,   30,
    QMetaType::Void, QMetaType::QDateTime,   32,

 // methods: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::QDateTime, 0x80000000 | 27,   24,   26,   28,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QDateTime,   24,   26,
    QMetaType::Void, QMetaType::QVariant,   24,

 // properties: name, type, flags
      34, QMetaType::QString, 0x00015401, uint(-1), 0,
       3, QMetaType::QString, 0x00015103, uint(0), 0,
      35, 0x80000000 | 36, 0x00015409, uint(-1), 0,
       6, 0x80000000 | 5, 0x0001510b, uint(1), 0,
      37, QMetaType::Double, 0x00015103, uint(2), 0,
      38, QMetaType::Double, 0x00015103, uint(2), 0,
      39, QMetaType::Double, 0x00015103, uint(3), 0,
      40, QMetaType::Double, 0x00015103, uint(3), 0,
      14, QMetaType::Double, 0x00015103, uint(4), 0,
      41, QMetaType::Int, 0x00015103, uint(5), 0,
      42, QMetaType::QVariant, 0x00015001, uint(9), 0,
      28, 0x80000000 | 27, 0x00015009, uint(11), 0,
      26, QMetaType::QDateTime, 0x00015001, uint(12), 0,
      43, QMetaType::Double, 0x00015001, uint(9), 0,
      44, QMetaType::Bool, 0x00015001, uint(9), 0,
      45, QMetaType::Int, 0x00015001, uint(9), 0,
      46, QMetaType::QString, 0x00015001, uint(9), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::VariableDefinition::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableDefinitionENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS_t,
        // property 'tagName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'description'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'type'
        QtPrivate::TypeAndForceComplete<VariableType, std::true_type>,
        // property 'unit'
        QtPrivate::TypeAndForceComplete<EngineeringUnit, std::true_type>,
        // property 'minValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'maxValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'rawMinValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'rawMaxValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'deadband'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'updateRate'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'value'
        QtPrivate::TypeAndForceComplete<QVariant, std::true_type>,
        // property 'quality'
        QtPrivate::TypeAndForceComplete<DataQuality, std::true_type>,
        // property 'timestamp'
        QtPrivate::TypeAndForceComplete<QDateTime, std::true_type>,
        // property 'doubleValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'boolValue'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'intValue'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'stringValue'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableDefinition, std::true_type>,
        // method 'descriptionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'unitChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<EngineeringUnit, std::false_type>,
        // method 'rangeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'rawRangeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'deadbandChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateRateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'alarmLimitsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scalingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'unitSuffixChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'valueChangedWithInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'qualityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'timestampChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        // method 'setValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataQuality, std::false_type>,
        // method 'setValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDateTime &, std::false_type>,
        // method 'setValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>
    >,
    nullptr
} };

void Industrial::VariableDefinition::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableDefinition *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->descriptionChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->unitChanged((*reinterpret_cast< std::add_pointer_t<EngineeringUnit>>(_a[1]))); break;
        case 2: _t->rangeChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 3: _t->rawRangeChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 4: _t->deadbandChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 5: _t->updateRateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->alarmLimitsChanged(); break;
        case 7: _t->scalingChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 8: _t->unitSuffixChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->valueChanged((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 10: _t->valueChangedWithInfo((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[3]))); break;
        case 11: _t->qualityChanged((*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[1]))); break;
        case 12: _t->timestampChanged((*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[1]))); break;
        case 13: _t->setValue((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<DataQuality>>(_a[3]))); break;
        case 14: _t->setValue((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2]))); break;
        case 15: _t->setValue((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VariableDefinition::*)(const QString & );
            if (_t _q_method = &VariableDefinition::descriptionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(EngineeringUnit );
            if (_t _q_method = &VariableDefinition::unitChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(double , double );
            if (_t _q_method = &VariableDefinition::rangeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(double , double );
            if (_t _q_method = &VariableDefinition::rawRangeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(double );
            if (_t _q_method = &VariableDefinition::deadbandChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(int );
            if (_t _q_method = &VariableDefinition::updateRateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)();
            if (_t _q_method = &VariableDefinition::alarmLimitsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(double , double );
            if (_t _q_method = &VariableDefinition::scalingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(const QString & );
            if (_t _q_method = &VariableDefinition::unitSuffixChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(QVariant );
            if (_t _q_method = &VariableDefinition::valueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(QVariant , const QDateTime & , DataQuality );
            if (_t _q_method = &VariableDefinition::valueChangedWithInfo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(DataQuality );
            if (_t _q_method = &VariableDefinition::qualityChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (VariableDefinition::*)(const QDateTime & );
            if (_t _q_method = &VariableDefinition::timestampChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VariableDefinition *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->tagName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->description(); break;
        case 2: *reinterpret_cast< VariableType*>(_v) = _t->type(); break;
        case 3: *reinterpret_cast< EngineeringUnit*>(_v) = _t->unit(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->minValue(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->maxValue(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->rawMinValue(); break;
        case 7: *reinterpret_cast< double*>(_v) = _t->rawMaxValue(); break;
        case 8: *reinterpret_cast< double*>(_v) = _t->deadband(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->updateRate(); break;
        case 10: *reinterpret_cast< QVariant*>(_v) = _t->value(); break;
        case 11: *reinterpret_cast< DataQuality*>(_v) = _t->quality(); break;
        case 12: *reinterpret_cast< QDateTime*>(_v) = _t->timestamp(); break;
        case 13: *reinterpret_cast< double*>(_v) = _t->doubleValue(); break;
        case 14: *reinterpret_cast< bool*>(_v) = _t->boolValue(); break;
        case 15: *reinterpret_cast< int*>(_v) = _t->intValue(); break;
        case 16: *reinterpret_cast< QString*>(_v) = _t->stringValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VariableDefinition *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setDescription(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setUnit(*reinterpret_cast< EngineeringUnit*>(_v)); break;
        case 4: _t->setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setRawMinValue(*reinterpret_cast< double*>(_v)); break;
        case 7: _t->setRawMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 8: _t->setDeadband(*reinterpret_cast< double*>(_v)); break;
        case 9: _t->setUpdateRate(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Industrial::VariableDefinition::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableDefinition::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableDefinitionENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::VariableDefinition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void Industrial::VariableDefinition::descriptionChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::VariableDefinition::unitChanged(EngineeringUnit _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::VariableDefinition::rangeChanged(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::VariableDefinition::rawRangeChanged(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Industrial::VariableDefinition::deadbandChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Industrial::VariableDefinition::updateRateChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Industrial::VariableDefinition::alarmLimitsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Industrial::VariableDefinition::scalingChanged(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Industrial::VariableDefinition::unitSuffixChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Industrial::VariableDefinition::valueChanged(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Industrial::VariableDefinition::valueChangedWithInfo(QVariant _t1, const QDateTime & _t2, DataQuality _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Industrial::VariableDefinition::qualityChanged(DataQuality _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Industrial::VariableDefinition::timestampChanged(const QDateTime & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS = QtMocHelpers::stringData(
    "Industrial::UnitConversionManager"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 33)   // "Industrial::UnitConversionMan..."
    },
    "Industrial::UnitConversionManager"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject Industrial::UnitConversionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UnitConversionManager, std::true_type>
    >,
    nullptr
} };

void Industrial::UnitConversionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *Industrial::UnitConversionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::UnitConversionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEUnitConversionManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::UnitConversionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableGroup",
    "descriptionChanged",
    "",
    "description",
    "variableAdded",
    "VariableDefinition*",
    "var",
    "variableRemoved",
    "tagName",
    "alarmStatusChanged",
    "groupName"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[26];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[14];
    char stringdata5[20];
    char stringdata6[4];
    char stringdata7[16];
    char stringdata8[8];
    char stringdata9[19];
    char stringdata10[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS = {
    {
        QT_MOC_LITERAL(0, 25),  // "Industrial::VariableGroup"
        QT_MOC_LITERAL(26, 18),  // "descriptionChanged"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 11),  // "description"
        QT_MOC_LITERAL(58, 13),  // "variableAdded"
        QT_MOC_LITERAL(72, 19),  // "VariableDefinition*"
        QT_MOC_LITERAL(92, 3),  // "var"
        QT_MOC_LITERAL(96, 15),  // "variableRemoved"
        QT_MOC_LITERAL(112, 7),  // "tagName"
        QT_MOC_LITERAL(120, 18),  // "alarmStatusChanged"
        QT_MOC_LITERAL(139, 9)   // "groupName"
    },
    "Industrial::VariableGroup",
    "descriptionChanged",
    "",
    "description",
    "variableAdded",
    "VariableDefinition*",
    "var",
    "variableRemoved",
    "tagName",
    "alarmStatusChanged",
    "groupName"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableGroupENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   48, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    3 /* Public */,
       4,    1,   41,    2, 0x06,    5 /* Public */,
       7,    1,   44,    2, 0x06,    7 /* Public */,
       9,    0,   47,    2, 0x06,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::QString, 0x00015401, uint(-1), 0,
       3, QMetaType::QString, 0x00015103, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::VariableGroup::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableGroupENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS_t,
        // property 'groupName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'description'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableGroup, std::true_type>,
        // method 'descriptionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<VariableDefinition *, std::false_type>,
        // method 'variableRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'alarmStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::VariableGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableGroup *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->descriptionChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->variableAdded((*reinterpret_cast< std::add_pointer_t<VariableDefinition*>>(_a[1]))); break;
        case 2: _t->variableRemoved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->alarmStatusChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< VariableDefinition* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VariableGroup::*)(const QString & );
            if (_t _q_method = &VariableGroup::descriptionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VariableGroup::*)(VariableDefinition * );
            if (_t _q_method = &VariableGroup::variableAdded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VariableGroup::*)(const QString & );
            if (_t _q_method = &VariableGroup::variableRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VariableGroup::*)();
            if (_t _q_method = &VariableGroup::alarmStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VariableGroup *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->groupName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->description(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VariableGroup *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setDescription(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Industrial::VariableGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableGroupENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::VariableGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Industrial::VariableGroup::descriptionChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::VariableGroup::variableAdded(VariableDefinition * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::VariableGroup::variableRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::VariableGroup::alarmStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS = QtMocHelpers::stringData(
    "Industrial::PlantArea",
    "areaName",
    "areaCode"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS_t {
    uint offsetsAndSizes[6];
    char stringdata0[22];
    char stringdata1[9];
    char stringdata2[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21),  // "Industrial::PlantArea"
        QT_MOC_LITERAL(22, 8),  // "areaName"
        QT_MOC_LITERAL(31, 8)   // "areaCode"
    },
    "Industrial::PlantArea",
    "areaName",
    "areaCode"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEPlantAreaENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00015401, uint(-1), 0,
       2, QMetaType::QString, 0x00015401, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::PlantArea::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEPlantAreaENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS_t,
        // property 'areaName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'areaCode'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PlantArea, std::true_type>
    >,
    nullptr
} };

void Industrial::PlantArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<PlantArea *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->areaName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->areaCode(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *Industrial::PlantArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::PlantArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEPlantAreaENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::PlantArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
