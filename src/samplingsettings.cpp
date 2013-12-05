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

#include "samplingsettings.h"

#include <qvariant.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qsizepolicy.h>


samplingSettings::samplingSettings( QWidget* parent, const char* name, double freq, int numCh,int nSamples,int continous)
    : QMainWindow( parent, name, WDestructiveClose )
{
  setCaption( tr( "Sampling Settings" ) );
  //setMinimumSize( QSize( 169, 148 ) );

  setCentralWidget( new QWidget( this, "qt_central_widget" ) );

  frame5 = new QFrame( centralWidget(), "frame5" );
  frame5->setFrameShape( QFrame::StyledPanel );
  frame5->setFrameShadow( QFrame::Raised );
  frame5Layout = new QVBoxLayout( frame5, 0, 0, "frame5Layout");

  testMainWindowLayout = new QVBoxLayout( centralWidget(), 0, 0, "testMainWindowLayout");
  testMainWindowLayout->addWidget( frame5 );

  groupBox1 = new QGroupBox( frame5, "groupBox1" );
  groupBox1->setColumnLayout(0, Qt::Vertical );
  groupBox1->layout()->setSpacing( 4 );
  groupBox1->layout()->setMargin( 4 );
  groupBox1Layout = new QGridLayout( groupBox1->layout() );
  groupBox1Layout->setAlignment( Qt::AlignTop );
  frame5Layout->addWidget( groupBox1 );

  textLabel1 = new QLabel( groupBox1, "textLabel1" );
  textLabel1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
  textLabel1->setText( tr( "Number of Channels" ) );
  groupBox1Layout->addWidget( textLabel1, 0, 0 );

  numberOfChannelsBox = new QSpinBox( groupBox1, "numberOfChannelsBox" );
  numberOfChannelsBox->setButtonSymbols( QSpinBox::PlusMinus );
  numberOfChannelsBox->setMaxValue( 50 );
  numberOfChannelsBox->setMinValue( 1 );
  numberOfChannelsBox->setValue( (int)numCh  );
  numberOfChannelsBox->setPrefix( QString::null );
  numberOfChannelsBox->setSpecialValueText( QString::null );
  groupBox1Layout->addWidget( numberOfChannelsBox, 0, 1 );

  samplingRateBox = new QSpinBox( groupBox1, "samplingRateBox" );
  samplingRateBox->setMaxValue( (int)1E9 );
  samplingRateBox->setMinValue( 1 );
  samplingRateBox->setValue( (int)freq  );
  samplingRateBox->setSuffix( tr( " Hz" ) );
  groupBox1Layout->addWidget( samplingRateBox, 1, 1 );

  textLabel2 = new QLabel( groupBox1, "textLabel2" );
  textLabel2->setText( tr( "Sampling rate" ) );
  textLabel2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
  groupBox1Layout->addWidget( textLabel2, 1, 0 );

  numberOfSamplesBox = new QSpinBox( groupBox1, "samplingRateBox" );
  numberOfSamplesBox->setMaxValue( 10000 );
  numberOfSamplesBox->setMinValue( 1 );
  numberOfSamplesBox->setValue( nSamples  );
  numberOfSamplesBox->setSuffix( tr( " samples" ) );
  groupBox1Layout->addWidget( numberOfSamplesBox, 2, 1 );

  textLabel3 = new QLabel( groupBox1, "textLabel2" );
  textLabel3->setText( tr( "Sample buffer" ) );
  textLabel3->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
  groupBox1Layout->addWidget( textLabel3, 2, 0 );

  continousCheckBox = new QCheckBox( groupBox1, "samplingRateBox" );
  continousCheckBox->setChecked( continous==1  );
  groupBox1Layout->addWidget( continousCheckBox, 3, 1 );

  textLabel4 = new QLabel( groupBox1, "textLabel2" );
  textLabel4->setText( tr( "Continous sampling" ) );
  textLabel4->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
  groupBox1Layout->addWidget( textLabel4, 3, 0 );

  applyButton = new QPushButton( frame5, "applyButton" );
  applyButton->setAutoDefault( TRUE );
  applyButton->setDefault( TRUE );
  applyButton->setFlat( FALSE );
  applyButton->setText( tr( "Save in config" ) );

  frame5Layout->addWidget( applyButton );

  resize( QSize(169, 150).expandedTo(minimumSizeHint()) );
  setMaximumSize(size());
  clearWState( WState_Polished );

  connect( applyButton, SIGNAL( clicked() ), this, SLOT( slotApply() ) );
}

samplingSettings::~samplingSettings()
{}

void samplingSettings::closeEvent( QCloseEvent* ce )
{
  ce->accept();
  return;
}

void samplingSettings::slotApply()
{
	emit signalSamplingSettingsChanged( (double)(samplingRateBox->value()),
					    (int)(numberOfChannelsBox->value()),
					    (int)(numberOfSamplesBox->value()),
					    (int)(continousCheckBox->isChecked()));
}
