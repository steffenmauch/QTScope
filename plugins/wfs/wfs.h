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
#ifndef WFS_H
#define WFS_H

#include <scopeplotplugin.h>
#include "plot.h"
#include "gradientPlot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QPushButton>

/**
@author Steffen Mauch
*/
class wfs : public scopePlotPlugin
{
Q_OBJECT

public:
	wfs(QTScope* caller, 
	      QWidget* parent, 
	      const char* name, 
	      int id, 
	      Qt::WindowFlags wflags, 
	      int numberOfSamples);
    ~wfs();

    void setDataSource(sampl_t**, double **) {}
    void insertValues(int, int) {}
    void replot() {}
    void samplingRateChanged() {}
    
    void getComedi(comedi_t *comediDevice);

   	virtual QSize sizeHint() const;
     
private:
    Plot *d_plot;
    gradientPlot *gradient_plot;
    QVBoxLayout *vbox_layout;
    QHBoxLayout *hbox_layout;
    
    QPushButton *en_contour;
    QPushButton *en_nullref;
    QPushButton *clear_nullref;
    
    // comedi specific variables
    comedi_t *comediDevice;
    int comediSubdevice;

    // the comedi command
    comedi_cmd *cmd;
    
    QTimer *timer;

private slots:
    void enableCountour();
    void clearReference();
    void setReference();
    void slotReadData();
    
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
