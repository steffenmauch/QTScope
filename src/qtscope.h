#ifndef QTSCOPE_H
#define QTSCOPE_H

#include "scopeplotplugin.h"

// Qt related includes
#include <qmainwindow.h>
#include <qptrlist.h>
#include <qsettings.h>
#include <qlabel.h>


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
class QFrame;
class QVBoxLayout;
class QCheckBox;
class QTScope;
class QVButtonGroup;
class QDockWindow;
class QPushButton;
class ChooserWindow;

// the function that will return a plugin object
typedef scopePlotPlugin* ( *CreateP )(QTScope* , QWidget* , const char* , int , int, int);

typedef struct pluginData
  {
    CreateP create;
    QString name;
    QString type;
    int numChannels;
  };

typedef struct dataTarget
  {
    QValueList <int> channels;
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
    void windowsMenuActivated( int id );

    // function to replot data
    void slotReplotPlots();
    // function to read out data from the daq device
    void slotReadData();
    void tileVertical();

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
    QString filename;
    // main Workspace that contains all plot windows
    QWorkspace* ws;
    QPopupMenu* windowsMenu;

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
    QValueList <pluginData> availablePlugins;

    // store information about running plugins
    QPtrList <dataTarget> activePlugins;

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
