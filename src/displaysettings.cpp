/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "displaysettings.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qsizepolicy.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <QCloseEvent>


displaySettings::displaySettings( QWidget* parent, const char* name, int contRate, int burstRate )
    : QDockWidget(name, parent, Qt::WDestructiveClose )
{
  setWindowTitle( tr("Display Settings") );

  setMinimumSize( QSize( 169, 148 ) );

  frame5 = new QFrame( this, "frame5" );
  frame5->setFrameShape( QFrame::StyledPanel );
  frame5->setFrameShadow( QFrame::Raised );
  frame5Layout = new Q3VBoxLayout( frame5, 0, 0, "frame5Layout");

  tabs = new QTabWidget( frame5, "tabs" );

  tab = new QWidget( tabs, "tab" );
  tabLayout = new Q3HBoxLayout( tab, 0, 0, "tabLayout");

  groupBox1 = new Q3GroupBox( tab, "groupBox1" );
  groupBox1->setColumnLayout(0, Qt::Vertical );
  groupBox1->layout()->setSpacing( 4 );
  groupBox1->layout()->setMargin( 4 );
  groupBox1Layout = new Q3GridLayout( groupBox1->layout() );
  groupBox1Layout->setAlignment( Qt::AlignTop );

  textLabel1 = new QLabel( groupBox1, "textLabel1" );
  textLabel1->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );

  groupBox1Layout->addWidget( textLabel1, 0, 0 );

  continousSpeedBox = new QSpinBox( groupBox1, "readOutSpeedBox" );
  continousSpeedBox->setButtonSymbols( QSpinBox::PlusMinus );
  continousSpeedBox->setMaxValue( 50 );
  continousSpeedBox->setMinValue( 1 );
  continousSpeedBox->setValue( (int)(1000/contRate) );
  continousSpeedBox->setSuffix( tr( " Hz" ) );

  groupBox1Layout->addWidget( continousSpeedBox, 0, 1 );

  burstSpeedBox = new QSpinBox( groupBox1, "refreshSpeedBox" );
  burstSpeedBox->setMaxValue( 50 );
  burstSpeedBox->setMinValue( 1 );
  burstSpeedBox->setValue( (int)(1000/burstRate) );
  burstSpeedBox->setPrefix( QString::null );
  burstSpeedBox->setSuffix( tr( " Hz" ) );
  burstSpeedBox->setSpecialValueText( QString::null );

  groupBox1Layout->addWidget( burstSpeedBox, 1, 1 );

  textLabel2 = new QLabel( groupBox1, "textLabel2" );
  textLabel2->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );

  groupBox1Layout->addWidget( textLabel2, 1, 0 );
  tabLayout->addWidget( groupBox1 );
  tabs->insertTab( tab, QString("") );
  frame5Layout->addWidget( tabs );

  applyButton = new QPushButton( frame5, "applyButton" );
  applyButton->setAutoDefault( TRUE );
  applyButton->setDefault( TRUE );
  applyButton->setFlat( FALSE );
  frame5Layout->addWidget( applyButton );

  groupBox1->setTitle( tr( "Refresh Rates" ) );
  textLabel1->setText( tr( "Continous" ) );
  textLabel2->setText( tr( "Burst" ) );
  tabs->changeTab( tab, tr( "Refresh" ) );
  applyButton->setText( tr( "Apply" ) );

  resize( QSize(169, 150).expandedTo(minimumSizeHint()) );
  setMaximumSize(size());
  //clearWState( WState_Polished );
  
  setWidget( frame5 );

  connect( applyButton, SIGNAL( clicked() ), this, SLOT( slotApply() ) );
}

displaySettings::~displaySettings()
{}

void displaySettings::closeEvent( QCloseEvent* ce )
{
  ce->accept();
  return;
}

void displaySettings::slotApply()
{
	emit signalIntervalsChanged( (int)(1000/continousSpeedBox->value()),
				     (int)(1000/burstSpeedBox->value()) );
}

