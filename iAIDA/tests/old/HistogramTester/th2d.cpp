#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testHistogram2D( std::ostream& out )
{
  out << "Creating an 2D fixed-binned histogram" << std::endl;
  const int numberOfBins = 10;
  const double lowEdge = 0;
  const double highEdge = 5;
  const std::string title = "Title of Histogram 2D";
  const std::string name = "h2d";
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2da( m_hf.createHistogram2D( title, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge ) );
  if ( ! h2da.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2da->setName( name + "a" );
  this->dumpHistogramInfo2D( *h2da, out );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = h2da->xAxis().lowerEdge() - 1;
  const double highFillEdge = h2da->xAxis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h2da->fill( x, y, w );
  }
  this->dumpHistogramInfo2D( *h2da, out );

  out << "Reseting it." << std::endl;
  if ( ! h2da->reset() ) throw std::runtime_error( "Could not reset the histogram" );
  this->dumpHistogramInfo2D( *h2da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h2da->fill( x, y, w );
  }
  this->dumpHistogramInfo2D( *h2da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! h2da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the histogram" );
  this->dumpHistogramInfo2D( *h2da, out );

  out << "Creating a new variable sized binned histogram compatile with the first one." << std::endl;
  std::vector< double > edgesCompatible( numberOfBins + 1 );
  for ( int i = 0; i < numberOfBins; ++i ) edgesCompatible[i] = h2da->xAxis().binLowerEdge(i);
  edgesCompatible[numberOfBins] = h2da->xAxis().upperEdge();
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2db( m_hf.createHistogram2D( title + "(2)", edgesCompatible, edgesCompatible, "" ) );
  if ( ! h2db.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2db->setName( name + "b" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h2db->fill( x, y, w );
  }
  this->dumpHistogramInfo2D( *h2db, out );

  out << "Incrementing the first histogram with the second." << std::endl;
  if ( ! h2da->add( *h2db ) ) throw std::runtime_error( "Could not increment two compatible histograms" );
  this->dumpHistogramInfo2D( *h2da, out );

  out << "Creating a non-compatible histogram" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2dc( m_hf.createHistogram2D( title, numberOfBins / 2, lowEdge, highEdge, numberOfBins / 2, lowEdge, highEdge ) );
  if ( ! h2dc.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2dc->setName( name + "c" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h2dc->fill( x, y, w );
  }
  this->dumpHistogramInfo2D( *h2dc, out );

  out << "Incrementing the third histogram with the second." << std::endl;
  if ( h2dc->add( *h2db ) ) throw std::runtime_error( "Incremented two incompatible histograms!!!" );
  this->dumpHistogramInfo2D( *h2dc, out );

  out << "Creating a copy of the third histogram." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2dd( m_hf.createCopy( *h2dc ) );
  if ( ! h2dd.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2dd->setName( name + "d" );
  this->dumpHistogramInfo2D( *h2dd, out );

  // add
  out << "Creating an addition histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2de( m_hf.add( *h2dc, *h2dd ) );
  if ( ! h2de.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2de->setName( name + "e" );
  this->dumpHistogramInfo2D( *h2de, out );

  // subtract
  out << "Creating a subtraction histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2df( m_hf.subtract( *h2dc, *h2dd ) );
  if ( ! h2df.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2df->setName( name + "f" );
  this->dumpHistogramInfo2D( *h2df, out );

  // multiply
  out << "Creating a multiplication histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2dg( m_hf.multiply( *h2dc, *h2dd ) );
  if ( ! h2dg.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2df->setName( name + "g" );
  this->dumpHistogramInfo2D( *h2dg, out );

  // divide
  out << "Creating a division histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram2D > h2dh( m_hf.divide( *h2dc, *h2dd ) );
  if ( ! h2dh.get() ) throw std::runtime_error( "Could not create a Histogram2D" );
  h2dh->setName( name + "h" );
  this->dumpHistogramInfo2D( *h2dh, out );

}





void
HistogramUnitTester::dumpHistogramInfo2D( const AIDA::IHistogram2D& h, std::ostream& os ) const
{
  os << "************ HISTOGRAM 2D **** START *******" << std::endl;
  this->dumpHistogramInfo( h, os );
  os << "MeanX : " << h.meanX() << std::endl
     << "RmsX : " << h.rmsX() << std::endl
     << "MeanY : " << h.meanY() << std::endl
     << "RmsY : " << h.rmsY() << std::endl
     << "Extra entries : " << h.extraEntries() << std::endl
     << std::endl;

  // Dump the histogram contents...
  os << "Bin contents: bin number x / bin number y / bin low edge x / bin centre x / bin high edge x / bin low edge y / bin centre y / bin high edge y / bin height / bin entries / bin error" << std::endl;
  const AIDA::IAxis& xAxis = h.xAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    const AIDA::IAxis& yAxis = h.yAxis();
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      os << i << "\t" << j << "\t"
	 << xAxis.binLowerEdge( i ) << "\t" << h.binMeanX( i,j ) << "\t" << xAxis.binUpperEdge( i ) << "\t"
	 << yAxis.binLowerEdge( j ) << "\t" << h.binMeanY( i,j ) << "\t" << yAxis.binUpperEdge( j ) << "\t"
	 << h.binHeight( i,j ) << "\t" << h.binEntries( i,j ) << "\t" << h.binError( i,j ) << std::endl;
    }
  }
  
  os << "************ HISTOGRAM 2D ***** END ********" << std::endl;
}
