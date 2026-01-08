/****************************************************************************
** Meta object code from reading C++ file 'variableconfigtool.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../industrial/variableconfigtool.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'variableconfigtool.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableConfigDialog",
    "variableSaved",
    "",
    "VariableDefinition*",
    "var",
    "variableDeleted",
    "tagName",
    "onSaveClicked",
    "onDeleteClicked",
    "onTestClicked",
    "onAddressChanged",
    "onTypeChanged",
    "index"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS_t {
    uint offsetsAndSizes[26];
    char stringdata0[33];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[4];
    char stringdata5[16];
    char stringdata6[8];
    char stringdata7[14];
    char stringdata8[16];
    char stringdata9[14];
    char stringdata10[17];
    char stringdata11[14];
    char stringdata12[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS = {
    {
        QT_MOC_LITERAL(0, 32),  // "Industrial::VariableConfigDialog"
        QT_MOC_LITERAL(33, 13),  // "variableSaved"
        QT_MOC_LITERAL(47, 0),  // ""
        QT_MOC_LITERAL(48, 19),  // "VariableDefinition*"
        QT_MOC_LITERAL(68, 3),  // "var"
        QT_MOC_LITERAL(72, 15),  // "variableDeleted"
        QT_MOC_LITERAL(88, 7),  // "tagName"
        QT_MOC_LITERAL(96, 13),  // "onSaveClicked"
        QT_MOC_LITERAL(110, 15),  // "onDeleteClicked"
        QT_MOC_LITERAL(126, 13),  // "onTestClicked"
        QT_MOC_LITERAL(140, 16),  // "onAddressChanged"
        QT_MOC_LITERAL(157, 13),  // "onTypeChanged"
        QT_MOC_LITERAL(171, 5)   // "index"
    },
    "Industrial::VariableConfigDialog",
    "variableSaved",
    "",
    "VariableDefinition*",
    "var",
    "variableDeleted",
    "tagName",
    "onSaveClicked",
    "onDeleteClicked",
    "onTestClicked",
    "onAddressChanged",
    "onTypeChanged",
    "index"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       5,    1,   59,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   62,    2, 0x08,    5 /* Private */,
       8,    0,   63,    2, 0x08,    6 /* Private */,
       9,    0,   64,    2, 0x08,    7 /* Private */,
      10,    0,   65,    2, 0x08,    8 /* Private */,
      11,    1,   66,    2, 0x08,    9 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::VariableConfigDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableConfigDialog, std::true_type>,
        // method 'variableSaved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<VariableDefinition *, std::false_type>,
        // method 'variableDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSaveClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTestClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTypeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void Industrial::VariableConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableConfigDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->variableSaved((*reinterpret_cast< std::add_pointer_t<VariableDefinition*>>(_a[1]))); break;
        case 1: _t->variableDeleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->onSaveClicked(); break;
        case 3: _t->onDeleteClicked(); break;
        case 4: _t->onTestClicked(); break;
        case 5: _t->onAddressChanged(); break;
        case 6: _t->onTypeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
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
            using _t = void (VariableConfigDialog::*)(VariableDefinition * );
            if (_t _q_method = &VariableConfigDialog::variableSaved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VariableConfigDialog::*)(const QString & );
            if (_t _q_method = &VariableConfigDialog::variableDeleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::VariableConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableConfigDialogENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Industrial::VariableConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Industrial::VariableConfigDialog::variableSaved(VariableDefinition * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::VariableConfigDialog::variableDeleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS = QtMocHelpers::stringData(
    "Industrial::VariableBrowser",
    "variableSelected",
    "",
    "tagName",
    "variableDoubleClicked",
    "configureVariable",
    "monitorVariable",
    "onTreeItemClicked",
    "QTreeWidgetItem*",
    "item",
    "column",
    "onTreeItemDoubleClicked",
    "onTableItemDoubleClicked",
    "row",
    "onContextMenuRequested",
    "pos",
    "onConfigureAction",
    "onMonitorAction",
    "onExportAction",
    "onImportAction",
    "onFilterChanged",
    "onSearchTextChanged",
    "text",
    "updateVariableStatus"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS_t {
    uint offsetsAndSizes[48];
    char stringdata0[28];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[22];
    char stringdata5[18];
    char stringdata6[16];
    char stringdata7[18];
    char stringdata8[17];
    char stringdata9[5];
    char stringdata10[7];
    char stringdata11[24];
    char stringdata12[25];
    char stringdata13[4];
    char stringdata14[23];
    char stringdata15[4];
    char stringdata16[18];
    char stringdata17[16];
    char stringdata18[15];
    char stringdata19[15];
    char stringdata20[16];
    char stringdata21[20];
    char stringdata22[5];
    char stringdata23[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS = {
    {
        QT_MOC_LITERAL(0, 27),  // "Industrial::VariableBrowser"
        QT_MOC_LITERAL(28, 16),  // "variableSelected"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 7),  // "tagName"
        QT_MOC_LITERAL(54, 21),  // "variableDoubleClicked"
        QT_MOC_LITERAL(76, 17),  // "configureVariable"
        QT_MOC_LITERAL(94, 15),  // "monitorVariable"
        QT_MOC_LITERAL(110, 17),  // "onTreeItemClicked"
        QT_MOC_LITERAL(128, 16),  // "QTreeWidgetItem*"
        QT_MOC_LITERAL(145, 4),  // "item"
        QT_MOC_LITERAL(150, 6),  // "column"
        QT_MOC_LITERAL(157, 23),  // "onTreeItemDoubleClicked"
        QT_MOC_LITERAL(181, 24),  // "onTableItemDoubleClicked"
        QT_MOC_LITERAL(206, 3),  // "row"
        QT_MOC_LITERAL(210, 22),  // "onContextMenuRequested"
        QT_MOC_LITERAL(233, 3),  // "pos"
        QT_MOC_LITERAL(237, 17),  // "onConfigureAction"
        QT_MOC_LITERAL(255, 15),  // "onMonitorAction"
        QT_MOC_LITERAL(271, 14),  // "onExportAction"
        QT_MOC_LITERAL(286, 14),  // "onImportAction"
        QT_MOC_LITERAL(301, 15),  // "onFilterChanged"
        QT_MOC_LITERAL(317, 19),  // "onSearchTextChanged"
        QT_MOC_LITERAL(337, 4),  // "text"
        QT_MOC_LITERAL(342, 20)   // "updateVariableStatus"
    },
    "Industrial::VariableBrowser",
    "variableSelected",
    "",
    "tagName",
    "variableDoubleClicked",
    "configureVariable",
    "monitorVariable",
    "onTreeItemClicked",
    "QTreeWidgetItem*",
    "item",
    "column",
    "onTreeItemDoubleClicked",
    "onTableItemDoubleClicked",
    "row",
    "onContextMenuRequested",
    "pos",
    "onConfigureAction",
    "onMonitorAction",
    "onExportAction",
    "onImportAction",
    "onFilterChanged",
    "onSearchTextChanged",
    "text",
    "updateVariableStatus"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEVariableBrowserENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  104,    2, 0x06,    1 /* Public */,
       4,    1,  107,    2, 0x06,    3 /* Public */,
       5,    1,  110,    2, 0x06,    5 /* Public */,
       6,    1,  113,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    2,  116,    2, 0x08,    9 /* Private */,
      11,    2,  121,    2, 0x08,   12 /* Private */,
      12,    2,  126,    2, 0x08,   15 /* Private */,
      14,    1,  131,    2, 0x08,   18 /* Private */,
      16,    0,  134,    2, 0x08,   20 /* Private */,
      17,    0,  135,    2, 0x08,   21 /* Private */,
      18,    0,  136,    2, 0x08,   22 /* Private */,
      19,    0,  137,    2, 0x08,   23 /* Private */,
      20,    0,  138,    2, 0x08,   24 /* Private */,
      21,    1,  139,    2, 0x08,   25 /* Private */,
      23,    0,  142,    2, 0x08,   27 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   10,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   13,   10,
    QMetaType::Void, QMetaType::QPoint,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::VariableBrowser::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEVariableBrowserENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VariableBrowser, std::true_type>,
        // method 'variableSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'variableDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'configureVariable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'monitorVariable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTreeItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTreeItemDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTableItemDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onContextMenuRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onConfigureAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onMonitorAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onImportAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFilterChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateVariableStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::VariableBrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableBrowser *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->variableSelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->variableDoubleClicked((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->configureVariable((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->monitorVariable((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onTreeItemClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->onTreeItemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->onTableItemDoubleClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->onContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 8: _t->onConfigureAction(); break;
        case 9: _t->onMonitorAction(); break;
        case 10: _t->onExportAction(); break;
        case 11: _t->onImportAction(); break;
        case 12: _t->onFilterChanged(); break;
        case 13: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->updateVariableStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VariableBrowser::*)(const QString & );
            if (_t _q_method = &VariableBrowser::variableSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VariableBrowser::*)(const QString & );
            if (_t _q_method = &VariableBrowser::variableDoubleClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VariableBrowser::*)(const QString & );
            if (_t _q_method = &VariableBrowser::configureVariable; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VariableBrowser::*)(const QString & );
            if (_t _q_method = &VariableBrowser::monitorVariable; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *Industrial::VariableBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::VariableBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEVariableBrowserENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Industrial::VariableBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Industrial::VariableBrowser::variableSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Industrial::VariableBrowser::variableDoubleClicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Industrial::VariableBrowser::configureVariable(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Industrial::VariableBrowser::monitorVariable(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS = QtMocHelpers::stringData(
    "Industrial::BatchConfigTool",
    "importFromExcel",
    "",
    "filename",
    "exportToExcel",
    "generateFromTemplate",
    "templateName",
    "validateConfigurations",
    "applyChanges",
    "rollbackChanges",
    "onImportClicked",
    "onExportClicked",
    "onValidateClicked",
    "onApplyClicked",
    "onRollbackClicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[28];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[14];
    char stringdata5[21];
    char stringdata6[13];
    char stringdata7[23];
    char stringdata8[13];
    char stringdata9[16];
    char stringdata10[16];
    char stringdata11[16];
    char stringdata12[18];
    char stringdata13[15];
    char stringdata14[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS_t qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS = {
    {
        QT_MOC_LITERAL(0, 27),  // "Industrial::BatchConfigTool"
        QT_MOC_LITERAL(28, 15),  // "importFromExcel"
        QT_MOC_LITERAL(44, 0),  // ""
        QT_MOC_LITERAL(45, 8),  // "filename"
        QT_MOC_LITERAL(54, 13),  // "exportToExcel"
        QT_MOC_LITERAL(68, 20),  // "generateFromTemplate"
        QT_MOC_LITERAL(89, 12),  // "templateName"
        QT_MOC_LITERAL(102, 22),  // "validateConfigurations"
        QT_MOC_LITERAL(125, 12),  // "applyChanges"
        QT_MOC_LITERAL(138, 15),  // "rollbackChanges"
        QT_MOC_LITERAL(154, 15),  // "onImportClicked"
        QT_MOC_LITERAL(170, 15),  // "onExportClicked"
        QT_MOC_LITERAL(186, 17),  // "onValidateClicked"
        QT_MOC_LITERAL(204, 14),  // "onApplyClicked"
        QT_MOC_LITERAL(219, 17)   // "onRollbackClicked"
    },
    "Industrial::BatchConfigTool",
    "importFromExcel",
    "",
    "filename",
    "exportToExcel",
    "generateFromTemplate",
    "templateName",
    "validateConfigurations",
    "applyChanges",
    "rollbackChanges",
    "onImportClicked",
    "onExportClicked",
    "onValidateClicked",
    "onApplyClicked",
    "onRollbackClicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndustrialSCOPEBatchConfigToolENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x0a,    1 /* Public */,
       4,    1,   83,    2, 0x0a,    3 /* Public */,
       5,    1,   86,    2, 0x0a,    5 /* Public */,
       7,    0,   89,    2, 0x0a,    7 /* Public */,
       8,    0,   90,    2, 0x0a,    8 /* Public */,
       9,    0,   91,    2, 0x0a,    9 /* Public */,
      10,    0,   92,    2, 0x08,   10 /* Private */,
      11,    0,   93,    2, 0x08,   11 /* Private */,
      12,    0,   94,    2, 0x08,   12 /* Private */,
      13,    0,   95,    2, 0x08,   13 /* Private */,
      14,    0,   96,    2, 0x08,   14 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Industrial::BatchConfigTool::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndustrialSCOPEBatchConfigToolENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BatchConfigTool, std::true_type>,
        // method 'importFromExcel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'exportToExcel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'generateFromTemplate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'validateConfigurations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'applyChanges'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rollbackChanges'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onImportClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onValidateClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onApplyClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRollbackClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Industrial::BatchConfigTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BatchConfigTool *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->importFromExcel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->exportToExcel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->generateFromTemplate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->validateConfigurations(); break;
        case 4: _t->applyChanges(); break;
        case 5: _t->rollbackChanges(); break;
        case 6: _t->onImportClicked(); break;
        case 7: _t->onExportClicked(); break;
        case 8: _t->onValidateClicked(); break;
        case 9: _t->onApplyClicked(); break;
        case 10: _t->onRollbackClicked(); break;
        default: ;
        }
    }
}

const QMetaObject *Industrial::BatchConfigTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Industrial::BatchConfigTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndustrialSCOPEBatchConfigToolENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Industrial::BatchConfigTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
