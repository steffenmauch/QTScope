/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *   Copyright (C) 2013 by Steffen Mauch                                   *
 *   Steffen Mauch, steffen.mauch@gmail.com                                *
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

#include "newviewdialog.h"

#include <QPushButton>
#include <QSpinBox>
#include <QLayout>
#include <QDebug>

//Added by qt3to4:
#include <Q3ValueList>


newViewDialog::newViewDialog(int numChannels, Q3ValueList <pluginData> availablePlugins, QTScope *c, QWidget *parent, const char *name, bool modal, Qt::WFlags f )
    : QDialog(parent, name, modal, f)
{
  caller = c;
  pl = availablePlugins;
  maxSelect = 1;
  maxChannels = numChannels;

  channelSelectors.setAutoDelete( TRUE );

  setWindowTitle( "QTScope -- Open new Plot" );
  resize( 320, 240 );

  mainLayout = new QVBoxLayout( this, 2, 2, "main");

  f1 = new QFrame( this );
  f1->setFrameShape( QFrame::StyledPanel );
  f1->setFrameShadow( QFrame::Sunken );
  mainLayout->addWidget(f1);

  chooserBox = new QHBoxLayout( f1, 2, 2, "choose" );

  // the list of plugins
  pluginsList = new QListWidget( f1 );
  pluginsList->setFocusPolicy( Qt::StrongFocus );
  pluginsList->setFrameStyle( QFrame::Panel | QFrame::Raised );

  QLinkedList<pluginData>::iterator it = pl.begin();
	while( it != pl.end() ){
		new QListWidgetItem( (*it).name, pluginsList );
		it++;
	}

  connect( pluginsList, SIGNAL( itemSelectionChanged () ),
	this, SLOT( slotPluginSelected() ) );

  chooserBox->addWidget(pluginsList);

  // the channels
  channelsList = new QWidget( );
  channelsListLayout = new QVBoxLayout();
  channelsListLayout->addWidget( new QLabel("Select Input Channel(s)") );
  
  channelsListL = new QVBoxLayout( );
  channelsListWidget = new QWidget();
  
  channelsListWidget->setLayout( channelsListL );
  
  channelsListLayout->addWidget( channelsListWidget );
  channelsListLayout->addStretch(1);
  channelsList->setLayout(channelsListLayout);

  chooserBox->addWidget(channelsList);

  // OK+Cancel
  buttonBox = new QHBoxLayout( mainLayout );

  okPushButton = new QPushButton( this, "ok" );
  okPushButton->setText( "OK" );
  okPushButton->setDefault( TRUE );
  okPushButton->setMaximumSize(okPushButton->sizeHint());
  okPushButton->setEnabled( FALSE );
  //okPushButton->setSizePolicy(QSizePolicy::Fixed);
  buttonBox->addWidget( okPushButton );

  cancelPushButton = new QPushButton( this, "cancel" );
  cancelPushButton->setText( "Cancel" );
  //cancelPushButton->setAccel( Key_Escape );
  cancelPushButton->setMaximumSize(cancelPushButton->sizeHint());
  //cancelPushButton->setSizePolicy(QSizePolicy::Fixed);
  buttonBox->addWidget( cancelPushButton );

  connect( okPushButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( cancelPushButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

	if( pluginsList->count() > 0 ){
		// start with first plugin
		pluginsList->setCurrentRow(0);
	}
}


newViewDialog::~newViewDialog()
{}

/*!
    \fn newViewDialog::accept()
 */
void newViewDialog::accept()
{
	//if( pluginsList->currentRow() >= 0 ){
		// prepare a list of requested channels
		int *channels = new int[maxSelect];
		channelSelectors.first();
		for(unsigned int i=0; i<maxSelect; i++) {
			channels[i] = channelSelectors.current()->value();
			channelSelectors.next();
		}
		// start a new plugin
		qDebug() << pluginsList->currentItem()->text();
		caller->runPlugin(pluginsList->currentItem()->text(), channels);
		QDialog::accept();
	//}
}

/*!
    \fn newViewDialog::slotPluginSelected()
 */
void newViewDialog::slotPluginSelected()
{
	QString name = pluginsList->currentItem()->text();
	okPushButton->setEnabled( TRUE );
	QLinkedList<pluginData>::iterator it = pl.begin();
	while( it != pl.end() )
    {
      if ( (*it).name == name )
        {
          maxSelect = (*it).numChannels;
          if(channelSelectors.count() < maxSelect)
            for(unsigned int i = channelSelectors.count(); i < maxSelect; i++) {
              channelSelectors.append( new QSpinBox( 0, maxChannels-1, 1, channelsList ) );
              channelsListL->addWidget(channelSelectors.current());
              channelSelectors.current()->show();
            }
          else if(channelSelectors.count() > maxSelect) {
            channelSelectors.last();
            for(unsigned int i = maxSelect; i< channelSelectors.count(); i++)
              channelSelectors.remove();
          }
        }
        it++;
    }
}


