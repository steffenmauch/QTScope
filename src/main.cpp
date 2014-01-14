#include <qapplication.h>
#include <qsettings.h>
#include "qtscope.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QTScope * mw = new QTScope();

    mw->setCaption( "QTScope" );
    mw->show();
    
    //qApp->setStyleSheet("QLineEdit { background-color: yellow; }");
    //QApplication::setStyle("cleanlooks");
    QApplication::setStyle("plastique");
    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
