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

#include "newviewdialog.h"

#include <QPushButton>
#include <QSpinBox>
#include <QLayout>
#include <QDebug>

newViewDialog::newViewDialog(comedi_t *comediDevice1, QList<pluginData> availablePlugins, QTScope *c, QWidget *parent, const char *name, bool modal, Qt::WindowFlags f )
    : QDialog( parent )
{
  caller = c;
  pl = availablePlugins;
  maxSelect = 0;
  maxChannels = 0;

  comediDevice = comediDevice1;
  
  setWindowTitle( "QTScope -- Open new Plot" );
  resize( 350, 240 );

  main = new QVBoxLayout();
  mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(2,2,2,2);

  f1 = new QFrame();
  f1->setFrameShape( QFrame::StyledPanel );
  f1->setFrameShadow( QFrame::Sunken );
  main->addWidget(f1);

  f1->setLayout(mainLayout);


  chooserBox = new QHBoxLayout();

  // the list of plugins
  pluginsList = new QListWidget( );
  pluginsList->setFocusPolicy( Qt::StrongFocus );
  pluginsList->setFrameStyle( QFrame::Panel | QFrame::Raised );

  QListIterator<pluginData> it(pl);
    while( it.hasNext() ){
        new QListWidgetItem( it.next().name, pluginsList );
    }

  connect( pluginsList, SIGNAL( itemSelectionChanged () ),
    this, SLOT( slotPluginSelected() ) );

  chooserBox->addWidget(pluginsList);
  QWidget* temp = new QWidget();
  temp->setLayout(chooserBox);
  mainLayout->addWidget(temp);

  // the channels
  channelsList = new QWidget( );
  channelsListLayout = new QVBoxLayout();
  channelsListLayout->addWidget( new QLabel("Select Input Channel(s)") );

  channelsListL = new QVBoxLayout( );
  channelsListWidget = new QWidget();

  channelsListWidget->setLayout( channelsListL );

  channelsListLayout->addWidget( channelsListWidget );

  // Add a vertical spacer to take up the remaining available space
  QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding,
    QSizePolicy::Expanding );
  channelsListLayout->addItem( spacer );

  status = new QLabel();
  channelsListLayout->addWidget( status );
  status->setAlignment(Qt::AlignCenter);
  status->setText("");
  status->setStyleSheet("QLabel { color : blue; }");

  channelsListLayout->addStretch(1);
  channelsList->setLayout(channelsListLayout);

  chooserBox->addWidget(channelsList);

  // OK+Cancel
  buttonBox = new QHBoxLayout();
  QWidget *bBox = new QWidget();
  bBox->setLayout(buttonBox);
  mainLayout->addWidget(bBox);

  okPushButton = new QPushButton( );
  okPushButton->setText( "OK" );
  okPushButton->setDefault( TRUE );
  okPushButton->setMaximumSize(okPushButton->sizeHint());
  okPushButton->setEnabled( FALSE );
  //okPushButton->setSizePolicy(QSizePolicy::Fixed);
  buttonBox->addWidget( okPushButton );

  cancelPushButton = new QPushButton( );
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

    setLayout(main);
}


newViewDialog::~newViewDialog()
{}

/*!
    \fn newViewDialog::accept()
 */
void newViewDialog::accept()
{
	// prepare a list of requested channels
	int *channels = new int[maxSelect];
	
	QListIterator<QSpinBox *> it(channelSelectors);
	int i=0;
	while( it.hasNext() ){
		channels[i] = (it.next())->value();
		i++;
	}
	
	// start a new plugin
	qDebug() << pluginsList->currentItem()->text();
	caller->runPlugin(pluginsList->currentItem()->text(), channels);
	QDialog::accept();
}

/*!
    \fn newViewDialog::slotPluginSelected()
 */
void newViewDialog::slotPluginSelected()
{
	QString name = pluginsList->currentItem()->text();
	okPushButton->setEnabled( TRUE );
	
	QListIterator<pluginData> it(pl);
	while( it.hasNext() ){
		pluginData data = it.next();
		if ( data.name == name ){
			maxSelect = data.numChannels;

            int comediSubdevice = comedi_find_subdevice_by_type(comediDevice,data.type_comedi,0);
            if( comediSubdevice >= 0){
                maxChannels = comedi_get_n_channels(comediDevice, comediSubdevice);
                okPushButton->setEnabled(TRUE);
                status->setText(QString("comedi subdevice\ntype %1 found!").arg(data.type_comedi));
            }
            else{
                //comedi_perror( QString("error in %1 line %2").arg(__func__).arg(__LINE__).toStdString().c_str() );
                okPushButton->setEnabled(FALSE);
                maxChannels = 0;
                status->setText(QString("comedi subdevice\ntype %1 not found!").arg(data.type_comedi));
            }

            if( maxChannels < 0 ){
                maxChannels = 0;
                comedi_perror( QString("error in %1 line %2").arg(__func__).arg(__LINE__).toStdString().c_str() );
            }

			if(channelSelectors.count() < maxSelect)
				for( int i = channelSelectors.count(); i < maxSelect; i++ ) {
					QSpinBox *spinBox = new QSpinBox();
					spinBox->setRange( 0, maxChannels-1 );
					channelsListL->addWidget( spinBox );
					
					channelSelectors << spinBox ;
				}
			else if(channelSelectors.count() > maxSelect) {
				channelSelectors.last();
				for( int i = maxSelect; i< channelSelectors.count(); i++ )
					 delete channelSelectors.takeLast();
			}
        }
	}
}


