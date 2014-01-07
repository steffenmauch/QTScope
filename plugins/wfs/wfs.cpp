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

#include "wfs.h"
// for the data formats
#include <comedilib.h>
#include <QPainter>
#include <iostream>

pluginInfo myPluginInfo = {"WFS Plugin", "Plot", 1, COMEDI_SUBD_MEMORY};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples) {
	return new wfs(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;

wfs::wfs(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags, int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags, numberOfSamples)
{
	cout << "wfs::wfs: wfs plugin generated\n";
	callingWidget = caller;
	idThis = id;
	
	//set sizePolicy (important for heightForWidth)
	QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	sizePolicy.setHeightForWidth(true);
	setSizePolicy(sizePolicy);
	
	window = new QWidget();
	vbox_layout = new QVBoxLayout();
	hbox_layout = new QHBoxLayout();
	
	d_plot = new Plot();
	
	gradient_plot = new gradientPlot( );
	
	en_contour = new QPushButton( tr("enable contour") );
	en_contour->setCheckable( TRUE );
	
	en_nullref = new QPushButton( tr("enable reference") );
	en_nullref->setCheckable( TRUE );
	
	clear_nullref = new QPushButton( tr("clear reference") );
	
	hbox_layout->addWidget(en_nullref);
	hbox_layout->addWidget(clear_nullref);
	
	vbox_layout->addLayout(hbox_layout);

	vbox_layout->addWidget(en_contour);
	vbox_layout->addWidget(gradient_plot);
	vbox_layout->addWidget(d_plot);
	d_plot->hide();

	// connect signals to buttons
	hbox_layout->connect(clear_nullref, SIGNAL( clicked() ),
		this, SLOT( clearReference() ) );
	vbox_layout->connect(en_nullref, SIGNAL( clicked() ),
		this, SLOT( setReference() ) );
	vbox_layout->connect(en_contour, SIGNAL( clicked() ),
		this, SLOT( enableCountour() ) );

    window->setLayout(vbox_layout);
    setWidget(window);
     
	//setWidget( gradient_plot );
    //setWidget(d_plot);
    
    setWindowTitle("Wavefront Sensor plugin");
    
	show();
	resize(400,400);
	
}

void wfs::enableCountour(){
	qDebug() << "called function: " << __func__ << endl;
	
	if ( en_contour->isChecked() ) {
		d_plot->show();
		gradient_plot->hide();
		//this->resize(400,200);
	} else {
		d_plot->hide();
		gradient_plot->show();
	}
}

void wfs::getComedi(comedi_t *comediDevice1){
	qDebug() << "called function: " << __func__ << endl;
	comediDevice = comediDevice1;
	
	comediSubdevice = comedi_find_subdevice_by_type(comediDevice,COMEDI_SUBD_MEMORY,0);
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotReadData()));
	timer->start(500);
	
}

void wfs::slotReadData(){
	qDebug() << "called function: " << __func__ << endl;
	int count = 196;
	lsampl_t data[count];
	double data_x[count];
	
	int retval = 0;
	retval = comedi_data_read_n(comediDevice, comediSubdevice, 0, 0, AREF_GROUND, &data[0], 100);
	
	if(retval < 0)	{
		comedi_perror("comedi_data_read");
		return;
	}
	
	retval = comedi_data_read_n(comediDevice, comediSubdevice, 100, 0, AREF_GROUND, &data[100], 96);
	
	if(retval < 0)	{
		comedi_perror("comedi_data_read");
		return;
	}
	
	for( int k = 0; k < count; k++ ){
		//printf("%d\n", data[k]);
		data_x[k] = double(data[k]);
	}
	gradient_plot->setData( &data_x[0], &data_x[0] );
}

void wfs::clearReference(){
	qDebug() << "called function: " << __func__ << endl;
}

void wfs::setReference(){
	qDebug() << "called function: " << __func__ << endl;
}

QSize wfs::sizeHint() const
{
	return QSize(480, 550);
	//cout << "getting called\n";
}

wfs::~wfs()
{
}


