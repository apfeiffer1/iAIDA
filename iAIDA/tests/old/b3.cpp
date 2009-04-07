#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr< AIDA::ITree > tree( tf->create() );

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );

  // Printing the contents of the (empty) tree
  std::cout << "Tree contents before the creation of any histogram." << std::endl;
  tree->ls();

  std::cout << "Creating an 1-dimensional histogram" << std::endl;
  AIDA::IHistogram1D* h1p = hf->createHistogram1D( "histo_1", "Example 1D histogram.", 50, 0, 50 );
  AIDA::IHistogram1D& h1 = *h1p;

  // Printing the histogram title
  std::cout << "Histogram title : " << h1.title() << std::endl;

  // Printing the contents of the tree
  std::cout << "Tree contents after the creation of the 1st histogram." << std::endl;
  tree->ls();

  std::cout << "Creating a new directory" << std::endl;
  tree->mkdir( "Dir_1" );
  tree->ls();

  // Entering the new directory
  tree->cd( "Dir_1" );

  std::cout << "Creating a 2-dimensional histogram" << std::endl;
  AIDA::IHistogram2D* h2p = hf->createHistogram2D( "histo_2", "Example 2D histogram.", 50, 0, 50, 20, 0, 20 );
  AIDA::IHistogram2D& h2 = *h2p;
  std::cout << "Histogram title : " << h2.title() << std::endl;

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h2.fill( 50 * static_cast<double>( std::rand() ) / RAND_MAX,
	     20 * static_cast<double>( std::rand() ) / RAND_MAX );
  }
  std::cout << "Bins in X : " << h2.xAxis().bins() << std::endl;
  std::cout << "Mean X : " << h2.meanX() << std::endl;
  std::cout << "RMS X  : " << h2.rmsX() << std::endl;
  std::cout << "Bins in Y : " << h2.yAxis().bins() << std::endl;
  std::cout << "Mean Y : " << h2.meanY() << std::endl;
  std::cout << "RMS Y  : " << h2.rmsY() << std::endl;

  std::cout << "Creating a 3-dimensional histogram" << std::endl;
  AIDA::IHistogram3D* h3p = hf->createHistogram3D( "histo_3", "Example 3D histogram.", 50, 0, 50, 20, 0, 20, 10, 0, 10 );
  AIDA::IHistogram3D& h3 = *h3p;
  std::cout << "Histogram title : " << h3.title() << std::endl;

  // Filling the histogram with random data
  for ( int i = 0; i < 1000; ++i ) {
    h2.fill( 50 * static_cast<double>( std::rand() ) / RAND_MAX,
	     20 * static_cast<double>( std::rand() ) / RAND_MAX,
	     10 * static_cast<double>( std::rand() ) / RAND_MAX );
  }
  std::cout << "Bins in X : " << h3.xAxis().bins() << std::endl;
  std::cout << "Mean X : " << h3.meanX() << std::endl;
  std::cout << "RMS X  : " << h3.rmsX() << std::endl;
  std::cout << "Bins in Y : " << h3.yAxis().bins() << std::endl;
  std::cout << "Mean Y : " << h3.meanY() << std::endl;
  std::cout << "RMS Y  : " << h3.rmsY() << std::endl;
  std::cout << "Bins in Z : " << h3.zAxis().bins() << std::endl;
  std::cout << "Mean Z : " << h3.meanZ() << std::endl;
  std::cout << "RMS Z  : " << h3.rmsZ() << std::endl;
  
  // Printing the contents of the current directory in the tree
  std::cout << "Tree contents in the directory " << tree->pwd() << std::endl;
  tree->ls();

  std::cout << "Creating another directory" << std::endl;
  tree->mkdir( "/Dir_2" );
  tree->cd( "/Dir_2" );

  std::cout << "Copying a histogram into the new directory" << std::endl;
  tree->cp( "../histo_1", "." );

  std::cout << "Moving a histogram into the new directory" << std::endl;
  tree->mv( "/Dir_1/histo_2", "." );


  tree->cd( ".." );
  std::cout << "Creating another directory" << std::endl;
  tree->mkdir( "Dir_3" );
  tree->cd( "Dir_3" );

  std::cout << "Creating other histogram types" << std::endl;  
  hf->createCloud1D( "cloud_1", "Example 1-D cloud" );
  hf->createCloud2D( "cloud_2", "Example 2-D cloud" );
  hf->createCloud3D( "cloud_3", "Example 3-D cloud" );

  hf->createProfile1D( "profile_1", "Example 1-D profile", 100, 0, 50 );
  hf->createProfile2D( "profile_2", "Example 2-D profile", 100, 0, 50, 100, 0, 50 );

  std::cout << "Fetching a profile histogram from the tree" << std::endl;
  AIDA::IManagedObject* mo = tree->find( "profile_1" );
  AIDA::IProfile1D& profile = dynamic_cast<AIDA::IProfile1D&>( *mo );
  std::cout << "Profile histogram title : " << profile.title() << std::endl;


  // Listing the full contents of the tree
  std::cout << "Final contents of the tree" << std::endl;
  tree->ls( "/", true );

  return 0;
}
