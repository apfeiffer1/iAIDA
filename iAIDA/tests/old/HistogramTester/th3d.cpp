#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

void
HistogramUnitTester::testHistogram3D( std::ostream& out )
{
  out << "Creating an 3D fixed-binned histogram" << std::endl;
  const int numberOfBins = 5;
  const double lowEdge = 0;
  const double highEdge = 5;
  const std::string title = "Title of Histogram 3D";
  const std::string name = "h3d";
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3da( m_hf.createHistogram3D( title, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge ) );
  if ( ! h3da.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3da->setName( name + "a" );
  this->dumpHistogramInfo3D( *h3da, out );

  out << "Filling it with random uniform data" << std::endl;
  const int totalEntries = 1000000;
  std::srand( 0 );
  const double lowFillEdge = h3da->xAxis().lowerEdge() - 1;
  const double highFillEdge = h3da->xAxis().upperEdge() + 1;

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h3da->fill( x, y, z, w );
  }
  this->dumpHistogramInfo3D( *h3da, out );

  out << "Reseting it." << std::endl;
  if ( ! h3da->reset() ) throw std::runtime_error( "Could not reset the histogram" );
  this->dumpHistogramInfo3D( *h3da, out );

  out << "ReFilling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h3da->fill( x, y, z, w );
  }
  this->dumpHistogramInfo3D( *h3da, out );

  const double scaleFactor = 0.5;
  out << "Scaling it with a factor of " << scaleFactor << std::endl;
  if ( ! h3da->scale( scaleFactor ) ) throw std::runtime_error( "Could not scale the histogram" );
  this->dumpHistogramInfo3D( *h3da, out );

  out << "Creating a new variable sized binned histogram compatile with the first one." << std::endl;
  std::vector< double > edgesCompatible( numberOfBins + 1 );
  for ( int i = 0; i < numberOfBins; ++i ) edgesCompatible[i] = h3da->xAxis().binLowerEdge(i);
  edgesCompatible[numberOfBins] = h3da->xAxis().upperEdge();
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3db( m_hf.createHistogram3D( title + "(3)", edgesCompatible, edgesCompatible, edgesCompatible, "" ) );
  if ( ! h3db.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3db->setName( name + "b" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h3db->fill( x, y, z, w );
  }
  this->dumpHistogramInfo3D( *h3db, out );

  out << "Incrementing the first histogram with the second." << std::endl;
  if ( ! h3da->add( *h3db ) ) throw std::runtime_error( "Could not increment two compatible histograms" );
  this->dumpHistogramInfo3D( *h3da, out );

  out << "Creating a non-compatible histogram" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3dc( m_hf.createHistogram3D( title, numberOfBins * 2, lowEdge, highEdge, numberOfBins, lowEdge, highEdge, numberOfBins, lowEdge, highEdge ) );
  if ( ! h3dc.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3dc->setName( name + "c" );
  out << "Filling it with random uniform data" << std::endl;
  std::srand( 0 );
  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double y = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double z = lowFillEdge + ( highFillEdge - lowFillEdge ) * ( static_cast<double>( std::rand() ) / RAND_MAX );
    const double w =  static_cast<double>( std::rand() ) / RAND_MAX;
    h3dc->fill( x, y, z, w );
  }
  this->dumpHistogramInfo3D( *h3dc, out );

  out << "Incrementing the third histogram with the second." << std::endl;
  if ( h3dc->add( *h3db ) ) throw std::runtime_error( "Incremented two incompatible histograms!!!" );
  this->dumpHistogramInfo3D( *h3dc, out );

  out << "Creating a copy of the third histogram." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3dd( m_hf.createCopy( *h3dc ) );
  if ( ! h3dd.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3dd->setName( name + "d" );
  this->dumpHistogramInfo3D( *h3dd, out );

  // add
  out << "Creating an addition histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3de( m_hf.add( *h3dc, *h3dd ) );
  if ( ! h3de.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3de->setName( name + "e" );
  this->dumpHistogramInfo3D( *h3de, out );

  // subtract
  out << "Creating a subtraction histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3df( m_hf.subtract( *h3dc, *h3dd ) );
  if ( ! h3df.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3df->setName( name + "f" );
  this->dumpHistogramInfo3D( *h3df, out );

  // multiply
  out << "Creating a multiplication histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3dg( m_hf.multiply( *h3dc, *h3dd ) );
  if ( ! h3dg.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3df->setName( name + "g" );
  this->dumpHistogramInfo3D( *h3dg, out );

  // divide
  out << "Creating a division histogram from the third and fourth" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram3D > h3dh( m_hf.divide( *h3dc, *h3dd ) );
  if ( ! h3dh.get() ) throw std::runtime_error( "Could not create a Histogram3D" );
  h3dh->setName( name + "h" );
  this->dumpHistogramInfo3D( *h3dh, out );

}





void
HistogramUnitTester::dumpHistogramInfo3D( const AIDA::IHistogram3D& h, std::ostream& os ) const
{
  os << "************ HISTOGRAM 3D **** START *******" << std::endl;
  this->dumpHistogramInfo( h, os );
  os << "MeanX : " << h.meanX() << std::endl
     << "RmsX : " << h.rmsX() << std::endl
     << "MeanY : " << h.meanY() << std::endl
     << "RmsY : " << h.rmsY() << std::endl
     << "MeanZ : " << h.meanZ() << std::endl
     << "RmsZ : " << h.rmsZ() << std::endl
     << "Extra entries : " << h.extraEntries() << std::endl
     << std::endl;

  // Dump the histogram contents...
  os << "Bin contents: bin number x / bin number y / bin number z / bin low edge x / bin centre x / bin high edge x / bin low edge y / bin centre y / bin high edge y / bin low edge z / bin centre z / bin high edge z / bin height / bin entries / bin error" << std::endl;
  const AIDA::IAxis& xAxis = h.xAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    const AIDA::IAxis& yAxis = h.yAxis();
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      const AIDA::IAxis& zAxis = h.zAxis();
      for ( int k = 0; k < zAxis.bins(); ++k ) {
	os << i << "\t" << j << "\t" << k << "\t"
	   << xAxis.binLowerEdge( i ) << "\t" << h.binMeanX( i,j,k ) << "\t" << xAxis.binUpperEdge( i ) << "\t"
	   << yAxis.binLowerEdge( j ) << "\t" << h.binMeanY( i,j,k ) << "\t" << yAxis.binUpperEdge( j ) << "\t"
	   << zAxis.binLowerEdge( k ) << "\t" << h.binMeanZ( i,j,k ) << "\t" << zAxis.binUpperEdge( k ) << "\t"
	   << h.binHeight( i,j,k ) << "\t" << h.binEntries( i,j,k ) << "\t" << h.binError( i,j,k ) << std::endl;
      }
    }
  }
  
  os << "************ HISTOGRAM 3D ***** END ********" << std::endl;
}
