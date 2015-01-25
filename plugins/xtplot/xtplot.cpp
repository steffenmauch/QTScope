/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *   Copyright (C) 2005 by Bernd Porr                                      *
 *   Bernd Porr, BerndPorr@f2s.com                                         *
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
#include "xtplot.h"

#include <qcheckbox.h>
#include <QLabel>
#include <QString>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QToolBar>
#include <QHBoxLayout>

#include <qwt_math.h>
#include <qwt_counter.h>
#include <qwt_plot_canvas.h>

#include <stdlib.h>
// for the data formats
#include <comedilib.h>
#include <iostream>

pluginInfo myPluginInfo = {"x-t Plot", "Plot", 1, COMEDI_SUBD_AI};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples) {
	return new xtPlot(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;
using std::cerr;

xtPlot::xtPlot(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags,numberOfSamples)
	{
	cout << "xtPlot::xtPlot: xtPlot Plugin generated\n";
	callingWidget = caller;
	idThis = id;
  
	y_Min = 1E+37;
	y_Max = -1E+37;

	setWindowTitle( QString().sprintf("Channel %s",name) );

	cerr << "Number of samples:" << numberOfSamples << "\n";

	plotLength = numberOfSamples;
	nSamples = numberOfSamples;

	x=new double[numberOfSamples];
	y=new double[numberOfSamples];

	// initialize the data arrays
	clearData();

	// allow Docking
	//setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea);
  
	// construct a toolbar
	QVBoxLayout *plotTools = new QVBoxLayout();
	QGroupBox *groupTools = new QGroupBox();

	autoscaleCheck = new QCheckBox("Autoscale");
	plotTools->addWidget(autoscaleCheck);
	autoscaleCheck->setChecked(TRUE);
	connect(autoscaleCheck, SIGNAL(clicked()), this, SLOT(slotAutoscaleToggled()));

	plotTools->addWidget( new QLabel(tr("Ymax:")) );
	ymaxCounter = new QwtCounter( );
	plotTools->addWidget( ymaxCounter );
    ymaxCounter->setRange(-1, 20.0);
    ymaxCounter->setSingleStep(0.01);
	ymaxCounter->setNumButtons(3);
	ymaxCounter->setIncSteps(QwtCounter::Button1, 1);
	ymaxCounter->setIncSteps(QwtCounter::Button2, 10);
	ymaxCounter->setIncSteps(QwtCounter::Button3, 100);
	ymaxCounter->setValue(1);
	connect(ymaxCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYmaxChanged(double)));
	ymaxCounter->setDisabled(TRUE);

	plotTools->addWidget( new QLabel(tr("Ymin:") ));
	yminCounter = new QwtCounter( );
	plotTools->addWidget( yminCounter );
    yminCounter->setRange(-20.0, 1);
    yminCounter->setSingleStep(0.01);
	yminCounter->setNumButtons(3);
	yminCounter->setIncSteps(QwtCounter::Button1, 1);
	yminCounter->setIncSteps(QwtCounter::Button2, 10);
	yminCounter->setIncSteps(QwtCounter::Button3, 100);
	yminCounter->setValue(-1);
	connect(yminCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYminChanged(double)));
	yminCounter->setDisabled(TRUE);

	QFont tbFont("Courier",14);
	tbFont.setBold(TRUE);

	QGroupBox* tbGrp=new QGroupBox();
	QHBoxLayout *tbGrp_layout = new QHBoxLayout;

	QLabel* lx = new QLabel(tr("X:"), tbGrp);
	tbGrp_layout->addWidget( lx );
	lx->setFont(tbFont);

	// create the button
	tbIncPushButton = new QPushButton( "+" );
	tbGrp_layout->addWidget( tbIncPushButton );
	tbIncPushButton->setFont(tbFont);
	tbIncPushButton->setSizeIncrement(0,0);
	tbIncPushButton->setSizePolicy (QSizePolicy(QSizePolicy::Fixed,
									QSizePolicy::Fixed));

	plotTools->connect(tbIncPushButton, SIGNAL( clicked() ),
			this, SLOT( incTbEvent() ) );
  
	// create the button
	tbDecPushButton = new QPushButton( "-" );
	tbGrp_layout->addWidget( tbDecPushButton );
	tbDecPushButton->setFont(tbFont);
	tbDecPushButton->setSizeIncrement(0,0);
	tbDecPushButton->setSizePolicy (QSizePolicy(QSizePolicy::Fixed,
									QSizePolicy::Fixed));
  

	plotTools->connect(tbDecPushButton, SIGNAL( clicked() ),
			this, SLOT( decTbEvent() ) );

	tbGrp->setLayout(tbGrp_layout);
	plotTools->addWidget( tbGrp );

	freezePushButton = new QPushButton( "Freeze" );
	plotTools->addWidget( freezePushButton );
	freezePushButton->setEnabled( TRUE );
	freezePushButton->setCheckable( TRUE );

	// filename the button
	filePushButton = new QPushButton( "Save Data" );
	plotTools->addWidget( filePushButton );
	filePushButton->setEnabled( FALSE );

	plotTools->connect(filePushButton, SIGNAL( clicked() ),
		this, SLOT( enterFileName() ) );

	//contruct a QwtPlot Widget
	plotWidget = new QwtPlot();
  
	curve = new QwtPlotCurve("Graph");
     
	//set curve color
	curve->setPen(QPen(Qt::green, 2));
     
	// add curves
	curve->attach(plotWidget);
     
	// copy the data into the curves
    curve->setRawSamples(x, y, plotLength);
  
	// QwtPlot specific defaults:
	// colour
	plotWidget->setCanvasBackground(Qt::white);

	// set some defaults for the axes
	plotWidget->setAxisTitle(QwtPlot::xBottom, "Time/ms");
	plotWidget->setAxisTitle(QwtPlot::yLeft, "Amplitude/V");
	// default xrange
	plotWidget->setAxisScale( QwtPlot::xBottom, 0, 1000);
	// yrange autoscale
	plotWidget->setAxisAutoScale(QwtPlot::yLeft);

	groupTools->setLayout(plotTools);
	groupTools->setFixedSize( groupTools->sizeHint() );
  
	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->addWidget(groupTools);
	hbox->addWidget(plotWidget);
  
	setLayout( hbox );
  
	// finally, refresh the plot
	plotWidget->replot(); 
}


xtPlot::~xtPlot(){
	delete x;
	delete y;
}

/*!
    \fn xtPlot::insertValue(double v)
 */
void xtPlot::insertValues(int num,int append){

	if ( freezePushButton->isChecked() ) {
		filePushButton->setEnabled( TRUE );
	} else {
		filePushButton->setEnabled( FALSE );
		
		if (append) {
			// scroll
			for(int k=nSamples-1; k>num; k--){
				y[k] = y[k-num-1];
			}
            //qwtShiftArray(y, nSamples, num);
			for(int i=0; i<num; i++) {
				y[i] = ds[num-i-1];
			}
			//qDebug() << 'add data' << num << endl;
		} else {
			// write from zero loc
			for(int i=0; i< num; i++) {
				y[i] = ds[i];
			}
		}
		
		if( autoscaleCheck->isChecked() ){
			for( int k=0; k<nSamples; k++ ){
				y_Min = qMin( y_Min, y[k] );
				y_Max = qMax( y_Max, y[k]) ;
			}
			//qDebug() << "min: " << qRound(yMin) << " max: " << qRound(yMax) << endl;
			plotWidget->setAxisScale( QwtPlot::yLeft, qRound(y_Min), qRound(y_Max) );
		}
	}
	plotWidget->replot();
}


/*!
    \fn xtPlot::clearData()

    reset the data on the plot
 */
void xtPlot::clearData(){
	for(int i=0; i<nSamples; i++) {
		x[i]=1000.0*(double)i/freq;
		y[i]=0.0;
	}
}


/*!
    \fn xtPlot::replot()
 */
void xtPlot::replot(){
	plotWidget->replot();
}


void xtPlot::incTbEvent(){
	plotLength=plotLength*2;
	xRangeChanged();
}



void xtPlot::decTbEvent(){
	plotLength=plotLength/2;
	xRangeChanged();
}



/*!
    \fn xtPlot::slotXrangeChanged(double)
 */
void xtPlot::xRangeChanged(){
	if (plotLength>=nSamples) {
		plotLength=nSamples;
	}
	if (plotLength<2) {
		plotLength=2;
	}
	samplingRateChanged();
}


void xtPlot::samplingRateChanged(){
	plotWidget->setAxisScale( QwtPlot::xBottom, 0, 1000.0*(double)plotLength/freq );
	clearData();
	//replot();
	plotWidget->replot(); 
}



/*!
    \fn xtPlot::slotYminChanged
 */
void xtPlot::slotYminChanged(double v){
    const QwtScaleDiv a = plotWidget->axisScaleDiv(QwtPlot::yLeft);
    plotWidget->setAxisScale( QwtPlot::yLeft, v, a.upperBound());
    ymaxCounter->setRange(v, 20.0);
    ymaxCounter->setSingleStep(0.01);
}


/*!
    \fn xtPlot::slotYmaxChanged(double)
 */
void xtPlot::slotYmaxChanged(double v){
    const QwtScaleDiv a = plotWidget->axisScaleDiv(QwtPlot::yLeft);
    plotWidget->setAxisScale( QwtPlot::yLeft, a.lowerBound(), v);
    yminCounter->setRange(-20.0, v);
    yminCounter->setSingleStep(0.01);
}


/*!
    \fn xtPlot::slotAutoscaleToggled()
 */
void xtPlot::slotAutoscaleToggled(){
	if( autoscaleCheck->isChecked() ){
		// yrange autoscale on
		yminCounter->setDisabled(TRUE);
		ymaxCounter->setDisabled(TRUE);
		plotWidget->setAxisAutoScale(QwtPlot::yLeft);
	}  else  {
		// yrange autoscale off
		yminCounter->setDisabled(FALSE);
		ymaxCounter->setDisabled(FALSE);

		yminCounter->setValue( qRound(y_Min) );
		ymaxCounter->setValue( qRound(y_Max) );
	}
}



void  xtPlot::enterFileName(){
	QFileDialog* fd=new QFileDialog( this );
	QString name = QString( tr("Save Data") );
	int result;
	do {
		result=0;
		QString selectedFilter;
		QString filename = fd->getSaveFileName( 
			this, 
			name, 
			QDir::currentPath(), 
			tr("comma separated (*.csv);;whitespace separated (*.txt);;whitespace separated(*.dat)"),
			&selectedFilter );
		
		if ( !filename.isNull() ) {

			QString extension;
			//qDebug() << selectedFilter;
			extension = selectedFilter.section('.', -1);
			extension.truncate(3);
			//qDebug() << extension << endl;
			if ( !filename.contains(extension, Qt::CaseInsensitive) ) {
				filename=filename+"."+extension;
				//qDebug() << filename << " " << extension << endl;
			}
			FILE* f=fopen(filename.toLatin1(),"wt");
			if (f) {
				char c;
				if ( extension.contains("csv",Qt::CaseInsensitive) ) {
					c=',';
				} else {
					c=' ';
				}
				for(int i=0; i<plotLength; i++) {
					float t=(float)i/freq;
					int result=fprintf(f,"%f%c%f\n",t,c,y[i]);
					if (result<0) {
						break;
					}
				}
				if (!(result<0)) {
					result=fclose(f);
				}
			} else {
				result=-1;
			}
		} else {
			result=0;
		}
		if (result<0) {
			name = QString("Error saving file. Please retry.");
		}
        } while (result<0);
	delete fd;
}
