#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include "src/AIDA_Cloud1D.h"
#include "src/AIDA_Cloud2D.h"
#include "src/AIDA_Cloud3D.h"

void
HistogramUnitTester::testCloudToHistogram( std::ostream& out )
{
  out << "Creating an 3D cloud" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud3D > c3( m_hf.createCloud3D( "3d cloud" ) );
  if ( ! c3.get() ) throw std::runtime_error( "Could not create a Cloud3D" );
  c3->setName( "c3" );
  out << "Filling the cloud" << std::endl;
  int totalEntries = 10000;
  std::srand( 0 );
  double lowFillEdge = 0;
  double highFillEdge = 50;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c3->fill( x, y, z, w );
  }
  this->dumpCloudInfo3D( *c3, out );

  out << "Creating a 3-dimensional histogram with fixed boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3a( m_hf.createHistogram3D( c3->title(),
									   3, lowFillEdge + 1, highFillEdge - 1,
									   3, lowFillEdge + 1, highFillEdge - 1,
									   3, lowFillEdge + 1, highFillEdge - 1 ) );
  if ( ! h3a.get() ) throw std::runtime_error( "Could not create a Histogram3d" );
  h3a->setName( "h3a" );
  c3->fillHistogram( *h3a );
  this->dumpHistogramInfo3D( *h3a, out );

  out << "Creating an 2D cloud" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud2D > c2( m_hf.createCloud2D( "2d cloud" ) );
  if ( ! c2.get() ) throw std::runtime_error( "Could not create a Cloud2D" );
  c2->setName( "c2" );
  out << "Filling the cloud" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c2->fill( x, y, w );
  }
  this->dumpCloudInfo2D( *c2, out );

  out << "Creating a 2-dimensional histogram with fixed boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2a( m_hf.createHistogram2D( c2->title(),
									   5, lowFillEdge + 1, highFillEdge - 1,
									   5, lowFillEdge + 1, highFillEdge - 1 ) );
  if ( ! h2a.get() ) throw std::runtime_error( "Could not create a Histogram2d" );
  h2a->setName( "h2a" );
  c2->fillHistogram( *h2a );
  this->dumpHistogramInfo2D( *h2a, out );

  out << "Creating an 1D cloud" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud1D > c1( m_hf.createCloud1D( "1d cloud" ) );
  if ( ! c1.get() ) throw std::runtime_error( "Could not create a Cloud1D" );
  c1->setName( "c1" );
  out << "Filling the cloud" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c1->fill( x, w );
  }
  this->dumpCloudInfo1D( *c1, out );

  out << "Creating a 1-dimensional histogram with fixed boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1a( m_hf.createHistogram1D( c1->title(),
									   10, lowFillEdge + 1, highFillEdge - 1 ) );
  if ( ! h1a.get() ) throw std::runtime_error( "Could not create a Histogram1d" );
  h1a->setName( "h1a" );
  c1->fillHistogram( *h1a );
  this->dumpHistogramInfo1D( *h1a, out );

}
