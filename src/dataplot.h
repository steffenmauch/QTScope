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
#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <qmainwindow.h>
#include <qwt_plot.h>

#include <comedilib.h>

#define MAX_PLOT_LENGTH 2000

class QwtCounter;
class QCheckBox;
class QTScope;

/**
This class provides a generic plot widget along with some opportunity to control a number of parameters. It utilizes QwtPlot from the Qwt Widget Library.

@author Matthias H. Hennig
*/
class DataPlot : public QMainWindow
  {
    Q_OBJECT

  public:
    DataPlot(QTScope* caller, QWidget* parent, const char* name, int id, int wflags);

    ~DataPlot();
    void insertValue(double v);
    void clearData();
    void replot();

  protected:
    void closeEvent( QCloseEvent * );

  signals:
    void message(const QString&, int );

  private:

    QTScope* callingWidget;
    int idThis;

    // here the data will be sent
    QwtPlot *plotWidget;

    // store curve
    long curve;

    // the data to be plotted is stored here
    double x[MAX_PLOT_LENGTH], y[MAX_PLOT_LENGTH];

    // the length (x-axis) of the plot
    int plotLength;

    QwtCounter* xrangeCounter;
    QwtCounter* yminCounter;
    QwtCounter* ymaxCounter;

    QCheckBox* autoscaleCheck;

private slots:
    void slotXrangeChanged(double);
    void slotYmaxChanged(double);
    void slotYminChanged(double);
    void slotAutoscaleToggled();

  };

#endif
