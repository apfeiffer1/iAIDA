#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr<AIDA::ITree> tree( tf->create() );

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

  // Creating a histogram
  AIDA::IHistogram1D& h1 = *( hf->createHistogram1D( "histo", "Example histogram.", 50, 0, 50 ) );

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h1.fill( 50 * static_cast<double>( std::rand() ) / RAND_MAX );
  }

  // Printing some statistical values of the histogram
  std::cout << "Mean : " << h1.mean() << std::endl;
  std::cout << "RMS : " << h1.rms() << std::endl;

  // Printing the contents of the histogram
  std::cout << "X value     entries    Y value (height)" << std::endl;
  const AIDA::IAxis& xAxis = h1.axis();
  for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
    std::cout << h1.binMean( iBin )
              << "       " << h1.binEntries( iBin )
              << "         " << h1.binHeight( iBin )
              << std::endl;
  }

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1.annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
    std::string key = annotation.key( annotationIndex );
    std::cout << annotation.key( annotationIndex ) << " : "
	      << annotation.value( annotationIndex ) << std::endl;
  }

  return 0;
}
