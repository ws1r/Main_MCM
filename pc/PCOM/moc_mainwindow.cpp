/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Dec 7 17:17:25 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      39,   11,   11,   11, 0x08,
      65,   11,   11,   11, 0x08,
      89,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     143,   11,   11,   11, 0x08,
     165,   11,   11,   11, 0x08,
     186,   11,   11,   11, 0x08,
     213,   11,   11,   11, 0x08,
     238,   11,   11,   11, 0x08,
     263,   11,   11,   11, 0x08,
     284,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0CheckMachineStatFunction()\0"
    "RegisterSettingFunction()\0"
    "EepromSettingFunction()\0"
    "SendGprsMessageFunction()\0"
    "SendZigbeeMessageFunction()\0"
    "BeepSettingFunction()\0LedSettingFunction()\0"
    "HardWareTestSelfFunction()\0"
    "SoftWareUpdateFunction()\0"
    "UpdateWallTimeFunction()\0TimeAdjustFunction()\0"
    "ReConnectFunction()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->CheckMachineStatFunction(); break;
        case 1: _t->RegisterSettingFunction(); break;
        case 2: _t->EepromSettingFunction(); break;
        case 3: _t->SendGprsMessageFunction(); break;
        case 4: _t->SendZigbeeMessageFunction(); break;
        case 5: _t->BeepSettingFunction(); break;
        case 6: _t->LedSettingFunction(); break;
        case 7: _t->HardWareTestSelfFunction(); break;
        case 8: _t->SoftWareUpdateFunction(); break;
        case 9: _t->UpdateWallTimeFunction(); break;
        case 10: _t->TimeAdjustFunction(); break;
        case 11: _t->ReConnectFunction(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
