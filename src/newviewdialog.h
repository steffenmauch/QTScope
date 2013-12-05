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

#include <qdialog.h>

class QHBoxLayout;
class QListBox;
class QButtonGroup;
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
  QValueList <pluginData> pl;

  // Layout elements
  QVBoxLayout *mainLayout;
  QHBoxLayout *chooserBox;
  QHBoxLayout *buttonBox;
  QButtonGroup *channelsList;
  QVBoxLayout *channelsListL;
  QListBox *pluginsList;
  QFrame *f1;
  // the spinboxes to choose the channels
  QPtrList <QSpinBox> channelSelectors;

  QPushButton *okPushButton;
  QPushButton *cancelPushButton;
  // the calling class, to run QTScope::runPlugin there
  QTScope *caller;

  // number of channels the plugin requests
  unsigned int maxSelect;
  // number of channels available
  unsigned int maxChannels;

public:
    newViewDialog(int numChannels, QValueList <pluginData> availablePlugins, QTScope *c, QWidget *parent = 0, const char *name = 0, bool modal = TRUE, WFlags f = 0);
    ~newViewDialog();

private slots:
    void accept();

public slots:
    void slotPluginSelected(const QString &name);

};

#endif
