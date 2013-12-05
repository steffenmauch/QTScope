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
#include "dataplot.h"
#include "qtscope.h"

#include <qcheckbox.h>
#include <qlabel.h>
#include <qstring.h>

#include <qwt_math.h>
#include <qwt_counter.h>

#include <stdlib.h>

#include <iostream>

DataPlot::DataPlot(QTScope* caller, QWidget* parent, const char* name, int id, int wflags)
    : QMainWindow(parent, name, wflags)
{
  callingWidget = caller;
  idThis = id;

  setCaption( QString().sprintf("Channel %s",name) );
  plotLength = MAX_PLOT_LENGTH;

  // initialize the data arrays
  clearData();

  // allow Docking
  setDockEnabled ( Qt::DockTop, TRUE );
  setDockEnabled ( Qt::DockLeft, TRUE );

  // conmstruct a toolbar
  QToolBar * plotTools = new QToolBar( this, "plot operations" );
  plotTools->setLabel( tr("Plot Operations") );

  //plotTools->addSeparator();
  QLabel* ly2 = new QLabel(tr("Ymax:"), plotTools);
  ymaxCounter = new QwtCounter( plotTools, "Ymax");
  ymaxCounter->setRange(-1, 20.0, 0.01);
  ymaxCounter->setNumButtons(3);
  ymaxCounter->setIncSteps(QwtCounter::Button1, 1);
  ymaxCounter->setIncSteps(QwtCounter::Button2, 10);
  ymaxCounter->setIncSteps(QwtCounter::Button3, 100);
  ymaxCounter->setValue(1);
  connect(ymaxCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYmaxChanged(double)));
  ymaxCounter->setDisabled(TRUE);

  plotTools->addSeparator();
  QLabel* ly1 = new QLabel(tr("Ymin:"), plotTools);
  yminCounter = new QwtCounter( plotTools, "Ymin");
  yminCounter->setRange(-20.0, 1, 0.01);
  yminCounter->setNumButtons(3);
  yminCounter->setIncSteps(QwtCounter::Button1, 1);
  yminCounter->setIncSteps(QwtCounter::Button2, 10);
  yminCounter->setIncSteps(QwtCounter::Button3, 100);
  yminCounter->setValue(-1);
  connect(yminCounter, SIGNAL(valueChanged(double)), this, SLOT(slotYminChanged(double)));
  yminCounter->setDisabled(TRUE);

  plotTools->addSeparator();
  QLabel* lx = new QLabel(tr("X:"), plotTools);
  xrangeCounter = new QwtCounter(plotTools,"X");
  xrangeCounter->setRange(1.0, MAX_PLOT_LENGTH, 1.0);
  xrangeCounter->setNumButtons(3);
  xrangeCounter->setIncSteps(QwtCounter::Button1, 10);
  xrangeCounter->setIncSteps(QwtCounter::Button2, 100);
  xrangeCounter->setIncSteps(QwtCounter::Button3, 1000);
  xrangeCounter->setValue(1000);
  connect(xrangeCounter, SIGNAL(valueChanged(double)), this, SLOT(slotXrangeChanged(double)));
  xrangeCounter->setDisabled(FALSE);

  plotTools->addSeparator();
  autoscaleCheck = new QCheckBox("Autoscale", plotTools);
  autoscaleCheck->setChecked(TRUE);
  connect(autoscaleCheck, SIGNAL(clicked()), this, SLOT(slotAutoscaleToggled()));

  moveDockWindow( plotTools, Left );

  // contruct a QwtPlot Widget
  plotWidget = new QwtPlot(this);

  // QwtPlot specific defaults:
  // colour
  plotWidget->setCanvasBackground(white);
  // outline
  plotWidget->enableOutline(FALSE);
  // no legend
  plotWidget->enableLegend(FALSE);
  // no grid
  plotWidget->enableGridX(FALSE);
  plotWidget->enableGridY(FALSE);
  // set some defaults for the axes
  plotWidget->setAxisTitle(QwtPlot::xBottom, "Time/ms");
  plotWidget->setAxisTitle(QwtPlot::yLeft, "Amplitude/V");
  // default xrange
  plotWidget->setAxisScale( QwtPlot::xBottom, 0, 1000);
  // yrange autoscale
  plotWidget->setAxisAutoScale(QwtPlot::yLeft);

  // insert a curve
  curve = plotWidget->insertCurve("");
  // assign data
  plotWidget->setCurveRawData(curve, x, y, plotLength);

  setCentralWidget(plotWidget);
}

DataPlot::~DataPlot()
{
  //delete [] x;
  //delete [] y;
}


void DataPlot::closeEvent( QCloseEvent* ce )
{
  ce->accept();
  // notify the calling widget
  callingWidget->slotChannelClosed(idThis);
  return;
}


/*!
    \fn DataPlot::insertValue(double v)
 */
void DataPlot::insertValue(double v)
{
  qwtShiftArray(y, MAX_PLOT_LENGTH, 1);
  y[0] = v;
}


/*!
    \fn DataPlot::clearData()

    reset the data on the plot
 */
void DataPlot::clearData()
{
  for(int i=0; i<MAX_PLOT_LENGTH; i++) {
    x[i]=i;
    y[i]=0.0;
  }
}


/*!
    \fn DataPlot::replot()
 */
void DataPlot::replot()
{
  plotWidget->replot();
}


/*!
    \fn DataPlot::slotXrangeChanged(double)
 */
void DataPlot::slotXrangeChanged(double v)
{
    plotWidget->setAxisScale( QwtPlot::xBottom, 0, v);
    replot();
}


/*!
    \fn DataPlot::slotYminChanged
 */
void DataPlot::slotYminChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScale(QwtPlot::yLeft);
  plotWidget->setAxisScale( QwtPlot::yLeft, v, a->hBound());
  ymaxCounter->setRange(v, 20.0, 0.01);
}


/*!
    \fn DataPlot::slotYmaxChanged(double)
 */
void DataPlot::slotYmaxChanged(double v)
{
  const QwtScaleDiv* a = plotWidget->axisScale(QwtPlot::yLeft);
  plotWidget->setAxisScale( QwtPlot::yLeft, a->lBound(), v);
  yminCounter->setRange(-20.0, v, 0.01);
}


/*!
    \fn DataPlot::slotAutoscaleToggled()
 */
void DataPlot::slotAutoscaleToggled()
{
  if(autoscaleCheck->isChecked())
  {
    // yrange autoscale on
    yminCounter->setDisabled(TRUE);
    ymaxCounter->setDisabled(TRUE);
    plotWidget->setAxisAutoScale(QwtPlot::yLeft);

  }
  else
  {
    // yrange autoscale off
    yminCounter->setDisabled(FALSE);
    ymaxCounter->setDisabled(FALSE);
    const QwtScaleDiv* a = plotWidget->axisScale(QwtPlot::yLeft);
    yminCounter->setValue(a->lBound());
    ymaxCounter->setValue(a->hBound());
    plotWidget->setAxisScale( QwtPlot::yLeft, a->lBound(), a->hBound());
  }
}

