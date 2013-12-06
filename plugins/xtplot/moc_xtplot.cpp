/****************************************************************************
** Meta object code from reading C++ file 'xtplot.h'
**
** Created: Fri Dec 6 10:24:55 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "xtplot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xtplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_xtPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    8,    7,    7, 0x0a,
      38,    7,    7,    7, 0x08,
      62,    7,    7,    7, 0x08,
      86,    7,    7,    7, 0x08,
     109,    7,    7,    7, 0x08,
     122,    7,    7,    7, 0x08,
     135,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_xtPlot[] = {
    "xtPlot\0\0f\0slotSetSamplingRate(double)\0"
    "slotYmaxChanged(double)\0slotYminChanged(double)\0"
    "slotAutoscaleToggled()\0incTbEvent()\0"
    "decTbEvent()\0enterFileName()\0"
};

void xtPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        xtPlot *_t = static_cast<xtPlot *>(_o);
        switch (_id) {
        case 0: _t->slotSetSamplingRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->slotYmaxChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->slotYminChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->slotAutoscaleToggled(); break;
        case 4: _t->incTbEvent(); break;
        case 5: _t->decTbEvent(); break;
        case 6: _t->enterFileName(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData xtPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject xtPlot::staticMetaObject = {
    { &scopePlotPlugin::staticMetaObject, qt_meta_stringdata_xtPlot,
      qt_meta_data_xtPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &xtPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *xtPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *xtPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_xtPlot))
        return static_cast<void*>(const_cast< xtPlot*>(this));
    return scopePlotPlugin::qt_metacast(_clname);
}

int xtPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = scopePlotPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
