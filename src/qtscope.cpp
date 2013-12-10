/***************************************************************************
 *   Copyright (C) 2004 by Matthias H. Hennig                              *
 *   hennig@cn.stir.ac.uk                                                  *
 *   Copyright (C) 2005 by Bernd Porr                                      *
 *   Bernd Porr, BerndPorr@f2s.com                                         *
 *   Copyright (C) 2013 by Steffen Mauch                                   *
 *   Steffen Mauch, steffen.mauch@gmail.com                                *
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

#include "qtscope.h"
#include <QMainWindow>
//Added by qt3to4:
#include <Q3ValueList>
#include <QLabel>
#include <QCloseEvent>
#include "newviewdialog.h"
#include "propertiesdialog.h"
#include "displaysettings.h"
#include "samplingsettings.h"


#include <unistd.h>
#include <dlfcn.h>

#include <iostream>

#include <math.h>

#include <qimage.h>
#include <qpixmap.h>
#include <QToolBar>
#include <q3toolbar.h>
#include <qtoolbutton.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <q3textedit.h>
#include <qfile.h>
#include <q3filedialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <q3accel.h>
#include <q3textstream.h>
#include <qtimer.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <QWhatsThis>
#include <q3vbox.h>
#include <qworkspace.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qstring.h>
#include <q3buttongroup.h>
#include <q3dockwindow.h>
#include <qpushbutton.h>
#include <QStringList>
#include <QDebug>

// icons
#include "fileopen.xpm"
#include "slower.xpm"
#include "faster.xpm"

using std::cout;
using std::cerr;

// the minimum time between two burst measurements
// and also the minimum time beween the end of a measurement
// and the next one.
#define MIN_BURST 200

QTScope::QTScope() : QMainWindow( 0, "QTScope", Qt::WDestructiveClose )
{
	continous=0;
	comediError=0;

	QStringList cmdline_args = QCoreApplication::arguments();
		
	comediFilename = "/dev/comedi0";
	// if first command line argument is available use it as 
	// device name for comedi_open
	// i.e. sudo ./bin/qtscope /dev/comedi2
	if( cmdline_args.size() == 2 ){
		QString str = QString( cmdline_args.at(1) );
		QByteArray *ba = new QByteArray( str.toLocal8Bit() );
		comediFilename = ba->data();
    }		

	comediDevice = comedi_open(comediFilename);
	if(!comediDevice) {
		comedi_perror(comediFilename);
		exit(-1);
	}
	// load settings
	settings.setPath( "github.com/steffenmauch/QTScope", "qtscope" );
	boardname=QString("/")+comedi_get_board_name(comediDevice)+QString("/");
	comedi_close(comediDevice);

	settings.beginGroup( "/qtscope/"+boardname+"/" );
	numberOfSamples =  settings.readNumEntry( "/sampling/nsamples", INITIAL_NUMBER_OF_SAMPLES );
	continousInterval = settings.readNumEntry( "/timing/continous", CONTINOUS_TIMER_INTERVAL );
	burstInterval = settings.readNumEntry( "/timing/burst", BURST_TIMER_INTERVAL );
	freq = settings.readNumEntry( "/sampling/rate", INITIAL_SAMPLING_RATE );
	continous=settings.readNumEntry( "/sampling/continous",freq<FREQU_THRES_CONT);
	n_chan =  settings.readNumEntry( "/sampling/nchan", INITIAL_NUMBER_OF_CHANNELS );
	range = settings.readNumEntry( "/sampling/range", INITIAL_RANGE );
	aref = settings.readNumEntry( "/sampling/aref", AREF_GROUND );

	settings.endGroup();

	// get plugin paths
	settings.beginGroup( "/qtscope/" );
	// window size
	int width = settings.readNumEntry( "/geometry/width", 800 );
	int height = settings.readNumEntry( "/geometry/height", 600 );
	resize( width, height );
	QStringList pk = settings.readListEntry( "/pluginpath" );
	settings.endGroup();

	QStringList::Iterator it = pk.begin();
	while( it != pk.end() )
		{
			pluginPath.append(*it);
			++it;
		}
	// no preferences available, set default
	if(pluginPath.size() == 0)
		{
			pluginPath.append("~/qtscope/plugins");
			pluginPath.append("~/.qtscope/plugins");
			pluginPath.append("/usr/local/lib/qtscope/plugins");
			pluginPath.append("/usr/lib/qtscope/plugins");
		}
		
	ws = new QMdiArea();
	setCentralWidget( ws );
	
	ws->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ws->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	// the main toolbar, for file operations etc.
	QToolBar * fileTools = addToolBar( "file operations" );
	//fileTools->setLabel( tr("File Operations") );

	// add an open icon
	QPixmap openIcon = QPixmap( fileopen );
	QToolButton * channelOpen = new QToolButton( openIcon, tr("New Plot Window"),
						     QString::null, this, SLOT(newView()), fileTools, "open channel" );
	fileTools->addWidget( channelOpen );
	QString channelOpenText = tr("<p>This button opens a new plot window.</p>");
	QWhatsThis::add( channelOpen, channelOpenText );
	Q3MimeSourceFactory::defaultFactory()->setPixmap( "fileopen", openIcon );

	fileTools->addSeparator();
	// slower timebase
	QPixmap slowerIcon = QPixmap( slower_xpm );
	QToolButton *lowRate = new QToolButton( slowerIcon, tr("lower sampling rate"),
			 QString::null, this, SLOT(slotSlower()), 
			 fileTools, "lower sampling rate" );
	fileTools->addWidget( lowRate );
	Q3MimeSourceFactory::defaultFactory()->setPixmap( "lower sampling rate", slowerIcon );

	// timebase
	labelTimebase=new QLabel(fileTools,"timebase");
	fileTools->addWidget( labelTimebase );
	
	// faster timebase
	QPixmap fasterIcon = QPixmap( faster_xpm );
	QToolButton *highRate = new QToolButton( fasterIcon, tr("higher sampling rate"),
			 QString::null, this, SLOT(slotFaster()), 
			 fileTools, "higher sampling rate" );
	fileTools->addWidget( highRate );
	Q3MimeSourceFactory::defaultFactory()->setPixmap( "higher sampling rate", fasterIcon );

	fileTools->addSeparator();

	// the file menu
	QMenu *file = menuBar()->addMenu( tr("&File") );
	file->addAction( tr("&New Plot Window"), this, SLOT(newView()), Qt::CTRL+Qt::Key_N );
	file->insertSeparator();
	file->addAction(tr("&Quit"), qApp, SLOT( closeAllWindows() ), Qt::CTRL+Qt::Key_Q );

	// the settings menu
	QMenu *set = menuBar()->addMenu( tr("&Settings") );
	set->addAction( tr("&Refresh rates"), this, SLOT(slotDisplaySettings()), Qt::CTRL+Qt::Key_R );
	set->addAction( tr("&Sampling Settings"), this, SLOT(slotSamplingSettings()), Qt::CTRL+Qt::Key_S );
	set->addAction( tr("&Plugin Paths"), this, SLOT(slotConfigure()), Qt::CTRL+Qt::Key_P );

	// the windows menu to provide some functions to arrange windows
	windowsMenu = new QMenu( );
	windowsMenu->setCheckable( TRUE );
	connect( windowsMenu, SIGNAL( aboutToShow() ),  this, SLOT( windowsMenuAboutToShow() ) );
	menuBar()->insertItem( "&Windows", windowsMenu );
	menuBar()->insertSeparator();

	// Help menu
	QMenu *help = menuBar()->addMenu(tr("&Help"));
	help->addAction( tr("&About"), this, SLOT(about()), Qt::Key_F1 );
	help->addAction( tr("About &Qt"), this, SLOT(aboutQt()) );
	help->insertSeparator();
	QAction *whatsThisAct = QWhatsThis::createAction();
	help->addAction( whatsThisAct );

	// create the workspace

	//vb->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
	fileTools->setStyleSheet( "border-style: sunken; " );

	// allow Docking
	//setDockEnabled ( Qt::DockTop, TRUE );
	//setDockEnabled ( Qt::DockLeft, TRUE );

	// load all plugins
	initPlugins();

	readoutTimer=NULL;

	initComedi();
	initBuffers();
	testComedi();
	startTimers();
	startComedi();

	showSamplingrate();
	statusBar()->message( tr("Ready"), 2000 );
}


QTScope::~QTScope() {
	freeBuffers();
}


void QTScope::startTimers() {
	// Generate timer event to read data
	readoutTimer = new QTimer(this);
	connect( readoutTimer, SIGNAL(timeout()), this, SLOT(slotReadData()) );
	if (continous) {
		readoutTimer->start( continousInterval );
		cerr << "Timer: " << continousInterval << " ms\n";
	} else {
		// if the sampling needs longer than the
		// display interval we increase the display
		// interval.
		int t=(int)(numberOfSamples*1000.0/freq);
		if (t>(burstInterval-MIN_BURST)) {
			burstInterval=t+MIN_BURST;
			cerr << "Adjusting burst interval.\n";
		}
		readoutTimer->start( 	 burstInterval ); //ms
		cerr << "Burst interval: " << burstInterval << " ms\n";
	}
}


void QTScope::stopTimers() {
	// stop timer
	if (readoutTimer) {
		readoutTimer->stop();
		delete readoutTimer;
		readoutTimer=NULL;
	}
}


void QTScope::slotSetIntervals(int c,int b) {
	stopTimers();
	continousInterval=c;
	burstInterval=b;
	startTimers();
}


void QTScope::stopAll() {
	// stop the data acquisition
	comedi_cancel(comediDevice,comediSubdevice);
	// stop the display timers
	stopTimers();
	// free the comedi devics
	comedi_close(comediDevice);

	if (cmd) {
		delete cmd;
		cmd=NULL;
	}
	return;
}


void QTScope::freeBuffers() {
	if (dataBuffer) {
		delete dataBuffer;
		dataBuffer=NULL;
	}

	if (convDataBuffer) {
		for(int i=0; i<max_chan; i++) {
			delete convDataBuffer[i];
			convDataBuffer[i]=NULL;
		}
		convDataBuffer=NULL;	
	}
	if (rawDataBuffer) {
		for(int i=0; i<max_chan; i++) {
			delete rawDataBuffer[i];
			rawDataBuffer[i]=NULL;
		}
		rawDataBuffer=NULL;	
	}
}


void QTScope::closeEvent( QCloseEvent* ce )
{
	dataTarget *it;

	stopAll();

	// announce close to active plugins
	while ( (it = activePlugins.first()) )
		it->target->close();

	settings.beginGroup( "/qtscope/" );
	settings.writeEntry( "/pluginpath", pluginPath);
	settings.writeEntry( "/geometry/width", this->width() );
	settings.writeEntry( "/geometry/height", this->height() );
	settings.endGroup( );

	// save settings
	settings.beginGroup( "/qtscope/"+boardname+"/" );
	settings.writeEntry( "/timing/continous", continousInterval );
	settings.writeEntry( "/timing/burst", burstInterval );
	settings.writeEntry( "/sampling/range", range );
	settings.writeEntry( "/sampling/aref", aref );
	char tmp[256];
	sprintf(tmp,"%9.0f",freq);
	settings.writeEntry( "/sampling/rate", tmp );
	settings.writeEntry( "/sampling/continous",continous);
	settings.writeEntry( "/sampling/nchan", n_chan );
	settings.endGroup( );

	ce->accept();
	return;
}

/*!
  \fn ChooserWindow::slotChannelClosed()

  This removes the plugin information when a plugin window has been closed.
*/
void QTScope::slotChannelClosed(int id)
{
	dataTarget *it;
	for ( it = activePlugins.first(); it; it = activePlugins.next() )
		if( it->id == id)
			{
				activePlugins.remove();
				continue;
			}
}

void QTScope::slotReadData()
{
	dataTarget *it;
	int i, num,ret;

	if (comediError) {
		return;
	}

	if ((!dataBuffer)||(!convDataBuffer)||(!rawDataBuffer)) {
		return;
	}

	// count the data points
	num = 0;

	if (continous) { // continous data
		// read data from the device
		while (comedi_get_buffer_contents(comediDevice,comediSubdevice)) {
			// read data from device
			if ((ret=read(comedi_fileno(comediDevice), 
				      dataBuffer, 
				      n_chan*sizeof(sampl_t))) <= 0) {
				cerr << "Acquisition error: " << ret << "\n";
				comediError=-1;
				return;
			}
			if( num < numberOfSamples ) {
				// fill the data buffer
				for( i = 0; i < n_chan; i++) {
					rawDataBuffer[i][num] = dataBuffer[i];
					convDataBuffer[i][num] = comedi_to_phys(dataBuffer[i],
										cRange,
										maxdata);
				}
				num++;
			} else {
				cout << "Data buffer overflow! Falling back to burst mode.\n";
				stopAll();
				initComedi();
				continous=0;
				testComedi();
				tellPlugins();
				startTimers();
				startComedi();
				return;
			}
			cout.flush();
		}
		// announce data to plugins
		if(activePlugins.count() != 0) {
			for ( it = activePlugins.first(); it; it = activePlugins.next() ) {
				it->target->insertValues(num,TRUE);
			}
		}
		slotReplotPlots();
	} else { // just bursts of data
		while(1) {
			ret=read(comedi_fileno(comediDevice),
				 dataBuffer,
				 numberOfSamples*sizeof(sampl_t)*n_chan);
			if(ret<0){
				fprintf(stderr,"Acquisition error=%d\n",ret);
				break;
			}else if(ret==0){
				/* reached stop condition */
				break;
			}else{
				int ch=0;
				for(unsigned int i=0;i<(ret/sizeof(sampl_t));i++){
					//cerr << num << "\n";
					rawDataBuffer[ch][num] = dataBuffer[i];
					convDataBuffer[ch][num] =comedi_to_phys(dataBuffer[i],
										cRange,
										maxdata);
					ch++;
					if (ch==n_chan) {
						ch=0;
						num++;
					}
				}
			}
		}
		// announce data to plugins
		if(activePlugins.count() != 0) {
			for ( it = activePlugins.first(); it; it = activePlugins.next() ) {
				it->target->insertValues(num,FALSE);
			}
		}
		slotReplotPlots();
		startComedi();
	}
}

void QTScope::slotReplotPlots()
{
	dataTarget *it;

	if(activePlugins.count() == 0)
		return;

	for ( it = activePlugins.first(); it; it = activePlugins.next() )
		it->target->replot();
}

void QTScope::newView()
{
	newViewDialog *gv = new newViewDialog(n_chan, availablePlugins, this, this);
	gv->exec();
}


void QTScope::windowsMenuAboutToShow()
{
	windowsMenu->clear();
	int cascadeId = windowsMenu->insertItem("&Cascade", ws, SLOT(cascade() ) );
	int tileId = windowsMenu->insertItem("&Tile", ws, SLOT(tile() ) );
	int verTileId = windowsMenu->insertItem("Tile &Vertical", this, SLOT(tileVertical() ) );
	#if 0
	if ( ws->windowList().isEmpty() ) {
		windowsMenu->setItemEnabled( cascadeId, FALSE );
		windowsMenu->setItemEnabled( tileId, FALSE );
		windowsMenu->setItemEnabled( verTileId, FALSE );
	}
	
	windowsMenu->insertSeparator();
	QWidgetList windows = ws->windowList();
	for ( int i = 0; i < int(windows.count()); ++i ) {
		int id = windowsMenu->insertItem(windows.at(i)->caption(),
						 this, SLOT( windowsMenuActivated( int ) ) );
		windowsMenu->setItemParameter( id, i );
		windowsMenu->setItemChecked( id, ws->activeWindow() == windows.at(i) );
	}
	#endif
}

void QTScope::windowsMenuActivated( int id ) {
	#if 0
	QWidget* w = ws->windowList().at( id );
	if ( w )
		w->showNormal();
	w->setFocus();
	#endif
}

void QTScope::tileVertical()
{
	#if 0
	// primitive horizontal tiling
	QWidgetList windows = ws->windowList();
	if ( !windows.count() )
		return;

	int heightForEach = ws->height() / windows.count();
	int y = 0;
	for ( int i = 0; i < int(windows.count()); ++i ) {
		QWidget *window = windows.at(i);
//		if ( window->testWState( WState_Maximized ) ) {
//			// prevent flicker
//			window->hide();
//			window->showNormal();
//		}
		int preferredHeight = window->minimumHeight()+window->parentWidget()->baseSize().height();
		int actHeight = QMAX(heightForEach, preferredHeight);

		window->parentWidget()->setGeometry( 0, y, ws->width(), actHeight );
		y += actHeight;
	}
	#endif
}

void QTScope::about()
{
    QMessageBox::about( this, tr("QTScope"), 
		tr("Version "+QString(VER)+"\n"+
		"\n"+QString(DATE)+"\n\n"+
		"by Mattias Hennig\nhennig@cn.stir.ac.uk,\n"+
		"\nBernd Porr\nBerndPorr@f2s.com\n"+
		"\nand\n\nSteffen Mauch\nsteffen.mauch@gmail.com\n"+
		"\n...a simple DAQ program for comedi devices...\n")
	);
}


void QTScope::aboutQt()
{
	QMessageBox::aboutQt( this, tr("QTScope") );
}


/*!
  \fn QTScope::initComedi()
*/
void  QTScope::initComedi() {
	aref = AREF_GROUND;

	comediDevice = comedi_open(comediFilename);
	if(!comediDevice) {
		comedi_perror(comediFilename);
		exit(-1);
	}
	comediSubdevice = comedi_find_subdevice_by_type(comediDevice,COMEDI_SUBD_AI,0);

	cmd=new comedi_cmd;
	if (!cmd) {
		cerr << "Could not allocate memory for the async command\n";
		exit(-1);
	}
			
	// get basic information about the hardware
	cout << "driver: " << comedi_get_driver_name(comediDevice) << "\n";
	cout << "board name: " << comedi_get_board_name(comediDevice) << "\n";
	max_chan = comedi_get_n_channels(comediDevice, comediSubdevice);
	maxdata = comedi_get_maxdata(comediDevice, comediSubdevice,0);

	cout << "channels: " << max_chan << "\n";

	// get range information
	cRange = comedi_get_range(comediDevice, comediSubdevice, 0, range);
	cout << "comedi initialized...\n";
}


void QTScope::initBuffers() {
	dataBuffer = new sampl_t[max_chan * numberOfSamples];
	convDataBuffer = new double*[max_chan];
	for(int i=0; i<max_chan; i++)
		convDataBuffer[i] = new double[numberOfSamples];
	rawDataBuffer = new sampl_t*[max_chan];
	for(int i=0; i<max_chan; i++)
		rawDataBuffer[i] = new sampl_t[numberOfSamples];
}


/*!
  \fn QTScope::testComedi()
*/
void QTScope::testComedi() {
	cerr << "Initialising the cmd structure\n";
	memset(cmd,0,sizeof(comedi_cmd));

	cerr << "Setting up the channel list\n";
	// channel list
	for(int i=0;i<n_chan;i++)
		chanlist[i]=CR_PACK(i,range,aref);

	cerr << "Requesting sampling rate: " << freq << "\n";
    int ret = comedi_get_cmd_generic_timed(comediDevice, comediSubdevice, cmd, 5, (unsigned int)(1e9/freq));
	if(ret<0) {
		cout << "comedi_get_cmd_generic_timed failed\n";
		return;
	}

	if ((cmd->convert_src ==  TRIG_TIMER)&&(cmd->convert_arg)) {
		freq=1E9 / cmd->convert_arg;
	}

	if ((cmd->scan_begin_src ==  TRIG_TIMER)&&(cmd->scan_begin_arg)) {
		freq=1E9 / cmd->scan_begin_arg;
	}

	if (continous) {
		cmd->stop_src = TRIG_NONE;
		cmd->stop_arg = 0;
		cerr << "Continous\n";
	} else {
		cmd->stop_src = TRIG_COUNT;
		cmd->stop_arg = numberOfSamples;
		cerr << "Not continous\n";
	}

	cerr << "Sampling rate now: " << freq << "\n";

	cerr << "Number of channels: " << n_chan << "\n";

	cmd->chanlist = chanlist;
	cmd->chanlist_len = n_chan;
	cmd->scan_end_arg = n_chan;

	ret = comedi_command_test(comediDevice,cmd);
	if(ret<0){
		comedi_perror("comedi_command_test");
		comediError=ret;
		return;
	}
	cerr << "first test returned " << ret <<"\n";
	ret = comedi_command_test(comediDevice,cmd);
	if(ret<0){
		comedi_perror("comedi_command_test");
		comediError=ret;
		return;
	}
	cerr << "second test returned " << ret <<"\n";

	if ((cmd->convert_src ==  TRIG_TIMER)&&(cmd->convert_arg)) {
		freq=1E9 / cmd->convert_arg;
	}

	if ((cmd->scan_begin_src ==  TRIG_TIMER)&&(cmd->scan_begin_arg)) {
		freq=1E9 / cmd->scan_begin_arg;
	}

	cerr << "Sampling rate now: " << freq << "\n";
}



void QTScope::startComedi() {
	int ret=comedi_command(comediDevice,cmd);
	if(ret<0) {
		comedi_perror("comedi_command");
	}
	return ;
}




/*!
  \fn QTScope::initPlugins()
  Scan for plugins
*/
int QTScope::initPlugins()
{
	const char *error;
	void *hndl;
	pluginInfo* pI;
	pluginData pD;
	QDir d;
	QDir pd;
	pCount = 0;

	d.setNameFilter("*.so");
	d.setSorting(QDir::Name);
	d.setFilter(QDir::Files);
	pd.setFilter(QDir::Dirs);

	cout << "Scanning for plugins...\n";

	// scan for plugins
	// locate proper directories in the source tree
	for ( QStringList::Iterator it = pluginPath.begin(); it != pluginPath.end(); ++it ) {
		pd.setPath(*it);
		//int counter = 0;
		for (uint i=0; i<pd.count(); i++) {
			// find plugin files
			d.setPath(pd.absFilePath(pd[i]));
			for (uint j=0; j<d.count(); j++) {
				cout << "found:" << d.absFilePath(d[j]).latin1() << "\n";
				hndl = dlopen(d.absFilePath(d[j]).latin1(), RTLD_LAZY);
				error = dlerror();
				if (error != NULL) {
					fprintf(stderr, "dlsym(message): %s\n", error);
					exit(1);
				} else {
					// read the plugin information and store them
					pI = reinterpret_cast< pluginInfo* > (reinterpret_cast< long >( dlsym( hndl, "myPluginInfo" )));
					if(pI) {
						cout << "plugin name: " << pI->name.ascii() << " comedi_type: " << pI->type_comedi << "\n";
						// get the plugin information
						pD.name = pI->name;
						pD.type = pI->type;
						pD.numChannels = pI->channelsRequest;
						pD.create = reinterpret_cast< CreateP >( reinterpret_cast< long >( dlsym( hndl, "createPlugin" ) ) );
						availablePlugins.append(pD);
					} else {
						cout << pI->name.ascii() << " is not a QTScope plugin\n";
					}
				}
			}
		}
		activePlugins.setAutoDelete( TRUE );
	}
	return 0;
}


/*!
  \fn QTScope::loadPlugin(QString path)
*/
int QTScope::runPlugin(QString name, int *channels)
{
	dataTarget *dT;
	double **dataSources;
	sampl_t **rawSources;

	Q3ValueList<pluginData>::iterator it;
	for ( it = availablePlugins.begin(); it != availablePlugins.end(); ++it )
		if( (*it).name == name)
			{
				// prepare plugin
				dT = new dataTarget;
				dT->target = (*it).create(this, 
							  ws, 
							  QString("%1").arg(channels[0]), 
							  pCount, 
							  Qt::WDestructiveClose,
							  numberOfSamples);
				dT->id = pCount;
				activePlugins.append(dT);

				dT->channels.clear();

				// provide pointers to data sources to the plugin
				dataSources = new double*[(*it).numChannels];
				rawSources = new sampl_t*[(*it).numChannels];
				for(int i=0; i<(*it).numChannels; i++)
					{
						dT->channels.append(channels[i]);
						dataSources[i] = &convDataBuffer[channels[i]][0];
						rawSources[i] = &rawDataBuffer[channels[i]][0];
					}
				dT->target->setDataSource(rawSources,dataSources);
				delete [] dataSources;

				// notify this->slotChannelClosed when the window has been closed
				connect(dT->target, SIGNAL(signalClosed(int)), this, SLOT(slotChannelClosed(int)));

				
				ws->addSubWindow( dT->target );
				dT->target->show();
				
				pCount++;
				continue;
			}
	tellPlugins();
	return 0;
}


/*!
  \fn QTScope::configure()
*/
void QTScope::slotConfigure()
{
	propertiesDialog *p = new propertiesDialog(this);
	p->exec();
}


/*!
  \fn QTScope::settings()
*/
void QTScope::slotDisplaySettings()
{
	displaySettings *s1 = new displaySettings(ws, "DisplaySettings", continousInterval, burstInterval);
	connect(s1, SIGNAL(signalIntervalsChanged(int,int)), this, SLOT( slotSetIntervals(int,int)));
	s1->show();
}

void QTScope::slotSamplingSettings()
{
	samplingSettings *s2 = new samplingSettings(ws, "SamplingSettings", freq, 
						    n_chan,
						    numberOfSamples,
						    continous);
	connect(s2, 
		SIGNAL( signalSamplingSettingsChanged(double,int,int,int)), this, 
		SLOT( slotSetSamplingSettings(double,int,int,int)));
	s2->show();
}


void QTScope::showSamplingrate() {
	char tmp[256];
	if (freq>=1e6) {
		sprintf(tmp,"%3.0f MHz",freq/1e6);
	} else {
		if (freq>=1e3) {
			sprintf(tmp,"%3.0f kHz",freq/1e3);
		} else {
			sprintf(tmp,"%3.0f Hz",freq);
		}
	}
	labelTimebase->setText(tmp);
}	


/*!
  \fn QTScope::slotSetNumberOfChannels(int i)
*/
void QTScope::slotSetSamplingSettings(double r,int n,int s,int c) {
	n_chan=n;
	freq=r;
	settings.writeEntry( "/sampling/nsamples", s );
	stopAll();
	initComedi();
	continous=c;
	testComedi();
	tellPlugins();
	startTimers();
	startComedi();
	showSamplingrate();
}

void QTScope::slotSlower() {
	char tmp[256];
	sprintf(tmp,"%d",((int)freq));
	double f;
	double fpot=pow(10,(double)(strlen(tmp)-1));
	cout << "fpot=" << fpot << "\n";
	if (*tmp=='1') {
		f=fpot/2;
	} else {
		if (*tmp=='2') {
			f=fpot;
		} else {
			f=fpot*2;
		}
	}
	slotSetSamplingSettings(f,n_chan,numberOfSamples,continous);
}

void QTScope::slotFaster() {
	char tmp[256];
	sprintf(tmp,"%d",((int)freq));
	double f;
	double fpot=pow(10,(double)(strlen(tmp)-1));
	cout << "fpot=" << fpot << "\n";
	if (*tmp=='1') {
		f=fpot*2.0;
	} else {
		if (*tmp=='2') {
			f=fpot*5.0;
		} else {
			f=fpot*10;
		}
	}
	slotSetSamplingSettings(f,n_chan,numberOfSamples,continous);
}


void QTScope::tellPlugins() {
	dataTarget *it=NULL;
	for ( it = activePlugins.first(); it; it = activePlugins.next() ) {
		if (cmd->convert_src == TRIG_NOW) {
			// no time between different samples
			it->target->slotSetSamplingRate(freq);
		} else {
			// time between different samples
			it->target->slotSetSamplingRate(freq/n_chan);
		}
	}
}
