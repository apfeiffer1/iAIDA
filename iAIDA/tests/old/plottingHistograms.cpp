#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  try {
    std::cout << "Creating an analysis factory" << std::endl;
    std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
    if ( ! af.get() ) {
      throw std::runtime_error( "Could not instantiate an analysis factory" );
    }

    std::cout << "Creating a tree factory" << std::endl;
    std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );
    if ( ! tf.get() ) {
      throw std::runtime_error( "Could not instantiate a tree factory" );
    }

    std::cout << "Creating a memory-mapped tree" << std::endl;
    std::auto_ptr<AIDA::ITree> tree( tf->create("Memory tree") );
    if ( ! tree.get() ) {
      throw std::runtime_error( "Could not instantiate a tree" );
    }
    std::cout << "Store name of the tree : " << tree->storeName() << std::endl;

    std::cout << "Creating a histogram factory" << std::endl;
    std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

    std::cout << "Creating a histogram." << std::endl;
    AIDA::IHistogram1D* ph1 = hf->createHistogram1D( "h1", "Example 1-d histogram", 100, 0, 100 );
    if ( ! ph1 ) throw std::runtime_error( "Could not create an 1-dimensional histogram" );
    AIDA::IHistogram1D& h1 = *ph1;
    std::cout << "Histogram title : " << h1.title() << std::endl;

    std::cout << "Filling it with random data" << std::endl;
    std::srand( 0 );
    for ( int i = 0; i < 1000; ++i ) {
      h1.fill( 102 * static_cast<double>( std::rand() ) / RAND_MAX  - 1 );
    }
    std::cout << "Mean : " << h1.mean() << std::endl;
    std::cout << "RMS : " << h1.rms() << std::endl;
    std::cout << std::endl;

    std::cout << "Creating a plotter factory" << std::endl;
    std::auto_ptr<AIDA::IPlotterFactory> pf( af->createPlotterFactory() );
    if ( ! pf.get() ) {
      throw std::runtime_error( "Could not instantiate a plotter factory" );
    }

    std::cout << "Creating a plotter" << std::endl;
    std::auto_ptr<AIDA::IPlotter> pl( pf->create() );
    if ( ! pl.get() ) {
      throw std::runtime_error( "Could not instantiate a plotter" );
    }
    if ( ! pl->show() ) {
      throw std::runtime_error( "Could not show the plotter." );
    }

    std::cout << "Plotting the histogram" << std::endl;
    if ( ! pl->currentRegion().plot( h1, "annotation" ) ) {
      throw std::runtime_error( "Could not plot a histogram" );
    }
    if ( ! pl->refresh() ) {
      throw std::runtime_error( "Could not update the plot." );
    }

    std::cout << "Press ENTER to continue" << std::endl;
    std::cin.get();

    tree->commit();
    tree->close();

    std::cout << "Successfull end." << std::endl;
    return 0;
  }
  catch ( std::exception & e ) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
