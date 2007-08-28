#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include "AIDA/IHistogram3D.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testCloud3D( std::ostream& out )
{
  out << "Creating an 3D cloud" << std::endl;
  const int numberOfEntriesToTriggerConversion = 10000;
  const std::string title = "Title of Cloud 3D";
  const std::string name = "c3d";
  std::auto_ptr< AIDA::Dev::IDevCloud3D > c3da( m_hf.createCloud3D( title, numberOfEntriesToTriggerConversion ) );
  if ( ! c3da.get() ) throw std::runtime_error( "Could not create a Cloud3D" );
  c3da->setName( name + "a" );
  this->dumpCloudInfo3D( *c3da, out );

  out << "Filling it with random uniform data" << std::endl;
  int totalEntries = 1000;
  std::srand( 0 );
  double lowFillEdge = 0;
  double highFillEdge = 50;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c3da->fill( x, y, z, w );
  }
  this->dumpCloudInfo3D( *c3da, out );

  out << "Reseting it." << std::endl;
  if ( ! c3da->reset() ) throw std::runtime_error( "Could not reset the cloud" );
  this->dumpCloudInfo3D( *c3da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  totalEntries = 1000000;
  bool isConverted = false;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c3da->fill( x, y, z, w );
    if ( ! isConverted && c3da->isConverted() ) {
      out << "Cloud converted after " << i + 1 << " entries" << std::endl;
      isConverted = true;
    }
  }
  this->dumpCloudInfo3D( *c3da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c3da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo3D( *c3da, out );

  out << "Creating another one with no limits" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud3D > c3db( m_hf.createCloud3D( title ) );
  if ( ! c3db.get() ) throw std::runtime_error( "Could not create a Cloud3D" );
  c3db->setName( name + "b" );
  out << "Filling it" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c3db->fill( x, y, z, w );
  }
  this->dumpCloudInfo3D( *c3db, out );

  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c3db->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo3D( *c3db, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud3D > c3dc( m_hf.createCopy( *c3db ) );
  if ( ! c3dc.get() ) throw std::runtime_error( "Could not create a Cloud3D" );
  c3dc->setName( name + "c" );
  this->dumpCloudInfo3D( *c3dc, out );

  out << "Converting it on demand with specific boundaries" << std::endl;
  c3dc->convert( 3, lowFillEdge+1, highFillEdge-1, 3, lowFillEdge+1, highFillEdge-1, 3, lowFillEdge+1, highFillEdge-1 );
  this->dumpCloudInfo3D( *c3dc, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud3D > c3dd( m_hf.createCopy( *c3dc ) );
  if ( ! c3dd.get() ) throw std::runtime_error( "Could not create a Cloud3D" );
  c3dd->setName( name + "d" );
  this->dumpCloudInfo3D( *c3dd, out );

}





void
HistogramUnitTester::dumpCloudInfo3D( const AIDA::ICloud3D& h, std::ostream& os ) const
{
  os << "************ CLOUD 3D **** START *******" << std::endl;
  this->dumpCloudInfo( h, os );
  os << "MeanX : " << h.meanX() << std::endl
     << "RmsX : " << h.rmsX() << std::endl
     << "Lower edge X : " << h.lowerEdgeX() << std::endl
     << "Upper edge X : " << h.upperEdgeX() << std::endl
     << "MeanY : " << h.meanY() << std::endl
     << "RmsY : " << h.rmsY() << std::endl
     << "Lower edge Y : " << h.lowerEdgeY() << std::endl
     << "Upper edge Y : " << h.upperEdgeY() << std::endl
     << "MeanZ : " << h.meanZ() << std::endl
     << "RmsZ : " << h.rmsZ() << std::endl
     << "Lower edge Z : " << h.lowerEdgeZ() << std::endl
     << "Upper edge Z : " << h.upperEdgeZ() << std::endl
     << std::endl;

  if ( h.isConverted() ) {
    os << "Dumping the output of the supporting histogram.............." << std::endl;
    this->dumpHistogramInfo3D( h.histogram(), os );
  }
  else {
    for ( int i = 0; i < 100 && i < h.entries(); ++i ) {
      os << "Entry " << i << " : x = " << h.valueX( i ) << " : y = " << h.valueY( i ) << " : z = " << h.valueZ( i ) << ",  w = " << h.weight( i ) << std::endl;
    }
    os << std::endl;
  }

  os << "************ CLOUD 3D ***** END ********" << std::endl;
}
