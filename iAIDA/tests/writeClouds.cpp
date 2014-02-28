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
  boost::shared_ptr<AIDA::ITree> tree(tf->create("clouds.xml","XML",readOnly,createNew, options)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  // Creating a cloud managed by the tree (cannot be smart pointers) 
  AIDA::ICloud1D * h1p = factory->createCloud1D("21", "Example cloud 1 (gauss)");
  AIDA::ICloud1D * h2p = factory->createCloud1D("22", "Example cloud 2 (flat)" );

  AIDA::ICloud2D * h10p = factory->createCloud2D("30", "Example cloud 2d (gauss)");

  AIDA::ICloud3D * c3dp = factory->createCloud3D("33", "Example cloud 3d (gauss)");

  // Filling the cloud with random data
  // gaussians (mean 0, sig 10):
  for ( int i = 0; i < 100; ++i ) {
     double x = var_nor();
     double y = var_nor();
     double z = var_nor();
     h1p->fill ( x+25, 1. );
     h10p->fill( x+25, y+50 );
     c3dp->fill( x+25, y+50, z );
  }
  
  // flat
  std::srand( 0 );
  for ( int i = 0; i < 110; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h2p->fill( x+5, 1. );
  }

   {
      // Printing some statistical values of the cloud
      std::cout << "Mean : " << h1p->mean() << std::endl;
      std::cout << "RMS  : " << h1p->rms() << std::endl;
      std::cout << std::endl;
   }

  // Printing the annotation values of the cloud
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1p->annotation();
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
