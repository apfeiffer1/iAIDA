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

  // Creating a tree mapped to an hbook file
  bool readOnly = false;
  bool createNew = true;
  std::auto_ptr<AIDA::ITree> tree( tf->create("histo.hbook", "hbook", readOnly, createNew ) );

  std::cout << "Created a Tree with store " << tree->storeName() << std::endl;  
  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

  // Creating a histogram
  AIDA::IHistogram1D& h1 = *( hf->createHistogram1D( "1", "Example histogram 1.", 50, 0, 50 ) );

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h1.fill( 25 * static_cast<double>( std::rand() ) / RAND_MAX );
  }

  // Creating a histogram
  AIDA::IHistogram1D& h2 = *( hf->createHistogram1D( "2", "Example histogram 2.", 50, 0, 50 ) );

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h2.fill( 25 + 25 * static_cast<double>( std::rand() ) / RAND_MAX );
  }


  // Creating the plotter factory (
  /*
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );

  // Creating a plotter
  std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
  plotter->show();

  
  // Plotting the first histogram
  plotter->currentRegion().plot( h1, "annotation" );
  plotter->refresh();
  std::cout << "Press <ENTER> to continue" << std::endl;
  std::cin.get();

  // Plotting the secnd histogram
  plotter->currentRegion().clear();
  plotter->currentRegion().plot( h2, "annotation" );
  plotter->refresh();
  std::cout << "Press <ENTER> to continue" << std::endl;
  std::cin.get();

  */

  // Flushing the histograms into the file
  tree->commit();

  // Explicitly closing the tree
  tree->close();

  return 0;
}
