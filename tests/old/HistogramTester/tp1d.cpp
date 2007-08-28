#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testProfile1D( std::ostream& out )
{
  out << "Creating an 1D fixed-binned profile histogram" << std::endl;
  const int numberOfBins = 100;
  const double lowEdge = 0;
  const double highEdge = 50;
  const std::string title = "Title of Profile 1D";
  const std::string name = "p1d";
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1da( m_hf.createProfile1D( title, numberOfBins, lowEdge, highEdge ) );
  if ( ! p1da.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1da->setName( name + "a" );
  this->dumpProfileInfo1D( *p1da, out );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = p1da->axis().lowerEdge() - 1;
  const double highFillEdge = p1da->axis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p1da->fill( x, y, w );
  }
  this->dumpProfileInfo1D( *p1da, out );

  out << "Reseting it." << std::endl;
  if ( ! p1da->reset() ) throw std::runtime_error( "Could not reset the profile" );
  this->dumpProfileInfo1D( *p1da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p1da->fill( x, y, w );
  }
  this->dumpProfileInfo1D( *p1da, out );

  out << "Creating a new variable sized binned profile compatile with the first one." << std::endl;
  std::vector< double > edgesCompatible( numberOfBins + 1 );
  for ( int i = 0; i < numberOfBins; ++i ) edgesCompatible[i] = p1da->axis().binLowerEdge(i);
  edgesCompatible[numberOfBins] = p1da->axis().upperEdge();
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1db( m_hf.createProfile1D( title + "(2)", edgesCompatible, "" ) );
  if ( ! p1db.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1db->setName( name + "b" );
  out << "Filling it with random uniform data" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p1db->fill( x, y, w );
  }
  this->dumpProfileInfo1D( *p1db, out );

  out << "Incrementing the first profile with the second." << std::endl;
  if ( ! p1da->add( *p1db ) )  throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo1D( *p1da, out );

  out << "Creating a non-compatible profile" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1dc( m_hf.createProfile1D( title, numberOfBins / 2, lowEdge, highEdge ) );
  if ( ! p1dc.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1dc->setName( name + "c" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p1dc->fill( x, y, w );
  }
  this->dumpProfileInfo1D( *p1dc, out );

  out << "Incrementing the third profile with the second." << std::endl;
  if ( p1dc->add( *p1db ) ) throw std::runtime_error( "Incremented two incompatible profiles!!!" );
  this->dumpProfileInfo1D( *p1dc, out );

  out << "Creating a copy of the third profile." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1dd( m_hf.createCopy( *p1dc ) );
  if ( ! p1dd.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1dd->setName( name + "d" );
  this->dumpProfileInfo1D( *p1dd, out );

  out << "Creating a profile with boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1de( m_hf.createProfile1D( title, numberOfBins, lowEdge, highEdge, lowEdge + 1,  highEdge -1 ) );
  if ( ! p1de.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1de->setName( name + "e" );
  out << "Filling the profile" << std::endl;
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    p1de->fill( x, y, w );
  }
  this->dumpProfileInfo1D( *p1de, out );

  out << "Create a copy of the profile with boundaries" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevProfile1D > p1df( m_hf.createCopy( *p1de ) );
  if ( ! p1df.get() ) throw std::runtime_error( "Could not create a Profile1D" );
  p1df->setName( name + "f" );
  this->dumpProfileInfo1D( *p1df, out );

  out << "Adding the last two profiles" << std::endl;
  if ( ! p1df->add( *p1de ) ) throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo1D( *p1df, out );

  out << "Adding the last profile with the unbounded profile" << std::endl;
  if ( ! p1df->add( *p1db ) ) throw std::runtime_error( "Could not increment two compatible profiles" );
  this->dumpProfileInfo1D( *p1df, out );

  out << "Adding the last profile with an incompatible unbounded profile" << std::endl;
  if ( p1df->add( *p1dc ) ) throw std::runtime_error( "Increment two incompatible profiles" );
  this->dumpProfileInfo1D( *p1df, out );
}





void
HistogramUnitTester::dumpProfileInfo1D( const AIDA::IProfile1D& h, std::ostream& os ) const
{
  os << "************ PROFILE 1D **** START *******" << std::endl;
  this->dumpProfileInfo( h, os );
  os << "Mean : " << h.mean() << std::endl
     << "Rms : " << h.rms() << std::endl
     << std::endl;

  // Dump the profile contents...
  os << "Bin contents: bin number / bin low edge / bin centre / bin high edge / bin height / bin entries / bin error / bin rms" << std::endl;
  const AIDA::IAxis& axis = h.axis();
  for ( int i = 0; i < axis.bins(); ++i ) {
    os << i << "\t" << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
       << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << "\t" << h.binRms( i ) << std::endl;
  }
  int i = static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN );
  os << "UNDERFLOW_BIN " << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
     << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << h.binRms( i ) << std::endl;

  i = static_cast< int >( AIDA::IAxis::OVERFLOW_BIN );
  os << "OVERFLOW_BIN " << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
     << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << h.binRms( i ) << std::endl;


  os << "************ PROFILE 1D ***** END ********" << std::endl;
}
