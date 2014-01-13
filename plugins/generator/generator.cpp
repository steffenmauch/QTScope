/***************************************************************************
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

#include "generator.h"
#include <QDebug>
#include <iostream>

pluginInfo myPluginInfo = {"Generator Plugin", "Plot", 1, COMEDI_SUBD_AO};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples) {
	return new generatorPlugin(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;

generatorPlugin::generatorPlugin(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags, int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags, numberOfSamples)
{
	cout << "generator::generator: Plugin generated\n";
	callingWidget = caller;
	idThis = id;
			
	list = new QVector<QCheckBox*>();
	
	window = new QWidget();
	window2 = new QWidget();
	hbox_layout = new QHBoxLayout();
	vbox_layout = new QVBoxLayout();

	frequency = new QLineEdit();
	frequency->setText("5");
	amplitude = new QLineEdit();
	amplitude->setText("2");
	offset = new QLineEdit();
	offset->setText("0");
	
	hbox_layout->addWidget( new QLabel( tr("Freq.[Hz]") ) );
	hbox_layout->addWidget( frequency );
	hbox_layout->addWidget( new QLabel( tr("Amp.[V]") ) );
	hbox_layout->addWidget( amplitude );
	hbox_layout->addWidget( new QLabel( tr("Off.[V]") ) );
	hbox_layout->addWidget( offset );
	
	window2->setLayout( hbox_layout );
	
	dropdown = new QComboBox();
	
	dropdown->addItem( QString( tr("Sinus") ), QVariant::fromValue(0));
	dropdown->addItem( QString( tr("Triangle") ), QVariant::fromValue(1));
	dropdown->addItem( QString( tr("Rectangle") ), QVariant::fromValue(2) );
	
	vbox_layout->addWidget( dropdown );
	vbox_layout->addWidget( window2 );
	
	vbox_layout->setAlignment ( Qt::AlignHCenter );
 
    for ( int i = 0; i < NB_SAMPLES; i++ ) {
        x[i] =  ((double)i) / NB_SAMPLES;
        y[i] = 0;
    }
 
    plot = new QwtPlot();
    curve = new QwtPlotCurve();
    curve->setRawSamples(x, y, 101);
    curve->attach( plot );

    plot->setAxisAutoScale(0, true);
    plot->setAxisAutoScale(2, true);
    
    plot->setAxisTitle(0,tr("in [V]"));
    plot->setAxisTitle(2,tr("in [s]"));
	
	vbox_layout->addWidget( plot );

	
	save = new QPushButton( tr("Set") );
	vbox_layout->addWidget(save);
	vbox_layout->connect(save, SIGNAL( clicked() ),
		this, SLOT( saveCheckBoxes() ) );
	
	window->setLayout(vbox_layout);
    setWidget(window);
	
	setWindowTitle("analog output");
	
	show();
}


void generatorPlugin::saveCheckBoxes(){
	qDebug() << "called function: " << __func__ << endl;
	
	double freq = frequency->text().toDouble();
	double off = offset->text().toDouble();
	double amp = amplitude->text().toDouble();
	
	int index =  dropdown->itemData( dropdown->currentIndex() ).toInt();
	
	for ( int i = 0; i < NB_SAMPLES; i++ ) {
        x[i] = 2/(freq) * ((double)i) / ((double)NB_SAMPLES);
        
        if( index == 2 ){
			if( i%(NB_SAMPLES/2) <= NB_SAMPLES/4 )
				y[i] = -amp/2 + off;
			else
				y[i] = amp/2 + off;
		}
		else if( index == 1 ){
			y[i] = amp/2.0 - amp* (i%(NB_SAMPLES/2) / ((double)NB_SAMPLES/2.0)) + off;
		}
		else
			y[i] = amp*sin(2*3.1415*freq*x[i]) + off;
			
    }
    curve->setRawSamples(x, y, NB_SAMPLES);

	plot->replot();	
}

QSize generatorPlugin::sizeHint() const
{
	qDebug() << "called function: " << __func__ << endl;
	return QSize(200, 400);
}

generatorPlugin::~generatorPlugin()
{
	qDebug() << "called function: " << __func__ << endl;
}


