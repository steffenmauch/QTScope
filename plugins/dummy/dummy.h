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
#ifndef DUMMY_H
#define DUMMY_H

#include <scopeplotplugin.h>

/**
@author Matthias H. Hennig, Bernd Porr
*/
class dummy : public scopePlotPlugin
{
Q_OBJECT

public:
	dummy(QTScope* caller, 
	      QWidget* parent, 
	      const char* name, 
	      int id, 
	      Qt::WindowFlags wflags, 
	      int numberOfSamples);
    ~dummy();

    void setDataSource(sampl_t**, double **) {}
    void insertValues(int, int) {}
    void replot() {}
    void samplingRateChanged() {};

   	virtual QSize sizeHint() const;

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
