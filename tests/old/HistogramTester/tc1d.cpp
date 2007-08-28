#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevCloud1D.h"
#include "AIDA/IHistogram1D.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testCloud1D( std::ostream& out )
{
  out << "Creating an 1D cloud" << std::endl;
  const int numberOfEntriesToTriggerConversion = 10000;
  const std::string title = "Title of Cloud 1D";
  const std::string name = "c1d";
  std::auto_ptr< AIDA::Dev::IDevCloud1D > c1da( m_hf.createCloud1D( title, numberOfEntriesToTriggerConversion ) );
  if ( ! c1da.get() ) throw std::runtime_error( "Could not create a Cloud1D" );
  c1da->setName( name + "a" );
  this->dumpCloudInfo1D( *c1da, out );

  out << "Filling it with random uniform data" << std::endl;
  int totalEntries = 1000;
  std::srand( 0 );
  double lowFillEdge = 0;
  double highFillEdge = 50;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c1da->fill( x, w );
  }
  this->dumpCloudInfo1D( *c1da, out );

  out << "Reseting it." << std::endl;
  if ( ! c1da->reset() ) throw std::runtime_error( "Could not reset the cloud" );
  this->dumpCloudInfo1D( *c1da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  totalEntries = 1000000;
  bool isConverted = false;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c1da->fill( x, w );
    if ( ! isConverted && c1da->isConverted() ) {
      out << "Cloud converted after " << i + 1 << " entries" << std::endl;
      isConverted = true;
    }
  }
  this->dumpCloudInfo1D( *c1da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c1da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo1D( *c1da, out );

  out << "Creating another one with no limits" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud1D > c1db( m_hf.createCloud1D( title ) );
  if ( ! c1db.get() ) throw std::runtime_error( "Could not create a Cloud1D" );
  c1db->setName( name + "b" );
  out << "Filling it" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    c1db->fill( x, w );
  }
  this->dumpCloudInfo1D( *c1db, out );

  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! c1db->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the cloud" );
  this->dumpCloudInfo1D( *c1db, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud1D > c1dc( m_hf.createCopy( *c1db ) );
  if ( ! c1dc.get() ) throw std::runtime_error( "Could not create a Cloud1D" );
  c1dc->setName( name + "c" );
  this->dumpCloudInfo1D( *c1dc, out );

  out << "Converting it on demand with specific boundaries" << std::endl;
  c1dc->convert( 10, lowFillEdge+1, highFillEdge-1 );
  this->dumpCloudInfo1D( *c1dc, out );

  out << "Creating a copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevCloud1D > c1dd( m_hf.createCopy( *c1dc ) );
  if ( ! c1dd.get() ) throw std::runtime_error( "Could not create a Cloud1D" );
  c1dd->setName( name + "d" );
  this->dumpCloudInfo1D( *c1dd, out );

}





void
HistogramUnitTester::dumpCloudInfo1D( const AIDA::ICloud1D& h, std::ostream& os ) const
{
  os << "************ CLOUD 1D **** START *******" << std::endl;
  this->dumpCloudInfo( h, os );
  os << "Mean : " << h.mean() << std::endl
     << "Rms : " << h.rms() << std::endl
     << "Lower edge : " << h.lowerEdge() << std::endl
     << "Upper edge : " << h.upperEdge() << std::endl
     << std::endl;

  if ( h.isConverted() ) {
    os << "Dumping the output of the supporting histogram.............." << std::endl;
    this->dumpHistogramInfo1D( h.histogram(), os );
  }
  else {
    for ( int i = 0; i < 100 && i < h.entries(); ++i ) {
      os << "Entry " << i << " : x = " << h.value( i ) << ",  w = " << h.weight( i ) << std::endl;
    }
    os << std::endl;
  }

  os << "************ CLOUD 1D ***** END ********" << std::endl;
}
