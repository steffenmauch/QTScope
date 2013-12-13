/***************************************************************************
 *   Copyright (C) 2013 by Steffen Mauch                                   *
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

#include "switch.h"
#include <QDebug>
#include <iostream>

pluginInfo myPluginInfo = {"Switch Plugin", "Plot", 2};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples) {
	return new switchPlugin(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;

switchPlugin::switchPlugin(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags, int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags, numberOfSamples)
{
	cout << "switch::switch: Dummy Plugin generated\n";
	callingWidget = caller;
	idThis = id;
	
	cb = new QCheckBox("Show Title");
	
	#define NUMBER 16
	
	list = new QVector<QCheckBox*>();
	
	window = new QWidget();
	vbox_layout = new QVBoxLayout();
	
	for(int k=0; k<NUMBER; k++){
		QCheckBox *temp = new QCheckBox( 
			QString().sprintf("Chan. %02i", k) 
			);
		list->append( temp );
		vbox_layout->addWidget(temp);
		;
	}
	
	vbox_layout->setAlignment ( Qt::AlignHCenter );
	
	save = new QPushButton( tr("Save") );
	vbox_layout->addWidget(save);
	vbox_layout->connect(save, SIGNAL( clicked() ),
		this, SLOT( saveCheckBoxes() ) );
	
	window->setLayout(vbox_layout);
    setWidget(window);
	
	setWindowTitle("digital output");
	
	show();
}

void switchPlugin::saveCheckBoxes(){
	qDebug() << "called function: " << __func__ << endl;
	
	QVectorIterator<QCheckBox*> i(*list);
	while ( i.hasNext() ){
		qDebug() << i.next()->checkState();
	}
}

QSize switchPlugin::sizeHint() const
{
	return QSize(100, 100);
}

switchPlugin::~switchPlugin()
{
}


