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

#include "scopeplotplugin.h"

#include "qtscope.h"
#include <QCloseEvent>
#include <QDebug>

#include <iostream>

using std::cout;

scopePlotPlugin::scopePlotPlugin(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int)
{
	idThis = id;
	// prevent division by zero
	freq=1;
	
	cout << "plugin: " << idThis << " created\n";
	
	connect(caller, SIGNAL(signalSamplingRateChanged(double)), this, SLOT( slotSetSamplingRate(double)));
}

void scopePlotPlugin::closeEvent( QCloseEvent* ce ) 
{
	cout << "plugin: " << idThis << " closing\n";
	// notify the calling widget is neccessary
	emit signalClosed( idThis );
	// exit
	ce->accept();
	return;
}

void scopePlotPlugin::slotSetSamplingRate(double f) 
{
	freq=f;
	// notify the child
	samplingRateChanged();
}

