/***************************************************************************
 *   Copyright (C) 2013 by Steffen Mauch                                   *
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

#include "gradientPlot.h"
#include <QPainter>
#include <QDebug>
#include <QResizeEvent>

#include <iostream>

gradientPlot::gradientPlot( QWidget* parent )
	: QWidget( parent )
{
	resize(300,300);
	
	ref_slopes_x = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
	ref_slopes_y = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
    
    actual_slopes_x = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
    actual_slopes_y = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
    
    matC = new MatrixXd( 2*NB_OF_APERTURES_PER_ROW*(NB_OF_APERTURES_PER_ROW-1),
						2*NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
						
	matE = new MatrixXd( 2*NB_OF_APERTURES_PER_ROW*(NB_OF_APERTURES_PER_ROW-1), 
			NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
	
	(*matC).setZero();
	(*matE).setZero();
	
	calcMatrixC( NB_OF_APERTURES_PER_ROW );
	calcMatrixE( NB_OF_APERTURES_PER_ROW );
    
    for( int k=0; k<14; k++ ){
		for( int l=0; l<14; l++ ){
			(*ref_slopes_x)(k,l) = 8.0f+k*16;
            (*ref_slopes_y)(k,l) = 8.0f+l*16;
		}
	}
	
	for( int k=0; k<14; k++ ){
		for( int l=0; l<14; l++ ){
			(*actual_slopes_x)(k,l) = 8.0f+k*16+3;
            (*actual_slopes_y)(k,l) = 8.0f+l*16;
		}
	}
    
    (*actual_slopes_x)(1,1) = -1;
}


	
void gradientPlot::calcMatrixE( int n ){

	for( int i=1; i<=n; i++ ) {
		for( int j=1; j<=(n-1); j++ ){
			
			(*matE)((i-1)*(n-1)+j-1, (i-1)*n+j-1)=-1; 
			(*matE)((i-1)*(n-1)+j-1, (i-1)*n+j+1-1)=1; 
			(*matE)((n+i-1)*(n-1)+j-1, i+(j-1)*n-1)=-1;
			(*matE)((n+i-1)*(n-1)+j-1, i+j*n-1)=1;
		}
	}

#if DEBUG_RECONSTRUCTION
	std::cout << "haha";
	std::cout << (*matC) << std::endl;
#endif
}

void gradientPlot::calcMatrixC( int n ){

	for( int i=1; i<=n; i++ ) {
		for( int j=1; j<(n-1); j++ ){
			
			(*matC)((i-1)*(n-1)+j-1, (i-1)*n+j-1)=0.5;
			(*matC)((i-1)*(n-1)+j-1, (i-1)*n+j+1-1)=0.5; 
			(*matC)((n+i-1)*(n-1)+j-1, n*(n+j-1)+i-1)=0.5; 
			(*matC)((n+i-1)*(n-1)+j-1, n*(n+j)+i-1)=0.5; 
		}
	}
	
#if DEBUG_RECONSTRUCTION
	std::cout << "haha";
	std::cout << (*matC) << std::endl;
#endif
}

void gradientPlot::resizeEvent(QResizeEvent * event){
	QSize ssize = event->size();
	int maxima = qRound( qMax(ssize.width(),ssize.height()) );
	resize( maxima, maxima );
}



void gradientPlot::paintEvent(QPaintEvent *){
	//qDebug() << "paintEvent" << endl;

	QRect rect(0, 20, 0, 60);  
	double scale = 1.0;
	QPainterPath path;
	QPainter painter(this);
	QPen pen;  // creates a default pen
     
	painter.setWindow(	qRound(-10)*scale, 			// xmin
						qRound(-10)*scale,			// ymin
						qRound(WFS_SIZE+20)*scale,	// xmax
                        qRound(WFS_SIZE+20)*scale	// ymax
                        );
	painter.scale(1,1);
	
    painter.fillRect(QRect(0, 0, WFS_SIZE - 1, WFS_SIZE - 1), Qt::white );

	pen.setStyle(Qt::SolidLine);
	pen.setWidth(3);
	pen.setBrush(Qt::green);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	painter.setPen(pen);
 
	painter.setRenderHint(QPainter::Antialiasing, true);
	

	painter.save();
	pen.setStyle(Qt::DashLine);
	pen.setWidth(1);
	pen.setBrush(Qt::gray);
	painter.setPen(pen);
	
	for( int k=0; k<14; k++ ){
		for( int l=0; l<14; l++ ){
			if( (*actual_slopes_x)(k,l) == -1 || (*actual_slopes_y)(k,l) == -1 ){
				;
			}
			else{
				painter.drawLine( QPointF( (*ref_slopes_x)(k,l), (*ref_slopes_y)(l,l) ), 
					QPointF( (*actual_slopes_x)(k,l), (*actual_slopes_y)(l,l) ) );
				}
			}
	}
	painter.restore();


	painter.save();
	pen.setWidth(1);
	pen.setBrush(Qt::red);
	painter.setPen(pen);
	
	for( int k=0; k<14; k++ ){
		for( int l=0; l<14; l++ )
			painter.drawPoint( QPointF( (*ref_slopes_x)(k,l), (*ref_slopes_y)(l,l) ) );
	}
	painter.restore();

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(palette().dark().color());
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(QRect(0, 0, WFS_SIZE - 1, WFS_SIZE - 1));
}

void gradientPlot::setData( double *data_x, double *data_y ){
    #if 0
	for( int k=0; k<NB_OF_APERTURES_PER_ROW; k++ ){
		for( int l=0; l<NB_OF_APERTURES_PER_ROW; l++ ){
			(*actual_slopes_x)(k,l) = *data_x;
			data_x++;
			(*actual_slopes_y)(k,l) = *data_y;
			data_y++;
		}
	}
	#endif
	update();
}

gradientPlot::~gradientPlot()
{
	delete ref_slopes_x;
	delete ref_slopes_y;
    
    delete actual_slopes_x;
    delete actual_slopes_y;
}


