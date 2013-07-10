#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include "AIDA/IHistogram2D.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testCloud2D( std::ostream& out )
{
  out << "Creating an 2D cloud" << std::endl;
  const int numberOfEntriesToTriggerConversion = 10000;
  const std::string title = "Title of Cloud 2D";
  const std::string name = "c2d";
  std::auto_ptr< AIDA::Dev::IDevCloud2D > c2da( m_hf.createCloud2D( title, numberOfEntriesToTriggerConversion ) );
  if ( ! c2da.get() ) throw std::runtime_error( "Could not create a Cloud2D" );
  c2da->setName( name + "a" );
  this->dumpCloudInfo2D( *c2da, out );

  out << "Filling it with random uniform data" << std::endl;
  int totalEntries = 1000;
  std::srand( 0 );
  double lowFillEdge = 0;
  double highFillEdge = 50;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c2da->fill( x, y, w );
  }
  this->dumpCloudInfo2D( *c2da, out );

  out << "Reseting it." << std::endl;
  if ( ! c2da->reset() ) throw std::runtime_error( "Could not reset the cloud" );
  this->dumpCloudInfo2D( *c2da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  totalEntries = 1000000;
  bool isConverted = false;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c2da->fill( x, y, w );
    if ( ! isConverted && c2da->isConverted() ) {
      out << "Cloud converted after " << i + 1 << " entries" << std::endl;
      isConverted = true;
    }
  }
  this->dumpCloudInfo2D( *c2da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c2da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo2D( *c2da, out );

  out << "Creating another one with no limits" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud2D > c2db( m_hf.createCloud2D( title ) );
  if ( ! c2db.get() ) throw std::runtime_error( "Could not create a Cloud2D" );
  c2db->setName( name + "b" );
  out << "Filling it" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c2db->fill( x, y, w );
  }
  this->dumpCloudInfo2D( *c2db, out );

  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c2db->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo2D( *c2db, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud2D > c2dc( m_hf.createCopy( *c2db ) );
  if ( ! c2dc.get() ) throw std::runtime_error( "Could not create a Cloud2D" );
  c2dc->setName( name + "c" );
  this->dumpCloudInfo2D( *c2dc, out );

  out << "Converting it on demand with specific boundaries" << std::endl;
  c2dc->convert( 5, lowFillEdge+1, highFillEdge-1, 5, lowFillEdge+1, highFillEdge-1 );
  this->dumpCloudInfo2D( *c2dc, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud2D > c2dd( m_hf.createCopy( *c2dc ) );
  if ( ! c2dd.get() ) throw std::runtime_error( "Could not create a Cloud2D" );
  c2dd->setName( name + "d" );
  this->dumpCloudInfo2D( *c2dd, out );

}





void
HistogramUnitTester::dumpCloudInfo2D( const AIDA::ICloud2D& h, std::ostream& os ) const
{
  os << "************ CLOUD 2D **** START *******" << std::endl;
  this->dumpCloudInfo( h, os );
  os << "MeanX : " << h.meanX() << std::endl
     << "RmsX : " << h.rmsX() << std::endl
     << "Lower edge X : " << h.lowerEdgeX() << std::endl
     << "Upper edge X : " << h.upperEdgeX() << std::endl
     << "MeanY : " << h.meanY() << std::endl
     << "RmsY : " << h.rmsY() << std::endl
     << "Lower edge Y : " << h.lowerEdgeY() << std::endl
     << "Upper edge Y : " << h.upperEdgeY() << std::endl
     << std::endl;

  if ( h.isConverted() ) {
    os << "Dumping the output of the supporting histogram.............." << std::endl;
    this->dumpHistogramInfo2D( h.histogram(), os );
  }
  else {
    for ( int i = 0; i < 100 && i < h.entries(); ++i ) {
      os << "Entry " << i << " : x = " << h.valueX( i ) << " : y = " << h.valueY( i ) << ",  w = " << h.weight( i ) << std::endl;
    }
    os << std::endl;
  }

  os << "************ CLOUD 2D ***** END ********" << std::endl;
}
