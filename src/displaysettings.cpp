/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *                                                                         *
 *   porting to QT4 + improvements                                         *
 *   Copyright (C) 2014 by Steffen Mauch                                   *
 *   steffen.mauch@gmail.com                                               *
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

#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>

displaySettings::displaySettings( QWidget* , const char* , int contRate, int burstRate )
{
  setWindowTitle( tr("Display Settings") );

  setMinimumSize( QSize( 169, 148 ) );

  frame5Layout = new QVBoxLayout();

  tabs = new QTabWidget( );

  tab = new QWidget( );
  tabLayout = new QHBoxLayout();
  tab->setLayout(tabLayout);

  groupBox1 = new QGroupBox();
  tabLayout->addWidget(groupBox1);
  groupBox1Layout = new QGridLayout();
  groupBox1->setLayout( groupBox1Layout );
  groupBox1Layout->setAlignment( Qt::AlignTop );

  textLabel1 = new QLabel();
  textLabel1->setAlignment( Qt::AlignVCenter | Qt::AlignRight );
  groupBox1Layout->addWidget( textLabel1, 0, 0 );

  continousSpeedBox = new QSpinBox();
  continousSpeedBox->setButtonSymbols( QSpinBox::PlusMinus );
  continousSpeedBox->setMaximum( 50 );
  continousSpeedBox->setMinimum( 1 );
  continousSpeedBox->setValue( (int)(1000/contRate) );
  continousSpeedBox->setSuffix( tr( " Hz" ) );
  groupBox1Layout->addWidget( continousSpeedBox, 0, 1 );

  burstSpeedBox = new QSpinBox();
  burstSpeedBox->setMaximum( 50 );
  burstSpeedBox->setMinimum( 1 );
  burstSpeedBox->setValue( (int)(1000/burstRate) );
  burstSpeedBox->setPrefix( QString::null );
  burstSpeedBox->setSuffix( tr( " Hz" ) );
  burstSpeedBox->setSpecialValueText( QString::null );
  groupBox1Layout->addWidget( burstSpeedBox, 1, 1 );

  textLabel2 = new QLabel();
  textLabel2->setAlignment( Qt::AlignVCenter | Qt::AlignRight );
  groupBox1Layout->addWidget( textLabel2, 1, 0 );

  tabLayout->addWidget( groupBox1 );
  tabs->insertTab( 0, tab, tr("Refresh") );
  frame5Layout->addWidget( tabs );

  applyButton = new QPushButton();
  applyButton->setAutoDefault( TRUE );
  applyButton->setDefault( TRUE );
  applyButton->setFlat( FALSE );
  frame5Layout->addWidget( applyButton );

  groupBox1->setTitle( tr( "Refresh Rates" ) );
  textLabel1->setText( tr( "Continous" ) );
  textLabel2->setText( tr( "Burst" ) );
  applyButton->setText( tr( "Apply" ) );

  setLayout( frame5Layout );

  connect( applyButton, SIGNAL( clicked() ), this, SLOT( slotApply() ) );
}

displaySettings::~displaySettings(){

}

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

