/****************************************************************************
** DataPlot meta object code from reading C++ file 'dataplot.h'
**
** Created: Fri Apr 2 13:12:15 2004
**      by: The Qt MOC ($Id: moc_dataplot.cpp,v 1.1.1.1 2005/02/05 01:20:07 bp1 Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "dataplot.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DataPlot::className() const
{
    return "DataPlot";
}

QMetaObject *DataPlot::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DataPlot( "DataPlot", &DataPlot::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DataPlot::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DataPlot", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DataPlot::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DataPlot", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DataPlot::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotXrangeChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotYmaxChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotYminChanged", 1, param_slot_2 };
    static const QUMethod slot_3 = {"slotAutoscaleToggled", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotXrangeChanged(double)", &slot_0, QMetaData::Private },
	{ "slotYmaxChanged(double)", &slot_1, QMetaData::Private },
	{ "slotYminChanged(double)", &slot_2, QMetaData::Private },
	{ "slotAutoscaleToggled()", &slot_3, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"message", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "message(const QString&,int)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"DataPlot", parentObject,
	slot_tbl, 4,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DataPlot.setMetaObject( metaObj );
    return metaObj;
}

void* DataPlot::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DataPlot" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL message
void DataPlot::message( const QString& t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_QString.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

bool DataPlot::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotXrangeChanged((double)static_QUType_double.get(_o+1)); break;
    case 1: slotYmaxChanged((double)static_QUType_double.get(_o+1)); break;
    case 2: slotYminChanged((double)static_QUType_double.get(_o+1)); break;
    case 3: slotAutoscaleToggled(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DataPlot::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: message((const QString&)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QMainWindow::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DataPlot::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool DataPlot::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
