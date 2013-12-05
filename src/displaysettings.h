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

#ifndef DISPLAYSETTINGS_H
#define DISPLAYSETTINGS_H

#include <qvariant.h>
#include <q3mainwindow.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3GridLayout>
#include <QCloseEvent>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QTabWidget;
class QWidget;
class Q3GroupBox;
class QLabel;
class QSpinBox;
class QPushButton;

class displaySettings : public Q3MainWindow
  {
    Q_OBJECT

  public:
    displaySettings( QWidget* parent, const char* name, int screenr, int readr  );
    ~displaySettings();

    void closeEvent( QCloseEvent * );

  private:

    QTabWidget* tabs;
    QWidget* tab;
    Q3GroupBox* groupBox1;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QSpinBox* continousSpeedBox;
    QSpinBox* burstSpeedBox;
    QPushButton* applyButton;
    Q3Frame* frame5;
    Q3VBoxLayout* testMainWindowLayout;
    Q3VBoxLayout* frame5Layout;
    Q3HBoxLayout* tabLayout;
    Q3GridLayout* groupBox1Layout;

  private slots:
    virtual void slotApply();

  signals:
    void signalIntervalsChanged(int,int);
  };

#endif // DISPLAYSETTINGS_H
