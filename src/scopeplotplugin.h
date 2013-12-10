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
#ifndef SCOPEPLOTPLUGIN_H
#define SCOPEPLOTPLUGIN_H

#include <QDockWidget>
#include <qstring.h>
#include <QCloseEvent>

// for the data formats
#include <comedilib.h>

class QTScope;

typedef struct pluginInfo{
  QString name;
  QString type;
  int channelsRequest;
  int type_comedi;
};

/**
This is the base class for all plot widgets, implemented as plugins.


@author Matthias H. Hennig, Bernd Porr
*/
class scopePlotPlugin : public QDockWidget
{
Q_OBJECT

public:
	scopePlotPlugin(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples);
	~scopePlotPlugin() {};

	virtual void setDataSource(sampl_t**, double **) = 0;
	virtual void insertValues(int n,int append=1) = 0;
	virtual void replot() = 0;

	void closeEvent( QCloseEvent * );

 protected:
	QTScope* callingWidget;
	int idThis;

 signals:
	void signalClosed(int);

 public slots:
	void slotSetSamplingRate(double f);
	
 protected:
	virtual void samplingRateChanged() = 0;
	
 protected:
	double freq;
};

#endif
