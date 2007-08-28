#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testProjections( std::ostream& out )
{
  // Create a three-dimensional histogram
  int nBins = 3;
  double lowEdge = 0;
  double highEdge = 90;
  out << "Creating a 3-dimensional histogram" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3d( m_hf.createHistogram3D( "example 3d histogram",
									   nBins, lowEdge, highEdge,
									   nBins, lowEdge, highEdge,
									   nBins, lowEdge, highEdge ) );
  if ( ! h3d.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3d->setName( "h3d" );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = h3d->xAxis().lowerEdge() - 1;
  const double highFillEdge = h3d->xAxis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h3d->fill( x, y, z, w );
  }
  this->dumpHistogramInfo3D( *h3d, out );

  out << "Creating the XY projection" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2xy( m_hf.projectionXY( *h3d ) );
  h2xy->setName( "h2xy" );
  h2xy->setTitle( "XY projection" );
  this->dumpHistogramInfo2D( *h2xy, out );

  out << "Creating the XZ projection" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2xz( m_hf.projectionXZ( *h3d ) );
  h2xz->setName( "h2xz" );
  h2xz->setTitle( "XZ projection" );
  this->dumpHistogramInfo2D( *h2xz, out );

  out << "Creating the YZ projection" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2yz( m_hf.projectionYZ( *h3d ) );
  h2yz->setName( "h2yz" );
  h2yz->setTitle( "YZ projection" );
  this->dumpHistogramInfo2D( *h2yz, out );

  out << "Creating the X projection from XY" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1x( m_hf.projectionX( *h2xy ) );
  h1x->setName( "h1x" );
  h1x->setTitle( "X projection" );
  this->dumpHistogramInfo1D( *h1x, out );

  out << "Creating the Y projection from XY" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1y( m_hf.projectionY( *h2xy ) );
  h1y->setName( "h1y" );
  h1y->setTitle( "Y projection" );
  this->dumpHistogramInfo1D( *h1y, out );

}
