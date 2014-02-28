#include <iostream>
#include <cstdlib>
#include <memory>

#include <boost/random.hpp>
#if BOOST_VERSION > 104100
#include <boost/random/uniform_int_distribution.hpp>
#else
#include <boost/random/uniform_int.hpp>
#endif
#include <boost/random/normal_distribution.hpp>

#include "AIDA/AIDA.h"

#include "AIDA_CPP/Exceptions.h"
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <typeinfo>
#include <boost/shared_ptr.hpp>

int main( int, char** ) {

   // init the random engine
   boost::mt19937 rng;   // I don't seed it on purpouse (it's not relevant)
   boost::normal_distribution<> nd(0.0, 10.0);
   boost::variate_generator<boost::mt19937&, 
                            boost::normal_distribution<> > var_nor(rng, nd);

  // create a tree

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 
  bool readOnly = false;
  bool createNew = true;
  std::string options = "uncompress";        // default: use zip compression
  boost::shared_ptr<AIDA::ITree> tree(tf->create("histos.xml","XML",readOnly,createNew, options)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  // Creating a histogram managed by the tree (cannot be smart pointers) 
  AIDA::IHistogram1D * h1p = factory->createHistogram1D("1", "Example histogram 1 (gauss)", 50, 0, 50);
  AIDA::IHistogram1D * h2p = factory->createHistogram1D("2", "Example histogram 2 (flat)", 50, 0, 50);

  AIDA::IHistogram2D * h10p = factory->createHistogram2D("10", "Example histogram 2d (gauss)", 20, 0, 100, 20, 0, 100);

  AIDA::IHistogram3D * h3dp = factory->createHistogram3D("13", "Example histogram 3d (gauss)", 10, 0, 100, 10, 0, 100, 10, 0, 100);

  AIDA::IProfile1D   * p1p = factory->createProfile1D  ("Example profile (gauss)", 50, 0, 50 );

  AIDA::IProfile2D   * p2p = factory->createProfile2D  ("Example 2D profile (gauss)", 10, 0, 50, 10, 0, 50 );

  AIDA::IHistogram1D & h1 = *h1p;
  AIDA::IProfile1D & p1 = *p1p;

  // Filling the histogram with random data
  // gaussians (mean 0, sig 10):
  for ( int i = 0; i < 1000; ++i ) {
     double x = var_nor();
     double y = var_nor();
     double z = var_nor();
     h1p->fill ( x+25, 1. );           // shift to center of histo 
     h10p->fill( x+25, y+50 );         // shift distrib to inside histo
     h3dp->fill( x+25, y+50, z+30 );   // shift distrib to inside histo
     p1p->fill ( x+20, y+30 );         // shift distrib to inside histo
     p2p->fill ( x+25, y+20, z+50 );   // shift distrib to inside histo
  }
  
  // flat
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h2p->fill( x+5, 1. );
  }

  {
    // Printing some statistical values of the histogram
    std::cout << "Mean : " << h1.mean() << std::endl;
    std::cout << "RMS  : " << h1.rms() << std::endl;
    
    // Printing the contents of the histogram
    std::cout << "X value     entries    Y value (height)" << std::endl;
    const AIDA::IAxis& xAxis = h1.axis();
    for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
      std::cout << h1.binMean( iBin )
		<< "       " << h1.binEntries( iBin )
		<< "         " << h1.binHeight( iBin )
		<< std::endl;
    }
  }
  std::cout << std::endl;

  {
    // Printing some statistical values of the profile
    std::cout << "Mean : " << p1.mean() << std::endl;
    std::cout << "RMS  : " << p1.rms() << std::endl;
    
    // Printing the contents of the histogram
    std::cout << "X value     entries    Y value    Y rms" << std::endl;
    const AIDA::IAxis& xAxis = p1.axis();
    for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
      std::cout << p1.binMean( iBin )
		<< "       " << p1.binEntries( iBin )
		<< "         " << p1.binHeight( iBin )
		<< "         " << p1.binRms( iBin )
		<< std::endl;
    }
  }

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1.annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
      std::string key = annotation.key( annotationIndex );
      std::cout << annotation.key( annotationIndex ) << " : "
                << annotation.value( annotationIndex ) << std::endl;
  }

  std::cout << "tree listing : " << std::endl; 
  // browse the tree
  tree->ls(); 

  std::cout << "storename " << tree->storeName() << std::endl; 

  // save files 
  std::cout << "write tree contents to disk " << std::endl; 
  tree->commit(); 

  std::cout << "That's it !" << std::endl;

  return 0;
}
