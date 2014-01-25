/****************************************************************************
** Meta object code from reading C++ file 'qtscope.h'
**
** Created: Sat Jan 25 16:23:53 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtscope.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtscope.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTScope[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,    8,    8,    8, 0x08,
      53,    8,    8,    8, 0x08,
      61,    8,    8,    8, 0x08,
      71,    8,    8,    8, 0x08,
      96,    8,    8,    8, 0x08,
     119,    8,    8,    8, 0x08,
     137,    8,    8,    8, 0x08,
     152,    8,    8,    8, 0x08,
     167,    8,    8,    8, 0x08,
     182,    8,    8,    8, 0x08,
     199,    8,    8,    8, 0x08,
     215,    8,    8,    8, 0x08,
     237,    8,    8,    8, 0x08,
     260,    8,    8,    8, 0x0a,
     287,  283,    8,    8, 0x0a,
     318,  313,    8,    8, 0x0a,
     362,    8,    8,    8, 0x0a,
     375,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTScope[] = {
    "QTScope\0\0signalSamplingRateChanged(double)\0"
    "newView()\0about()\0aboutQt()\0"
    "windowsMenuAboutToShow()\0"
    "windowsMenuActivated()\0slotReplotPlots()\0"
    "slotReadData()\0tileVertical()\0"
    "saveSettings()\0loadPluginPath()\0"
    "slotConfigure()\0slotDisplaySettings()\0"
    "slotSamplingSettings()\0slotChannelClosed(int)\0"
    "c,b\0slotSetIntervals(int,int)\0f,,,\0"
    "slotSetSamplingSettings(double,int,int,int)\0"
    "slotSlower()\0slotFaster()\0"
};

void QTScope::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTScope *_t = static_cast<QTScope *>(_o);
        switch (_id) {
        case 0: _t->signalSamplingRateChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->newView(); break;
        case 2: _t->about(); break;
        case 3: _t->aboutQt(); break;
        case 4: _t->windowsMenuAboutToShow(); break;
        case 5: _t->windowsMenuActivated(); break;
        case 6: _t->slotReplotPlots(); break;
        case 7: _t->slotReadData(); break;
        case 8: _t->tileVertical(); break;
        case 9: _t->saveSettings(); break;
        case 10: _t->loadPluginPath(); break;
        case 11: _t->slotConfigure(); break;
        case 12: _t->slotDisplaySettings(); break;
        case 13: _t->slotSamplingSettings(); break;
        case 14: _t->slotChannelClosed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->slotSetIntervals((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->slotSetSamplingSettings((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 17: _t->slotSlower(); break;
        case 18: _t->slotFaster(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTScope::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTScope::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QTScope,
      qt_meta_data_QTScope, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTScope::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTScope::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTScope::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTScope))
        return static_cast<void*>(const_cast< QTScope*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QTScope::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void QTScope::signalSamplingRateChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
