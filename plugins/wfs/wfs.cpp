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

#include <iostream>

pluginInfo myPluginInfo = {"WFS Plugin", "Plot", 1};

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
	
	resize(400,200);
	
	d_plot = new Plot(this);

    setWidget(d_plot);
    
    setWindowTitle("Wavefront Sensor plugin");
    
	show();
}

QSize wfs::sizeHint() const
{
	return QSize(100, 110);
	cout << "getting called\n";
}

wfs::~wfs()
{
}


