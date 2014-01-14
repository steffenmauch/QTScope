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
#include <q3listbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <QDebug>
#include <q3filedialog.h>

propertiesDialog::propertiesDialog(QTScope *c, const char *name)
    : Q3TabDialog(c, name)
{
  caller = c;

  // so far, only the plugin paths can be set!

  Q3VBox *pluginsTab = new Q3VBox( this );
  pluginsTab->setMargin( 5 );
  pluginsTab->setSpacing( 5 );

  (void)new QLabel( QString( "Plugin Search Path (restart to activate)" ), pluginsTab );

  ppaths = new Q3ListBox( pluginsTab );
  for ( QStringList::Iterator it = caller->pluginPath.begin(); it != caller->pluginPath.end(); ++it )
    {
      ppaths->insertItem( *it );
    }
  ppaths->setCurrentItem( 1 );

  QPushButton *addButton = new QPushButton("Add New...", pluginsTab);
  connect ( addButton, SIGNAL( clicked() ), SLOT( addClicked() ) );
  QPushButton *removeButton = new QPushButton("Remove", pluginsTab);
  connect ( removeButton, SIGNAL( clicked() ), SLOT( removeClicked() ) );

  addButton->setMaximumSize(addButton->sizeHint());
  removeButton->setMaximumSize(removeButton->sizeHint());

  addTab( pluginsTab, "Plugins" );

  resize(320, 270);


}

propertiesDialog::~propertiesDialog()
{}

/*!
    \fn propertiesDialog::addClicked()
 */
void propertiesDialog::addClicked()
{
  Q3FileDialog* fd = new Q3FileDialog( this, "file dialog", TRUE );
  fd->setMode( Q3FileDialog::Directory );
  QString fileName;
  if ( fd->exec() == QDialog::Accepted )
    {
      fileName = fd->selectedFile();
      ppaths->insertItem( fileName );
      caller->pluginPath.append(fileName);
    }
}


/*!
    \fn propertiesDialog::removeClicked()
 */
void propertiesDialog::removeClicked()
{
	QString selected = ppaths->currentText();
	ppaths->setCurrentItem( -1 );
	
	if( caller->pluginPath.size() > 1 ){
		for ( QStringList::Iterator it = caller->pluginPath.begin(); it != caller->pluginPath.end(); ++it )
			if( selected == (*it) )
				it = caller->pluginPath.remove(it);
		
	}
	else
		caller->pluginPath.remove( caller->pluginPath.begin() );
		
	ppaths->removeItem(ppaths->currentItem());
}
