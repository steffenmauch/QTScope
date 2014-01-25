/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *                                                                         *
 *   porting to QT4 + improvements                                         *
 *   Copyright (C) 2014 by Steffen Mauch                                   *
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

#ifndef NEWVIEWDIALOG_H
#define NEWVIEWDIALOG_H

#include "qtscope.h"

#include <QDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QSpinBox>
#include <QList>

/**
This widget provides the user interaction to open a new plot plugin.

@author Matthias H. Hennig
*/
class newViewDialog : public QDialog
{
Q_OBJECT

private:
  // store a copy of the plugin data here
  QList <pluginData> pl;

  // Layout elements
  QVBoxLayout *mainLayout;
  QVBoxLayout *main;
  QHBoxLayout *chooserBox;
  QHBoxLayout *buttonBox;
  QWidget *channelsList;
  QWidget *channelsListWidget;
  QVBoxLayout *channelsListLayout;
  QVBoxLayout *channelsListL;
  QListWidget *pluginsList;
  QFrame *f1;
  // the spinboxes to choose the channels
  QList<QSpinBox *> channelSelectors;

  QPushButton *okPushButton;
  QPushButton *cancelPushButton;
  // the calling class, to run QTScope::runPlugin there
  QTScope *caller;

  // number of channels the plugin requests
  int maxSelect;
  // number of channels available
  int maxChannels;

public:
    newViewDialog(int numChannels, QList<pluginData> availablePlugins, QTScope *c, QWidget *parent = 0, const char *name = 0, bool modal = TRUE, Qt::WFlags f = 0);
    ~newViewDialog();

private slots:
    void accept();

public slots:
    void slotPluginSelected();

};

#endif
