/****************************************************************************
** ChooserWindow meta object code from reading C++ file 'chooserwindow.h'
**
** Created: Tue Apr 6 14:10:28 2004
**      by: The Qt MOC ($Id: moc_chooserwindow.cpp,v 1.1.1.1 2005/02/05 01:20:07 bp1 Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "chooserwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ChooserWindow::className() const
{
    return "ChooserWindow";
}

QMetaObject *ChooserWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ChooserWindow( "ChooserWindow", &ChooserWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ChooserWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ChooserWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ChooserWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ChooserWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ChooserWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ChooserWindow", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ChooserWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ChooserWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ChooserWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool ChooserWindow::qt_invoke( int _id, QUObject* _o )
{
    return QMainWindow::qt_invoke(_id,_o);
}

bool ChooserWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ChooserWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool ChooserWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
