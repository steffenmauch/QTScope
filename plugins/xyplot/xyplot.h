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
#ifndef XYPLOT_H
#define XYPLOT_H

#include <scopeplotplugin.h>
#include <q3mainwindow.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_engine.h>

class QwtCounter;
class QCheckBox;
class QTScope;

/**
@author Matthias H. Hennig, Bernd Porr
*/
class xyPlot : public scopePlotPlugin
{
Q_OBJECT
public:
	xyPlot(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples);
    ~xyPlot();

    void insertValues(int num,int append=1);
    void replot();
    void setDataSource(sampl_t**, double **v) {dsx = v[0]; dsy = v[1];}
    void samplingRateChanged() {};

private:
    // the data plot
    QwtPlot *plotWidget;

    // store curve
    //long curve;
    QwtPlotCurve *curve;
    
    QwtPlotGrid *grid;

    // pointer to data
    double *dsx, *dsy;

    // plot data is stored here
    double *x, *y;

    // the length (x-axis) of the plot
    int plotLength;

    QwtCounter* rangeCounter;
    QwtCounter* yminCounter;
    QwtCounter* ymaxCounter;
    QwtCounter* xminCounter;
    QwtCounter* xmaxCounter;

    QCheckBox* autoscaleCheck;

    void clearData();


private slots:
    void slotRangeChanged(double);
    void slotYmaxChanged(double);
    void slotYminChanged(double);
    void slotXmaxChanged(double);
    void slotXminChanged(double);
    void slotAutoscaleToggled();

};

extern "C" {
scopePlotPlugin *createPlugin (QTScope* caller, 
			       QWidget* parent, 
			       const char* name, 
			       int id, 
			       Qt::WindowFlags wflags,
			       int numberOfSamples);
}

#endif
