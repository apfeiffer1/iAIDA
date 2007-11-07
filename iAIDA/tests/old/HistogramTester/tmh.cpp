#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA/IAxis.h"
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include <limits>

void
HistogramUnitTester::testMergeHistogram( std::ostream& out )
{
  out << "Creating an 1D fixed-binned histogram" << std::endl;
  const int numberOfBins = 100;
  const double lowEdge = 0;
  const double highEdge = 1.;
  const std::string title = "Title of Histogram 1D";
  const std::string name = "h1d";
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1da( m_hf.createHistogram1D( title, numberOfBins, lowEdge, highEdge ) );
  if ( ! h1da.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1da->setName( name + "a" );
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Filling it with some data" << std::endl;
  const int totalEntries = 111112;
  std::srand( 0 );

  for ( int i = 0; i < totalEntries; ++i ) {
    const double x = 0.356;
    h1da->fill( x );
  }
  this->dumpHistogramInfo1D( *h1da, out );

  out << "Creating a copy of the first histogram." << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1dd( m_hf.createCopy( *h1da ) );
  if ( ! h1dd.get() ) throw std::runtime_error( "Could not create a Histogram1D" );
  h1dd->setName( name + "copy" );
  this->dumpHistogramInfo1D( *h1dd, out );

  // add
  out << "Creating a histogram by adding the first and its copy" << std::endl;
  std::auto_ptr< AIDA::Dev::IDevHistogram1D > h1de( m_hf.add( *h1da, *h1dd ) );
  if ( ! h1de.get() ) throw std::runtime_error( "Could not create the added Histogram1D" );
  h1de->setName( name + "add" );
  this->dumpHistogramInfo1D( *h1de, out );

}


