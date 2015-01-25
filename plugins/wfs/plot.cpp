/***************************************************************************
 *   Copyright (C) 2013 by Steffen Mauch                                   *
 *   steffen.mauch@gmail.com                                               *
 *                                                                         *
 *   based on spectrogram example from Qwt 6.x                             *
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
#include <qwt_color_map.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>
#include "plot.h"

class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer( QwtPlotCanvas *canvas ): QwtPlotZoomer( canvas )
    {
        setTrackerMode( AlwaysOn );
    }

    virtual QwtText trackerTextF( const QPointF &pos ) const
    {
        QColor bg( Qt::white );
        bg.setAlpha( 200 );

        QwtText text = QwtPlotZoomer::trackerTextF( pos );
        text.setBackgroundBrush( QBrush( bg ) );
        return text;
    }
};

class RasterData: public QwtMatrixRasterData
{
public:
	RasterData( ){


		QVector<double> values;

		for( uint i = 0; i < NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW; i++ ){
			values += 0;
		}
		
		numColumns = NB_OF_APERTURES_PER_ROW;
		setValueMatrix(values, numColumns);

		setInterval( Qt::XAxis,
		QwtInterval( 1, NB_OF_APERTURES_PER_ROW, QwtInterval::ExcludeMaximum ) );
		setInterval( Qt::YAxis,
		QwtInterval( 1, NB_OF_APERTURES_PER_ROW, QwtInterval::ExcludeMaximum ) );
		setInterval( Qt::ZAxis, QwtInterval(-40.0, 40.0) );
	}
	
	RasterData( double* data ){


		QVector<double> values;

		for( uint i = 0; i < NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW; i++ ){
			values += data[i];
		}
		
		numColumns = NB_OF_APERTURES_PER_ROW;
		setValueMatrix(values, numColumns);

		setInterval( Qt::XAxis,
		QwtInterval( 1, NB_OF_APERTURES_PER_ROW, QwtInterval::ExcludeMaximum ) );
		setInterval( Qt::YAxis,
		QwtInterval( 1, NB_OF_APERTURES_PER_ROW, QwtInterval::ExcludeMaximum ) );

		setInterval( Qt::ZAxis, QwtInterval(-40.0, 40.0) );

	}
	
private:
	int numColumns;
};

class ColorMap: public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap( Qt::darkCyan, Qt::red )
    {
        addColorStop( 0.1, Qt::blue );
        addColorStop( 0.4, Qt::green );
        addColorStop( 0.7, Qt::yellow );
        addColorStop( 1.0, Qt::red );
    }
};

Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setRenderThreadCount( 0 ); // use system specific thread count

    d_spectrogram->setColorMap( new ColorMap() );
    d_spectrogram->setCachePolicy( QwtPlotRasterItem::PaintCache );

    d_spectrogram->setData( new RasterData() );
    d_spectrogram->attach( this );

    QList<double> contourLevels;
    for ( double level = -40; level < 40.0; level += 1.0 )
        contourLevels += level;
    d_spectrogram->setContourLevels( contourLevels );

    const QwtInterval zInterval = d_spectrogram->data()->interval( Qt::ZAxis );
    // A color bar on the right axis
    QwtScaleWidget *rightAxis = axisWidget( QwtPlot::yRight );
    rightAxis->setTitle( "Intensity" );
    rightAxis->setColorBarEnabled( true );
    rightAxis->setColorMap( zInterval, new ColorMap() );

    setAxisScale( QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue() );
    enableAxis( QwtPlot::yRight );

    plotLayout()->setAlignCanvasToScales( true );
    replot();

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    QwtPlotZoomer* zoomer = new MyZoomer( (QwtPlotCanvas*)canvas() );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setAxisEnabled( QwtPlot::yRight, false );
    panner->setMouseButton( Qt::MidButton );

    // Avoid jumping when labels with more/less digits
    // appear/disappear when scrolling vertically

    const QFontMetrics fm( axisWidget( QwtPlot::yLeft )->font() );
    QwtScaleDraw *sd = axisScaleDraw( QwtPlot::yLeft );
    sd->setMinimumExtent( fm.width( "100.00" ) );

    const QColor c( Qt::darkBlue );
    zoomer->setRubberBandPen( c );
    zoomer->setTrackerPen( c );
    
    
    	
	ref_slopes_x = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
	ref_slopes_y = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
	
	for( int k=0; k<14; k++ ){
		for( int l=0; l<14; l++ ){
			(*ref_slopes_x)(k,l) = 8.0f+k*16;
            (*ref_slopes_y)(k,l) = 8.0f+l*16;
		}
	}
    
    actual_slopes_x = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
    actual_slopes_y = new MatrixXd(NB_OF_APERTURES_PER_ROW,NB_OF_APERTURES_PER_ROW);
    
	matC = new MatrixXd( 2*NB_OF_APERTURES_PER_ROW*(NB_OF_APERTURES_PER_ROW-1),
						2*NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
						
	matE = new MatrixXd( 2*NB_OF_APERTURES_PER_ROW*(NB_OF_APERTURES_PER_ROW-1), 
			NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
			
	result = new MatrixXd( NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW,1 );
	
	(*matC).setZero();
	(*matE).setZero();
	
	calcMatrixC( NB_OF_APERTURES_PER_ROW );
	calcMatrixE( NB_OF_APERTURES_PER_ROW );
}

Plot::~Plot( ){
	delete ref_slopes_x;
	delete ref_slopes_y;
    
    delete actual_slopes_x;
    delete actual_slopes_y;
    
    delete matE;
    delete matC;
    
    delete result;
}

void Plot::setData( double *data_x, double *data_y ){
	#if 1
	//qDebug() << "here i am";

	for( int k=0; k<NB_OF_APERTURES_PER_ROW; k++ ){
		for( int l=0; l<NB_OF_APERTURES_PER_ROW; l++ ){
			(*actual_slopes_x)(k,l) = *data_x;
			data_x++;
			(*actual_slopes_y)(k,l) = *data_y;
			data_y++;
		}
	}
	
	MatrixXd refX = ((*actual_slopes_x)-(*ref_slopes_x));
	MatrixXd refY = ((*actual_slopes_y)-(*ref_slopes_y));
	
	MatrixXd S( 1, 2*NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
	S << Map<MatrixXd>( ( refX.transpose()).data(),
						1, NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW),
		 Map<MatrixXd>( ( refY.transpose()).data(),
						1, NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW) ;
	
	
	
	JacobiSVD<MatrixXd> svd( (*matE), ComputeThinU | ComputeThinV);
	
	MatrixXd D = svd.singularValues().asDiagonal();
	MatrixXd pseudoInvD( NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW,
							NB_OF_APERTURES_PER_ROW*NB_OF_APERTURES_PER_ROW );
	pseudoInverse( D, pseudoInvD, 10e-9);
	
#if 0
	std::cout << "Eigen output:" << std::endl;
	
	//std::cout << "matrix S:" << S << std::endl;
	
	std::cout << S.size() << std::endl;
	
	std::cout << "C*S size: " << ((*matC)*S.transpose()).size() << std::endl;
	
	//std::cout << "C value: " << (*matC) << std::endl;
	
	//std::cout << "E value: " << (*matE) << std::endl;
	
	std::cout << "size singular value: " << (svd.singularValues()).asDiagonal().size() << std::endl;
	
	std::cout << "Pinv(D): " << pseudoInvD << "::end Pinv(D) " << endl;
	
	std::cout << "size V*D*U'*C*S: " << (svd.matrixV()* pseudoInvD *svd.matrixU().transpose()*(*matC)*S.transpose()).size() << std::endl;
#endif
	
	(*result) << (svd.matrixV() * pseudoInvD * svd.matrixU().transpose()*(*matC)*S.transpose());

#if 0
	std::cout << "values of wavefront reconstruction: " << (*result);
#endif
	
    d_spectrogram->setData( new RasterData( (*result).data() ) );
    replot();
	
	
	//
	#if 0
[n, n]=size(Sx);
S = [reshape(Sx', 1,n*n) reshape(Sy', 1,n*n)]';
E = getE(n);
[U, D, V] = svd(E, 0);
D = pinv(D);
C = getC(n); 
W=V*D*U'*C*S;
W=reshape(W', n, n)./ds;
	#endif

	#endif
	update();
}

void Plot::calcMatrixE( int n ){

	for( int i=1; i<=n; i++ ) {
		for( int j=1; j<=(n-1); j++ ){
			
			(*matE)((i-1)*(n-1)+j-1, (i-1)*n+j-1)=-1; 
			(*matE)((i-1)*(n-1)+j-1, (i-1)*n+j+1-1)=1; 
			(*matE)((n+i-1)*(n-1)+j-1, i+(j-1)*n-1)=-1;
			(*matE)((n+i-1)*(n-1)+j-1, i+j*n-1)=1;
		}
	}

#if DEBUG_RECONSTRUCTION
	std::cout << "matE - Plot";
	std::cout << (*matE) << std::endl;
#endif
}

void Plot::calcMatrixC( int n ){

	for( int i=1; i<=n; i++ ) {
		for( int j=1; j<=(n-1); j++ ){
			
			(*matC)((i-1)*(n-1)+j-1, (i-1)*n+j-1)=0.5;
			(*matC)((i-1)*(n-1)+j-1, (i-1)*n+j+1-1)=0.5; 
			(*matC)((n+i-1)*(n-1)+j-1, n*(n+j-1)+i-1)=0.5; 
			(*matC)((n+i-1)*(n-1)+j-1, n*(n+j)+i-1)=0.5; 
		}
	}
	
#if DEBUG_RECONSTRUCTION
	std::cout << "matC - Plot";
	std::cout << (*matC) << std::endl;
#endif
}

void Plot::showContour( bool on )
{
    d_spectrogram->setDisplayMode( QwtPlotSpectrogram::ContourMode, on );
    replot();
}

void Plot::showSpectrogram( bool on )
{
    d_spectrogram->setDisplayMode( QwtPlotSpectrogram::ImageMode, on );
    d_spectrogram->setDefaultContourPen( 
        on ? QPen( Qt::black, 0 ) : QPen( Qt::NoPen ) );

    replot();
}

void Plot::setAlpha( int alpha )
{
    d_spectrogram->setAlpha( alpha );
    replot();
}
