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
HistogramUnitTester::testSlices( std::ostream& out )
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

  out << "Creating a XY slice of thickness 1 layer" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2xy( m_hf.sliceXY( *h3d, 0, 0 ) );
  h2xy->setName( "h2xy" );
  h2xy->setTitle( "XY slice" );
  this->dumpHistogramInfo2D( *h2xy, out );

  out << "Creating a XZ slice of thickness 2 layers" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2xz( m_hf.sliceXZ( *h3d, 0, 1 ) );
  h2xz->setName( "h2xz" );
  h2xz->setTitle( "XZ slice" );
  this->dumpHistogramInfo2D( *h2xz, out );

  out << "Creating a YZ slice of thickness 3 layers" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2yz( m_hf.sliceYZ( *h3d, 0, 2 ) );
  h2yz->setName( "h2yz" );
  h2yz->setTitle( "YZ slice" );
  this->dumpHistogramInfo2D( *h2yz, out );

  out << "Creating a X slice in the middle from the XY slice" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1x( m_hf.sliceX( *h2xy, 1 ) );
  h1x->setName( "h1x" );
  h1x->setTitle( "X slice" );
  this->dumpHistogramInfo1D( *h1x, out );

  out << "Creating a Y slice in the middle from the XY slice" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1y( m_hf.sliceY( *h2xy, 1 ) );
  h1y->setName( "h1y" );
  h1y->setTitle( "Y slice" );
  this->dumpHistogramInfo1D( *h1y, out );

  out << "Creating a X slice from the first two layers from the XY slice" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1xa( m_hf.sliceX( *h2xy, 0, 1 ) );
  h1xa->setName( "h1xa" );
  h1xa->setTitle( "X slice (a)" );
  this->dumpHistogramInfo1D( *h1xa, out );

  out << "Creating a Y slice from the first two layers from the XY slice" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1ya( m_hf.sliceY( *h2xy, 0, 1 ) );
  h1ya->setName( "h1ya" );
  h1ya->setTitle( "Y slice (a)" );
  this->dumpHistogramInfo1D( *h1ya, out );
}
