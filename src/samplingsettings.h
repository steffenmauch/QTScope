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

#ifndef SAMPLINGSETTINGS_H
#define SAMPLINGSETTINGS_H

#include <QDockWidget>
#include <QLabel>
#include <QCheckBox>
#include <QCloseEvent>
#include <QVariant>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>


class QVBoxLayout;
class QGridLayout;
class QTabWidget;
class QWidget;
class QGroupBox;
class QLabel;
class QSpinBox;
class QPushButton;

class samplingSettings : public QWidget
  {
    Q_OBJECT

  public:
	  samplingSettings( QWidget* parent, const char* name, double f, int ch,int nSampl,int continous);
	  ~samplingSettings();
    
    void closeEvent( QCloseEvent * );

  private:

    QGroupBox* groupBox1;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel4;
    QSpinBox* samplingRateBox;
    QSpinBox* numberOfChannelsBox;
    QSpinBox* numberOfSamplesBox;
    QCheckBox* continousCheckBox;
    QPushButton* applyButton;
    QVBoxLayout* frame5Layout;
    QGridLayout* groupBox1Layout;

  private slots:
    virtual void slotApply();

  signals:
    void signalSamplingSettingsChanged(double,int,int,int);
  };

#endif // SAMPLINGSETTINGS_H
