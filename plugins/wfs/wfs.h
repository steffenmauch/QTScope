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
    void samplingRateChanged() {};

   	virtual QSize sizeHint() const;
   	
private:
    Plot *d_plot;
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
