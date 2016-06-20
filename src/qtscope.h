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
 
#ifndef QTSCOPE_H
#define QTSCOPE_H

#define TRUE  1
#define FALSE 0

#include "scopeplotplugin.h"

// Qt related includes
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QSettings>
#include <QCloseEvent>
#include <QList>
#include <QLabel>

// data aquisition via comedi
#include <comedilib.h>

// maximum number of channels
#define N_CHANS 256

// sampling rate after startup
#define INITIAL_SAMPLING_RATE 1000 // Hz

// threshold between scrolling and burst sampling
#define FREQU_THRES_CONT 10000 // Hz

// number of samples for one cmd
#define INITIAL_NUMBER_OF_SAMPLES 1000

// number of channels
#define INITIAL_NUMBER_OF_CHANNELS 1

// initial range setting
#define INITIAL_RANGE 0

// default interval for the readout timer
#define CONTINOUS_TIMER_INTERVAL 100
#define BURST_TIMER_INTERVAL 1000

#define PLOT_WINDOW_INACTIVE 0
#define PLOT_WINDOW_XTPLOT 1

class QWorkspace;
class QCheckBox;
class QTScope;
class QPushButton;

// the function that will return a plugin object
typedef scopePlotPlugin* ( *CreateP )(QTScope* , QWidget* , const char* , int , Qt::WindowFlags, int);

struct pluginData
  {
    CreateP create;
    QString name;
    QString type;
    int numChannels;
    int type_comedi;
  };

struct dataTarget
  {
    QList<int> channels;
    int id;
    scopePlotPlugin *target;
  };


class QTScope: public QMainWindow
  {
    Q_OBJECT

  private slots:
    void newView();

    void about();
    void aboutQt();

    void windowsMenuAboutToShow();
    void windowsMenuActivated();

    // function to replot data
    void slotReplotPlots();
    // function to read out data from the daq device
    void slotReadData();
    void tileVertical();
    
	// save settings
    void saveSettings();
    
    void loadPluginPath();

    // open some dialogs to make settings
    void slotConfigure();
    void slotDisplaySettings();
    void slotSamplingSettings();

  public slots:
    void slotChannelClosed( int  );
    void slotSetIntervals(int c,int b);
    void slotSetSamplingSettings(double f,int,int,int);

    void slotSlower();
    void slotFaster();

  signals:
    void signalSamplingRateChanged(double);

  public:
    QTScope();
    ~QTScope();

    // start a new plugin process
    int runPlugin(QString name, int *channels);

    // the possible locations of plugins
    QStringList pluginPath;

  protected:
    void closeEvent( QCloseEvent* );

  private:
    // program properties
    QSettings settings;
    // main Workspace that contains all plot windows
    QMdiArea* ws;
    QMenu* windowsMenu;

    QLabel* labelTimebase;

    // timer for screen refresh
    QTimer *readoutTimer;

    int continousInterval;
    int burstInterval;

    // Size of all data buffers in samples
    int numberOfSamples;

    void showSamplingrate();

    // PLUGIN MANAGEMENT
    //-------------------

    // collect available plugin data
    QList<pluginData> availablePlugins;

    // store information about running plugins
    QList<dataTarget> activePlugins;

    // increment
    int pCount;

    // load plugins
    int initPlugins();

    // starts the timers
    void startTimers();

    // COMEDI
    //--------

    // this method sets up communication with the comedi device
    void initComedi();

    // tests of command makes sense
    void testComedi();

    // starts data acqu
    void startComedi();

    void stopTimers();

    // stops timers and comedi
    void stopAll(); 

    void initBuffers();
    void freeBuffers();
    void tellPlugins();

    // flag is cmd is running
    int comediRunning;

    // comedi specific variables
    comedi_t *comediDevice;
    int comediSubdevice;
    // buffer for the data read from device
    sampl_t *dataBuffer;
    // buffer for the raw data in a nice 2D array
    sampl_t **rawDataBuffer;
    // buffer for converted data
    double **convDataBuffer;

    // store range information here
    comedi_range *cRange;

    // sampling rate
    double freq;

    // range selection
    int range;

    // the comedi command
    comedi_cmd *cmd;

    // the filename of the comedi device
    char *comediFilename;

    // name of the board
    QString boardname;

    // the reference
    int aref;

    // the number of channels
    int n_chan;
    
    // the max number of channels
    int max_chan;

    // the channel list
    unsigned int chanlist[N_CHANS];

    // is cmd always running or only every timer interrupt?
    int continous;

    // error condition
    int comediError;
    
    // max numberical value from comedi
    sampl_t maxdata;
  };


#endif
