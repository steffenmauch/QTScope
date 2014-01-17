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
#include "propertiesdialog.h"
#include "qtscope.h"

#include <q3vbox.h>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QFileDialog>

propertiesDialog::propertiesDialog(QTScope *c, const char *name)
{
	caller = c;

	tabWidget = new QTabWidget(this);

	// so far, only the plugin paths can be set!

	Q3VBox *pluginsTab = new Q3VBox( this );
	pluginsTab->setMargin( 5 );
	pluginsTab->setSpacing( 5 );

	new QLabel( QString( "Plugin Search Path (restart to activate)" ), pluginsTab );

	ppaths = new QListWidget( pluginsTab );
	
	for ( QStringList::Iterator it = caller->pluginPath.begin(); it != caller->pluginPath.end(); ++it ){
		new QListWidgetItem( *it, ppaths );
    }

	QPushButton *addButton = new QPushButton("Add New...", pluginsTab);
	connect ( addButton, SIGNAL( clicked() ), SLOT( addClicked() ) );
	removeButton = new QPushButton("Remove", pluginsTab);
	connect ( removeButton, SIGNAL( clicked() ), SLOT( removeClicked() ) );

	addButton->setMaximumSize(addButton->sizeHint());
	removeButton->setMaximumSize(removeButton->sizeHint());

	tabWidget->addTab( pluginsTab, "Plugins" );
	tabWidget->resize(320, 270);
	
	if( ppaths->count() == 0 )
		removeButton->setEnabled( FALSE );
	else
		ppaths->setCurrentRow(0);
}

propertiesDialog::~propertiesDialog()
{}

/*!
    \fn propertiesDialog::addClicked()
 */
void propertiesDialog::addClicked()
{
	QFileDialog* fd = new QFileDialog( this, "file dialog");
	fd->setMode( QFileDialog::Directory );
	QString fileName;
	if ( fd->exec() == QDialog::Accepted ){
		fileName = fd->selectedFile();
		new QListWidgetItem( fileName, ppaths );
		caller->pluginPath.append(fileName);
		removeButton->setEnabled( TRUE );
		ppaths->setCurrentRow( 0 );
	}
}


/*!
    \fn propertiesDialog::removeClicked()
 */
void propertiesDialog::removeClicked()
{
	QString selected = ppaths->currentItem()->text();
	if( caller->pluginPath.size() > 1 ){
		for ( QStringList::Iterator it = caller->pluginPath.begin(); it != caller->pluginPath.end(); ++it ){
			if( selected == (*it) ){
				it = caller->pluginPath.remove(it);
				break;
			}
		}
	}
	else
		caller->pluginPath.remove( caller->pluginPath.begin() );
		
	QListWidgetItem* item = ppaths->takeItem( ppaths->currentRow() );
	delete item;
	
	if( ppaths->count() == 0 ){
		removeButton->setEnabled( FALSE );
	}
	ppaths->setCurrentRow( 0 );
}
