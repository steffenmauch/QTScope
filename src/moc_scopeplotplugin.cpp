/****************************************************************************
** Meta object code from reading C++ file 'scopeplotplugin.h'
**
** Created: Sat Jan 25 16:04:07 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "scopeplotplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scopeplotplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_scopePlotPlugin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   35,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_scopePlotPlugin[] = {
    "scopePlotPlugin\0\0signalClosed(int)\0f\0"
    "slotSetSamplingRate(double)\0"
};

void scopePlotPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        scopePlotPlugin *_t = static_cast<scopePlotPlugin *>(_o);
        switch (_id) {
        case 0: _t->signalClosed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotSetSamplingRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData scopePlotPlugin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject scopePlotPlugin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_scopePlotPlugin,
      qt_meta_data_scopePlotPlugin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &scopePlotPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *scopePlotPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *scopePlotPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_scopePlotPlugin))
        return static_cast<void*>(const_cast< scopePlotPlugin*>(this));
    return QWidget::qt_metacast(_clname);
}

int scopePlotPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void scopePlotPlugin::signalClosed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
