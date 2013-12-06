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
#include "xyplot.h"

#include <qcheckbox.h>
#include <qlabel.h>
#include <qstring.h>

#include <qwt_math.h>
#include <qwt_counter.h>

#include <stdlib.h>

#include <iostream>

pluginInfo myPluginInfo = {"x-y Plot", "Plot", 2};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, int wflags,int numberOfSamples)
{
	return new xyPlot(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;

xyPlot::xyPlot(QTScope* caller, QWidget* parent, const char* name, int id, int wflags, int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags,numberOfSamples)
{
  cout << "xyPlot::xyPlot: xyPlot Plugin generated\n";
  callingWidget = caller;
  idThis = id;

  setCaption( QString().sprintf("Channels %s",name) );
  plotLength = numberOfSamples;
  x=new double[plotLength];
  y=new double[plotLength];

  // initialize the data arrays
  clearData();

  // allow Docking
  setDockEnabled ( Qt::DockTop, TRUE );
  setDockEnabled ( Qt::DockLeft, TRUE );

  // conmstruct a toolbar
  Q3ToolBar * plotTools = new Q3ToolBar( this, "plot operations" );
  plotTools->setLabel( tr("Plot Operations") );

  new QLabel(tr("Y max:"), plotTools);
  ymaxCounter = new QwtCounter( plotTools );
  ymaxCounter->setRange(-1, 20.0, 0.01);
  ymaxCounter->setNumButtons(2);
  ymaxCounter->setIncSteps(QwtCounter::Button1, 10);
  ymaxCounter->setIncSteps(QwtCounter::Button2, 100);
  ymaxCounter->setValue(1);
  connect(ymaxCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYmaxChanged(double)));
  ymaxCounter->setDisabled(TRUE);

  new QLabel(tr("Y min:"), plotTools);
  yminCounter = new QwtCounter( plotTools );
  yminCounter->setRange(-20.0, 1, 0.01);
  yminCounter->setNumButtons(2);
  yminCounter->setIncSteps(QwtCounter::Button1, 10);
  yminCounter->setIncSteps(QwtCounter::Button2, 100);
  yminCounter->setValue(-1);
  connect(yminCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYminChanged(double)));
  yminCounter->setDisabled(TRUE);

  plotTools->addSeparator();

  new QLabel(tr("X min:"), plotTools);
  xminCounter = new QwtCounter( plotTools );
  xminCounter->setRange(-20.0, 1, 0.01);
  xminCounter->setNumButtons(2);
  xminCounter->setIncSteps(QwtCounter::Button1, 10);
  xminCounter->setIncSteps(QwtCounter::Button2, 100);
  xminCounter->setValue(-1);
  connect(xminCounter, SIGNAL(valueChanged(double)), this, SLOT(slotXminChanged(double)));
  xminCounter->setDisabled(TRUE);

  new QLabel(tr("X max:"), plotTools);
  xmaxCounter = new QwtCounter( plotTools );
  xmaxCounter->setRange(-1, 20.0, 0.01);
  xmaxCounter->setNumButtons(2);
  xmaxCounter->setIncSteps(QwtCounter::Button1, 10);
  xmaxCounter->setIncSteps(QwtCounter::Button2, 100);
  xmaxCounter->setValue(1);
  connect(xmaxCounter, SIGNAL(valueChanged(double)), this, SLOT(slotXmaxChanged(double)));
  xmaxCounter->setDisabled(TRUE);


  plotTools->addSeparator();
  new QLabel(tr("Data Points:"), plotTools);
  rangeCounter = new QwtCounter(plotTools );
  rangeCounter->setRange(0, plotLength, 1.0);
  rangeCounter->setNumButtons(2);
  rangeCounter->setIncSteps(QwtCounter::Button1, 100);
  rangeCounter->setIncSteps(QwtCounter::Button3, 1000);
  rangeCounter->setValue(100);
  connect(rangeCounter, SIGNAL(valueChanged(double)), this, SLOT(slotRangeChanged(double)));
  rangeCounter->setDisabled(FALSE);

  plotTools->addSeparator();
  autoscaleCheck = new QCheckBox("Autoscale", plotTools);
  autoscaleCheck->setChecked(TRUE);
  connect(autoscaleCheck, SIGNAL(clicked()), this, SLOT(slotAutoscaleToggled()));

  moveDockWindow( plotTools, Qt::DockLeft );

  // contruct a QwtPlot Widget
  plotWidget = new QwtPlot(this);

  // QwtPlot specific defaults:
  // colour
  plotWidget->setCanvasBackground(Qt::white);
  // outline
  //plotWidget->enableOutline(FALSE);
  // no legend
  //plotWidget->enableLegend(FALSE);
  
  // no grid
  grid = new QwtPlotGrid();
  grid->enableX(true);
  grid->enableXMin(true);

  // for major grid line
  grid->setMajPen( QPen(Qt::black,1) );
  // for minor grid line
  grid->setMinPen( QPen(Qt::gray,1) );

  QwtScaleDiv div;
  QwtLinearScaleEngine *lineSE = new QwtLinearScaleEngine();
  div = lineSE->divideScale(0, 150, 2, 5, 15);
  
  grid->attach( plotWidget );
  
  // set some defaults for the axes
  plotWidget->setAxisTitle(QwtPlot::xBottom, "Amplitude/V");
  plotWidget->setAxisTitle(QwtPlot::yLeft, "Amplitude/V");
  // default xrange
  plotWidget->setAxisScale( QwtPlot::xBottom, 0, 1000);
  // yrange autoscale
  plotWidget->setAxisAutoScale(QwtPlot::yLeft);
  plotWidget->setAxisAutoScale(QwtPlot::xBottom);

  curve = new QwtPlotCurve("Graph");
     
  //set curve color
  curve->setPen(QPen(Qt::green, 2));
     
  // add curves
  curve->attach(plotWidget);
      
  // copy the data into the curves
  curve->setRawSamples(x, y, plotLength);
     
  // finally, refresh the plot
  plotWidget->replot(); 

  setCentralWidget(plotWidget);
}


xyPlot::~xyPlot() {
	delete x;
	delete y;
}

/*!
    \fn xtPlot::insertValue(double v)
 */
void xyPlot::insertValues(int num,int append)
{
	if (append) {
        //qwtShiftArray(x, plotLength, num);
        for(int k=plotLength-1; k>=0; k--){
			x[k] = x[k-num-1];
		}
        //qwtShiftArray(y, plotLength, num);
        for(int k=plotLength-1; k>=0; k--){
			y[k] = y[k-num-1];
		}
	}
	for(int i=0; i<num; i++) {
		x[i] = dsx[num-i-1];
		y[i] = dsy[num-i-1];
	}
}

/*!
    \fn xtPlot::clearData()

    reset the data on the plot
 */
void xyPlot::clearData()
{
  for(int i=0; i<plotLength; i++)
    {
      x[i] = 0.0;
      y[i] = 0.0;
    }
}


/*!
    \fn xtPlot::replot()
 */
void xyPlot::replot()
{
  plotWidget->replot();
}


/*!
    \fn xtPlot::slotXrangeChanged(double)
 */
void xyPlot::slotRangeChanged(double v)
{
  // assign data
  //plotWidget->setCurveRawData(curve, x, y, (int)v);
  curve->setRawSamples(x, y, (int)v);
  plotWidget->replot();
}


/*!
    \fn xtPlot::slotYminChanged
 */
void xyPlot::slotYminChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScaleDiv(QwtPlot::yLeft);
  plotWidget->setAxisScale( QwtPlot::yLeft, v, a->upperBound());
  ymaxCounter->setRange(v, 20.0, 0.01);
}


/*!
    \fn xtPlot::slotYmaxChanged(double)
 */
void xyPlot::slotYmaxChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScaleDiv(QwtPlot::yLeft);
  plotWidget->setAxisScale( QwtPlot::yLeft, a->lowerBound(), v);
  yminCounter->setRange(-20.0, v, 0.01);
}

/*!
    \fn xtPlot::slotYminChanged
 */
void xyPlot::slotXminChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScaleDiv(QwtPlot::xBottom);
  plotWidget->setAxisScale( QwtPlot::xBottom, v, a->upperBound());
  xmaxCounter->setRange(v, 20.0, 0.01);
}


/*!
    \fn xtPlot::slotYmaxChanged(double)
 */
void xyPlot::slotXmaxChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScaleDiv(QwtPlot::xBottom);
  plotWidget->setAxisScale( QwtPlot::xBottom, a->lowerBound(), v);
  xminCounter->setRange(-20.0, v, 0.01);
}


/*!
    \fn xtPlot::slotAutoscaleToggled()
 */
void xyPlot::slotAutoscaleToggled()
{
  if(autoscaleCheck->isChecked())
    {
      // yrange autoscale on
      yminCounter->setDisabled(TRUE);
      ymaxCounter->setDisabled(TRUE);
      xminCounter->setDisabled(TRUE);
      xmaxCounter->setDisabled(TRUE);
      plotWidget->setAxisAutoScale(QwtPlot::yLeft);
      plotWidget->setAxisAutoScale(QwtPlot::xBottom);
    }
  else
    {
      // yrange autoscale off
      yminCounter->setDisabled(FALSE);
      ymaxCounter->setDisabled(FALSE);
      xminCounter->setDisabled(FALSE);
      xmaxCounter->setDisabled(FALSE);
      const QwtScaleDiv* a = plotWidget->axisScaleDiv(QwtPlot::yLeft);
      yminCounter->setValue(a->lowerBound());
      ymaxCounter->setValue(a->upperBound());
      plotWidget->setAxisScale( QwtPlot::yLeft, a->lowerBound(), a->upperBound());
      a = plotWidget->axisScaleDiv(QwtPlot::xBottom);
      xminCounter->setValue(a->lowerBound());
      xmaxCounter->setValue(a->upperBound());
      plotWidget->setAxisScale( QwtPlot::xBottom, a->lowerBound(), a->upperBound());
    }
}

