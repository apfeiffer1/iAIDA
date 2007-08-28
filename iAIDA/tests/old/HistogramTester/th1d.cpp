#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include <limits>

void
HistogramUnitTester::testHistogram1D( std::ostream& out )
{
  out << "Creating an 1D fixed-binned histogram" << std::endl;
  const int numberOfBins = 100;
  const double lowEdge = 0;
  const double highEdge = 50;
  const std::string title = "Title of Histogram 1D";
  const std::string name = "h1d";
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1da( m_hf.createHistogram1D( title, numberOfBins, lowEdge, highEdge ) );
  if ( ! h1da.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1da->setName( name + "a" );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = h1da->axis().lowerEdge() - 1;
  const double highFillEdge = h1da->axis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h1da->fill( x, w );
  }
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Filling it with infinity" << std::endl;
  h1da->fill( std::numeric_limits<int>::infinity());
  h1da->fill( std::numeric_limits<float>::infinity());
  h1da->fill( std::numeric_limits<double>::infinity());

  out << "Filling it with quiet_NaN" << std::endl;
  h1da->fill( std::numeric_limits<int>::quiet_NaN(), std::numeric_limits<int>::quiet_NaN() );
  h1da->fill( std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() );
  h1da->fill( std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() );

  out << "Filling it with signalling_NaN" << std::endl;
  h1da->fill( std::numeric_limits<int>::signaling_NaN(), std::numeric_limits<int>::signaling_NaN() );
  h1da->fill( std::numeric_limits<float>::signaling_NaN(), std::numeric_limits<float>::signaling_NaN() );
  h1da->fill( std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN() );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Reseting it." << std::endl;
  if ( ! h1da->reset() ) throw std::runtime_error( "Could not reset the histogram" );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h1da->fill( x, w );
  }
  this->dumpHistogramInfo1D( *h1da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! h1da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the histogram" );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Creating a new variable sized binned histogram compatile with the first one." << std::endl;
  std::vector< double > edgesCompatible( numberOfBins + 1 );
  for ( int i = 0; i < numberOfBins; ++i ) edgesCompatible[i] = h1da->axis().binLowerEdge(i);
  edgesCompatible[numberOfBins] = h1da->axis().upperEdge();
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1db( m_hf.createHistogram1D( title + "(2)", edgesCompatible, "" ) );
  if ( ! h1db.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1db->setName( name + "b" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h1db->fill( x, w );
  }
  this->dumpHistogramInfo1D( *h1db, out );

  out << "Incrementing the first histogram with the second." << std::endl;
  if ( ! h1da->add( *h1db ) ) throw std::runtime_error( "Could not increment two compatible histograms" );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Creating a non-compatible histogram" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1dc( m_hf.createHistogram1D( title, numberOfBins / 2, lowEdge, highEdge ) );
  if ( ! h1dc.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1dc->setName( name + "c" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h1dc->fill( x, w );
  }
  this->dumpHistogramInfo1D( *h1dc, out );

  out << "Incrementing the third histogram with the second." << std::endl;
  if ( h1dc->add( *h1db ) ) throw std::runtime_error( "Incremented two incompatible histograms!!!" );
  this->dumpHistogramInfo1D( *h1dc, out );

  out << "Creating a copy of the third histogram." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1dd( m_hf.createCopy( *h1dc ) );
  if ( ! h1dd.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1dd->setName( name + "d" );
  this->dumpHistogramInfo1D( *h1dd, out );

  // add
  out << "Creating an addition histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1de( m_hf.add( *h1dc, *h1dd ) );
  if ( ! h1de.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1de->setName( name + "e" );
  this->dumpHistogramInfo1D( *h1de, out );

  // subtract
  out << "Creating a subtraction histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1df( m_hf.subtract( *h1dc, *h1dd ) );
  if ( ! h1df.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1df->setName( name + "f" );
  this->dumpHistogramInfo1D( *h1df, out );

  // multiply
  out << "Creating a multiplication histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1dg( m_hf.multiply( *h1dc, *h1dd ) );
  if ( ! h1dg.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1df->setName( name + "g" );
  this->dumpHistogramInfo1D( *h1dg, out );

  // divide
  out << "Creating a division histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1dh( m_hf.divide( *h1dc, *h1dd ) );
  if ( ! h1dh.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1dh->setName( name + "h" );
  this->dumpHistogramInfo1D( *h1dh, out );

}





void
HistogramUnitTester::dumpHistogramInfo1D( const AIDA::IHistogram1D& h, std::ostream& os ) const
{
  os << "************ HISTOGRAM 1D **** START *******" << std::endl;
  this->dumpHistogramInfo( h, os );
  os << "Mean : " << h.mean() << std::endl
     << "Rms : " << h.rms() << std::endl
     << std::endl;

  // Dump the histogram contents...
  os << "Bin contents: bin number / bin low edge / bin centre / bin high edge / bin height / bin entries / bin error" << std::endl;
  const AIDA::IAxis& axis = h.axis();
  for ( int i = 0; i < axis.bins(); ++i ) {
    os << i << "\t" << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
       << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << std::endl;
  }
  int i = static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN );
  os << "UNDERFLOW_BIN " << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
     << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << std::endl;

  i = static_cast< int >( AIDA::IAxis::OVERFLOW_BIN );
  os << "OVERFLOW_BIN " << axis.binLowerEdge( i ) << "\t" << h.binMean( i ) << "\t" << axis.binUpperEdge( i ) << "\t"
     << h.binHeight( i ) << "\t" << h.binEntries( i ) << "\t" << h.binError( i ) << std::endl;


  os << "************ HISTOGRAM 1D ***** END ********" << std::endl;
}
