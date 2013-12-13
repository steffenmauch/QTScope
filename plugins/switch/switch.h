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
#ifndef SWITCH_H
#define SWITCH_H

#include <scopeplotplugin.h>
#include <QCheckBox>
#include <QVector>
#include <QHBoxLayout>
#include <QPushButton>

/**
@author Steffen Mauch
*/
class switchPlugin : public scopePlotPlugin
{
Q_OBJECT

public:
	switchPlugin(QTScope* caller, 
	      QWidget* parent, 
	      const char* name, 
	      int id, 
	      Qt::WindowFlags wflags, 
	      int numberOfSamples);
    ~switchPlugin();

    void setDataSource(sampl_t**, double **) {}
    void insertValues(int, int) {}
    void replot() {}
    void samplingRateChanged() {};
    void getComedi(comedi_t *comediDevice) {};

   	virtual QSize sizeHint() const;
   	
private:
	QCheckBox *cb ;
	QVector<QCheckBox*> *list;
	QVBoxLayout *vbox_layout;
    QWidget *window;
    QPushButton *save;
    
private slots:
    void saveCheckBoxes();

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
