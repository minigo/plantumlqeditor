/****************************************************************************
** Meta object code from reading C++ file 'preferencesdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "preferencesdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'preferencesdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PreferencesDialog_t {
    QByteArrayData data[13];
    char stringdata[340];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PreferencesDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PreferencesDialog_t qt_meta_stringdata_PreferencesDialog = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 10),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 32),
QT_MOC_LITERAL(4, 63, 31),
QT_MOC_LITERAL(5, 95, 32),
QT_MOC_LITERAL(6, 128, 31),
QT_MOC_LITERAL(7, 160, 29),
QT_MOC_LITERAL(8, 190, 31),
QT_MOC_LITERAL(9, 222, 30),
QT_MOC_LITERAL(10, 253, 29),
QT_MOC_LITERAL(11, 283, 28),
QT_MOC_LITERAL(12, 312, 27)
    },
    "PreferencesDialog\0onRejected\0\0"
    "on_customJavaPathEdit_textEdited\0"
    "on_customJavaPathButton_clicked\0"
    "on_customPlantUmlEdit_textEdited\0"
    "on_customPlantUmlButton_clicked\0"
    "on_assistantXmlButton_clicked\0"
    "on_customGraphizEdit_textEdited\0"
    "on_customGraphizButton_clicked\0"
    "on_customCacheEdit_textEdited\0"
    "on_customCacheButton_clicked\0"
    "on_clearCacheButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PreferencesDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    1,   70,    2, 0x08 /* Private */,
       4,    0,   73,    2, 0x08 /* Private */,
       5,    1,   74,    2, 0x08 /* Private */,
       6,    0,   77,    2, 0x08 /* Private */,
       7,    0,   78,    2, 0x08 /* Private */,
       8,    1,   79,    2, 0x08 /* Private */,
       9,    0,   82,    2, 0x08 /* Private */,
      10,    1,   83,    2, 0x08 /* Private */,
      11,    0,   86,    2, 0x08 /* Private */,
      12,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PreferencesDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PreferencesDialog *_t = static_cast<PreferencesDialog *>(_o);
        switch (_id) {
        case 0: _t->onRejected(); break;
        case 1: _t->on_customJavaPathEdit_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_customJavaPathButton_clicked(); break;
        case 3: _t->on_customPlantUmlEdit_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->on_customPlantUmlButton_clicked(); break;
        case 5: _t->on_assistantXmlButton_clicked(); break;
        case 6: _t->on_customGraphizEdit_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_customGraphizButton_clicked(); break;
        case 8: _t->on_customCacheEdit_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->on_customCacheButton_clicked(); break;
        case 10: _t->on_clearCacheButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject PreferencesDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PreferencesDialog.data,
      qt_meta_data_PreferencesDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *PreferencesDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PreferencesDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PreferencesDialog.stringdata))
        return static_cast<void*>(const_cast< PreferencesDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PreferencesDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
