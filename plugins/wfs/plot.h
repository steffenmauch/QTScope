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
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot.h>

#include <iostream>
#include <Eigen/SVD>
#include <Eigen/Core>
using namespace Eigen;

#define WFS_SIZE 224
#define NB_OF_APERTURES_PER_ROW 14

#define DEBUG_RECONSTRUCTION 0

class RasterData;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );
	~Plot();
	
	void setData( double *data_x, double *data_y );

public Q_SLOTS:
    void showContour( bool on );
    void showSpectrogram( bool on );
    void setAlpha( int );

private:
    QwtPlotSpectrogram *d_spectrogram;
    
    MatrixXd *actual_slopes_x;
    MatrixXd *actual_slopes_y;
    
	MatrixXd *ref_slopes_x;
	MatrixXd *ref_slopes_y;
	
	MatrixXd *matC;
	MatrixXd *matE;
	
	MatrixXd *result;
	
	void calcMatrixC( int n );
	void calcMatrixE( int n );
};
