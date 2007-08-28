#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevProfile2D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testProfile2D( std::ostream& out )
{
  out << "Creating an 2D fixed-binned profile" << std::endl;
  const int numberOfBins = 10;
  const double lowEdge = 0;
  const double highEdge = 5;
  const std::string title = "Title of Profile 2D";
  const std::string name = "p2d";
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2da( m_hf.createProfile2D( title, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge ) );
  if ( ! p2da.get() ) throw std::runtime_error( "Could not create a Profile2D" );
  p2da->setName( name + "a" );
  this->dumpProfileInfo2D( *p2da, out );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = p2da->xAxis().lowerEdge() - 1;
  const double highFillEdge = p2da->xAxis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p2da->fill( x, y, z, w );
  }
  this->dumpProfileInfo2D( *p2da, out );

  out << "Reseting it." << std::endl;
  if ( ! p2da->reset() ) throw std::runtime_error( "Could not reset the profile" );
  this->dumpProfileInfo2D( *p2da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p2da->fill( x, y, z, w );
  }
  this->dumpProfileInfo2D( *p2da, out );

  out << "Creating a new variable sized binned profile compatile with the first one." << std::endl;
  std::vector< double > edgesCompatible( numberOfBins + 1 );
  for ( int i = 0; i < numberOfBins; ++i ) edgesCompatible[i] = p2da->xAxis().binLowerEdge(i);
  edgesCompatible[numberOfBins] = p2da->xAxis().upperEdge();
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2db( m_hf.createProfile2D( title + "(2)", edgesCompatible, edgesCompatible, "" ) );
  if ( ! p2db.get() ) throw std::runtime_error( "Could not create a Profile2D" );
  p2db->setName( name + "b" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p2db->fill( x, y, z, w );
  }
  this->dumpProfileInfo2D( *p2db, out );

  out << "Incrementing the first profile with the second." << std::endl;
  if ( ! p2da->add( *p2db ) ) throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo2D( *p2da, out );

  out << "Creating a non-compatible profile" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2dc( m_hf.createProfile2D( title, numberOfBins / 2, lowEdge, highEdge, numberOfBins / 2, lowEdge, highEdge ) );
  if ( ! p2dc.get() ) throw std::runtime_error( "Could not create a Profile2D" );
  p2dc->setName( name + "c" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p2dc->fill( x, y, z, w );
  }
  this->dumpProfileInfo2D( *p2dc, out );

  out << "Incrementing the third profile with the second." << std::endl;
  if ( p2dc->add( *p2db ) ) throw std::runtime_error( "Incremented two incompatible profiles!!!" );
  this->dumpProfileInfo2D( *p2dc, out );

  out << "Creating a copy of the third profile." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2dd( m_hf.createCopy( *p2dc ) );
  if ( ! p2dd.get() ) throw std::runtime_error( "Could not create a Profile2D" );
  p2dd->setName( name + "d" );
  this->dumpProfileInfo2D( *p2dd, out );

  out << "Creating a profile with boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2de( m_hf.createProfile2D( title, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge, lowEdge + 1,  highEdge -1 ) );
  if ( ! p2de.get() ) throw std::runtime_error( "Could not create a Profile2D" );
  p2de->setName( name + "e" );
  out << "Filling the profile" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p2de->fill( x, y, z, w );
  }
  this->dumpProfileInfo2D( *p2de, out );

  out << "Create a copy of the profile with boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile2D > p2df( m_hf.createCopy( *p2de ) );
  if ( ! p2df.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p2df->setName( name + "f" );
  this->dumpProfileInfo2D( *p2df, out );

  out << "Adding the last two profiles" << std::endl;
  if ( ! p2df->add( *p2de ) ) throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo2D( *p2df, out );

  out << "Adding the last profile with the unbounded profile" << std::endl;
  if ( ! p2df->add( *p2db ) ) throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo2D( *p2df, out );

  out << "Adding the last profile with an incompatible unbounded profile" << std::endl;
  if ( p2df->add( *p2dc ) ) throw std::runtime_error( "Increment two incompatible profiles" );
  this->dumpProfileInfo2D( *p2df, out );
}





void
HistogramUnitTester::dumpProfileInfo2D( const AIDA::IProfile2D& h, std::ostream& os ) const
{
  os << "************ PROFILE 2D **** START *******" << std::endl;
  this->dumpProfileInfo( h, os );
  os << "MeanX : " << h.meanX() << std::endl
     << "RmsX : " << h.rmsX() << std::endl
     << "MeanY : " << h.meanY() << std::endl
     << "RmsY : " << h.rmsY() << std::endl
     << "Extra entries : " << h.extraEntries() << std::endl
     << std::endl;

  // Dump the profile contents...
  os << "Bin contents: bin number x / bin number y / bin low edge x / bin centre x / bin high edge x / bin low edge y / bin centre y / bin high edge y / bin height / bin entries / bin error / bin rms" << std::endl;
  const AIDA::IAxis& xAxis = h.xAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    const AIDA::IAxis& yAxis = h.yAxis();
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      os << i << "\t" << j << "\t"
	 << xAxis.binLowerEdge( i ) << "\t" << h.binMeanX( i,j ) << "\t" << xAxis.binUpperEdge( i ) << "\t"
	 << yAxis.binLowerEdge( j ) << "\t" << h.binMeanY( i,j ) << "\t" << yAxis.binUpperEdge( j ) << "\t"
	 << h.binHeight( i,j ) << "\t" << h.binEntries( i,j ) << "\t" << h.binError( i,j ) << h.binRms( i,j ) << std::endl;
    }
  }
  
  os << "************ PROFILE 2D ***** END ********" << std::endl;
}
