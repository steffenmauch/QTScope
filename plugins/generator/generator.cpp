/***************************************************************************
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

#include "generator.h"
#include <QDebug>
#include <iostream>




char *cmd_src(int src,char *buf)
{
    buf[0]=0;

    if(src&TRIG_NONE)strcat(buf,"none|");
    if(src&TRIG_NOW)strcat(buf,"now|");
    if(src&TRIG_FOLLOW)strcat(buf, "follow|");
    if(src&TRIG_TIME)strcat(buf, "time|");
    if(src&TRIG_TIMER)strcat(buf, "timer|");
    if(src&TRIG_COUNT)strcat(buf, "count|");
    if(src&TRIG_EXT)strcat(buf, "ext|");
    if(src&TRIG_INT)strcat(buf, "int|");
#ifdef TRIG_OTHER
    if(src&TRIG_OTHER)strcat(buf, "other|");
#endif

    if(strlen(buf)==0){
        sprintf(buf,"unknown(0x%08x)",src);
    }else{
        buf[strlen(buf)-1]=0;
    }

    return buf;
}

void dump_cmd(FILE *out,comedi_cmd *cmd)
{
    char buf[100];

    fprintf(out,"subdev:      %d\n", cmd->subdev);
    fprintf(out,"flags:       %x\n", cmd->flags);

    fprintf(out,"start:      %-8s %d\n",
        cmd_src(cmd->start_src,buf),
        cmd->start_arg);

    fprintf(out,"scan_begin: %-8s %d\n",
        cmd_src(cmd->scan_begin_src,buf),
        cmd->scan_begin_arg);

    fprintf(out,"convert:    %-8s %d\n",
        cmd_src(cmd->convert_src,buf),
        cmd->convert_arg);

    fprintf(out,"scan_end:   %-8s %d\n",
        cmd_src(cmd->scan_end_src,buf),
        cmd->scan_end_arg);

    fprintf(out,"stop:       %-8s %d\n",
        cmd_src(cmd->stop_src,buf),
        cmd->stop_arg);

    fprintf(out,"chanlist_len: %d\n", cmd->chanlist_len);
    fprintf(out,"data_len: %d\n", cmd->data_len);

}






pluginInfo myPluginInfo = {"Generator Plugin", "Plot", 1, COMEDI_SUBD_AO};

scopePlotPlugin *createPlugin (QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags,int numberOfSamples) {
	return new generatorPlugin(caller, parent, name, id, wflags,numberOfSamples);
}

using std::cout;

generatorPlugin::generatorPlugin(QTScope* caller, QWidget* parent, const char* name, int id, Qt::WindowFlags wflags, int numberOfSamples)
	: scopePlotPlugin(caller, parent, name, id, wflags, numberOfSamples)
{
	cout << "generator::generator: Plugin generated\n";
	callingWidget = caller;
	idThis = id;
			
	list = new QVector<QCheckBox*>();
	
	window2 = new QWidget();
	hbox_layout = new QHBoxLayout();
	vbox_layout = new QVBoxLayout();

	frequency = new QLineEdit();
	frequency->setText("5");
	amplitude = new QLineEdit();
	amplitude->setText("2");
	offset = new QLineEdit();
	offset->setText("0");
	
	hbox_layout->addWidget( new QLabel( tr("Freq.[Hz]") ) );
	hbox_layout->addWidget( frequency );
	hbox_layout->addWidget( new QLabel( tr("Amp.[V]") ) );
	hbox_layout->addWidget( amplitude );
	hbox_layout->addWidget( new QLabel( tr("Off.[V]") ) );
	hbox_layout->addWidget( offset );
	
	window2->setLayout( hbox_layout );
	
	dropdown = new QComboBox();
	
	dropdown->addItem( QString( tr("Sinus") ), QVariant::fromValue(0));
	dropdown->addItem( QString( tr("Triangle") ), QVariant::fromValue(1));
	dropdown->addItem( QString( tr("Rectangle") ), QVariant::fromValue(2) );
	
	vbox_layout->addWidget( dropdown );
	vbox_layout->addWidget( window2 );
	
	vbox_layout->setAlignment ( Qt::AlignHCenter );
 
    for ( int i = 0; i < NB_SAMPLES; i++ ) {
        x[i] =  ((double)i) / NB_SAMPLES;
        y[i] = 0;
    }
 
    plot = new QwtPlot();
    curve = new QwtPlotCurve();
    curve->setRawSamples(x, y, 101);
    curve->attach( plot );

    plot->setAxisAutoScale(0, true);
    plot->setAxisAutoScale(2, true);
    
    plot->setAxisTitle(0,tr("in [V]"));
    plot->setAxisTitle(2,tr("in [s]"));
	
	vbox_layout->addWidget( plot );

	
	save = new QPushButton( tr("Set") );
	vbox_layout->addWidget(save);
	vbox_layout->connect(save, SIGNAL( clicked() ),
		this, SLOT( saveCheckBoxes() ) );
	
    setLayout(vbox_layout);
	
	setWindowTitle("analog output");
	
	show();
}


void generatorPlugin::saveCheckBoxes(){
	
	double freq = frequency->text().toDouble();
	double off = offset->text().toDouble();
	double amp = amplitude->text().toDouble();
	
	int index =  dropdown->itemData( dropdown->currentIndex() ).toInt();
	
	for ( int i = 0; i < NB_SAMPLES; i++ ) {
        x[i] = 2/(freq) * ((double)i) / ((double)NB_SAMPLES);
        
        if( index == 2 ){
			if( i%(NB_SAMPLES/2) <= NB_SAMPLES/4 )
				y[i] = -amp/2 + off;
			else
				y[i] = amp/2 + off;
		}
		else if( index == 1 ){
			y[i] = amp/2.0 - amp* (i%(NB_SAMPLES/2) / ((double)NB_SAMPLES/2.0)) + off;
		}
		else
			y[i] = amp*sin(2*3.1415*freq*x[i]) + off;
			
    }
    curve->setRawSamples(x, y, NB_SAMPLES);

	plot->replot();	
}


/* This is the size of chunks we deal with when creating and
   outputting data.  This *could* be 1, but that would be
   inefficient */
#define BUF_LEN 4096

void generatorPlugin::getComedi(comedi_t *comediDevice1){
    qDebug() << "called function: " << __func__ << endl;
    comediDevice = comediDevice1;

    comediSubdevice = comedi_find_subdevice_by_type(comediDevice,COMEDI_SUBD_AO,0);
    if( comediDevice < 0 )
        comedi_perror( QString("error in %1 line %2").arg(__func__).arg(__LINE__).toStdString().c_str() );

    cmd = new comedi_cmd;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotWriteData()));
    timer->start(500);

    chanlist[0] = CR_PACK(0,8192,AREF_GROUND );

    memset(cmd,0,sizeof(cmd));

    /* fill in the command data structure: */
    cmd->subdev         = comediSubdevice;
    cmd->flags          = 0;
    cmd->start_src      = TRIG_INT;
    cmd->start_arg      = 0;
    cmd->scan_begin_src = TRIG_TIMER;
    cmd->scan_begin_arg = 1e9/freq;
    cmd->convert_src    = TRIG_NOW;
    cmd->convert_arg    = 0;
    cmd->scan_end_src   = TRIG_COUNT;
    cmd->scan_end_arg   = 1; // why 1 !?!?
    cmd->stop_src       = TRIG_NONE;
    cmd->stop_arg       = 0;
    cmd->data_len       = 1;
    cmd->chanlist       = chanlist;
    cmd->chanlist_len   = 1; // same as above

    dump_cmd(stdout,cmd);


    lsampl_t data[BUF_LEN];
    for(int k =0;k<BUF_LEN;k++)
        data[k] = k;

    int ret;
    comedi_range range;
    range.max = 10;
    range.min = 0;
    range.unit = UNIT_none;

    printf("writing %d to device=%s subdevice=%d channel=%d range=%d analog reference=%d\n",
           data[0], "comediDevice", comediSubdevice, 4, 4096, AREF_GROUND);

    ret = comedi_data_write(comediDevice, comediSubdevice, 5, 4096, AREF_GROUND, data[0]);
    if(ret < 0){
        comedi_perror( QString("error in %1 line %2").arg(__func__).arg(__LINE__).toStdString().c_str() );
        timer->stop();
    }
}

void generatorPlugin::slotWriteData(){

    lsampl_t data = 100;

    qDebug() << "called function: " << __func__ << endl;
    int ret;
    ret = comedi_data_write(comediDevice, comediSubdevice, 5, 4096, AREF_GROUND, data);
    if(ret < 0){
        comedi_perror( QString("error in %1 line %2").arg(__func__).arg(__LINE__).toStdString().c_str() );
        timer->stop();
    }

 #if 0
    int count = 196;
    lsampl_t data[count];
    double data_x[count];
    double data_y[count];

    int retval = 0;
    retval = comedi_data_read_n(comediDevice, comediSubdevice, 0, 0, AREF_GROUND, &data[0], 100);

    if(retval < 0)	{
        comedi_perror("comedi_data_read");
        return;
    }

    retval = comedi_data_read_n(comediDevice, comediSubdevice, 100, 0, AREF_GROUND, &data[100], 96);

    if(retval < 0)	{
        comedi_perror("comedi_data_read");
        return;
    }

    for( int k = 0; k < count; k++ ){
        //printf("%d\n", data[k]);
        data_x[k] = double(data[k]);
    }

    // enable this for a fake tip/tilt wavefront
    #if 1
    for( int k=0; k<14; k++ ){
        for( int l=0; l<14; l++ ){
            data_x[k*14+l] = 8.0f+k*16+4;
            data_y[k*14+l] = 8.0f+l*16+0;
        }
    }
    #endif
    if ( en_contour->isChecked() )
        d_plot->setData( &data_x[0], &data_y[0] );
    else
        gradient_plot->setData( &data_x[0], &data_y[0] );
#endif
}


QSize generatorPlugin::sizeHint() const
{
	return QSize(200, 400);
}

generatorPlugin::~generatorPlugin()
{
}


