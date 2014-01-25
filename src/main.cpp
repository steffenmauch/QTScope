#include <qapplication.h>
#include <qsettings.h>
#include "qtscope.h"

int main( int argc, char ** argv )
{
    QCoreApplication::setOrganizationName("Comedi-GUI");
    QCoreApplication::setApplicationName("QTScope");
    QCoreApplication::setOrganizationDomain("https://github.com/steffenmauch/QTScope");

    QApplication a( argc, argv );
    QTScope * mw = new QTScope();

    //qApp->setStyleSheet("QSpinBox { min-width: 20px; \
    //                    min-height: 20px; }");
    mw->setWindowTitle( "QTScope" );
    mw->show();

    //qApp->setStyleSheet("QLineEdit { background-color: yellow; }");
    //QApplication::setStyle("cleanlooks");
    QApplication::setStyle("plastique");
    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
