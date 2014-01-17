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

#ifndef NEWVIEWDIALOG_H
#define NEWVIEWDIALOG_H

#include "qtscope.h"

#include <QDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>


//Added by qt3to4:
#include <Q3ValueList>
#include <Q3PtrList>
class Q3ButtonGroup;

class QSpinBox;
class QFrame;

/**
This widget provides the user interaction to open a new plot plugin.

@author Matthias H. Hennig
*/
class newViewDialog : public QDialog
{
Q_OBJECT

private:
  // store a copy of the plugin data here
  Q3ValueList <pluginData> pl;

  // Layout elements
  QVBoxLayout *mainLayout;
  QHBoxLayout *chooserBox;
  QHBoxLayout *buttonBox;
  Q3ButtonGroup *channelsList;
  QVBoxLayout *channelsListL;
  QListWidget *pluginsList;
  QFrame *f1;
  // the spinboxes to choose the channels
  Q3PtrList <QSpinBox> channelSelectors;

  QPushButton *okPushButton;
  QPushButton *cancelPushButton;
  // the calling class, to run QTScope::runPlugin there
  QTScope *caller;

  // number of channels the plugin requests
  unsigned int maxSelect;
  // number of channels available
  unsigned int maxChannels;

public:
    newViewDialog(int numChannels, Q3ValueList <pluginData> availablePlugins, QTScope *c, QWidget *parent = 0, const char *name = 0, bool modal = TRUE, Qt::WFlags f = 0);
    ~newViewDialog();

private slots:
    void accept();

public slots:
    void slotPluginSelected();

};

#endif
