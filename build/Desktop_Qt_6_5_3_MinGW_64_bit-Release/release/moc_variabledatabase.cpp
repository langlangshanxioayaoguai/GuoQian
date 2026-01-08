/****************************************************************************
** Meta object code from reading C++ file 'variabledatabase.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../industrial/variabledatabase.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'variabledatabase.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableDatabase",
    "databaseChanged",
    "",
    "variableSaved",
    "tagName",
    "variableDeleted",
    "importCompleted",
    "successCount",
    "failCount"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[29];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[8];
    char stringdata5[16];
    char stringdata6[16];
    char stringdata7[13];
    char stringdata8[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS = {
    {
        QT_MOC_LITERAL(0, 28),  // "Industrial::VariableDatabase"
        QT_MOC_LITERAL(29, 15),  // "databaseChanged"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 13),  // "variableSaved"
        QT_MOC_LITERAL(60, 7),  // "tagName"
        QT_MOC_LITERAL(68, 15),  // "variableDeleted"
        QT_MOC_LITERAL(84, 15),  // "importCompleted"
        QT_MOC_LITERAL(100, 12),  // "successCount"
        QT_MOC_LITERAL(113, 9)   // "failCount"
    },
    "Industrial::VariableDatabase",
    "databaseChanged",
    "",
    "variableSaved",
    "tagName",
    "variableDeleted",
    "importCompleted",
    "successCount",
    "failCount"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableDatabaseENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    1 /* Public */,
       3,    1,   39,    2, 0x06,    2 /* Public */,
       5,    1,   42,    2, 0x06,    4 /* Public */,
       6,    2,   45,    2, 0x06,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::VariableDatabase::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableDatabaseENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableDatabase, std::true_type>,
        // method 'databaseChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'variableSaved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'importCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void Industrial::VariableDatabase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableDatabase *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->databaseChanged(); break;
        case 1: _t->variableSaved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->variableDeleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->importCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VariableDatabase::*)();
            if (_t _q_method = &VariableDatabase::databaseChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VariableDatabase::*)(const QString & );
            if (_t _q_method = &VariableDatabase::variableSaved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VariableDatabase::*)(const QString & );
            if (_t _q_method = &VariableDatabase::variableDeleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VariableDatabase::*)(int , int );
            if (_t _q_method = &VariableDatabase::importCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::VariableDatabase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableDatabase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableDatabaseENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::VariableDatabase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Industrial::VariableDatabase::databaseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Industrial::VariableDatabase::variableSaved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::VariableDatabase::variableDeleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::VariableDatabase::importCompleted(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableDBManager"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 29)   // "Industrial::VariableDBManager"
    },
    "Industrial::VariableDBManager"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableDBManagerENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject Industrial::VariableDBManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableDBManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableDBManager, std::true_type>
    >,
    nullptr
} };

void Industrial::VariableDBManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *Industrial::VariableDBManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableDBManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableDBManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Industrial::VariableDBManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
