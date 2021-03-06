#include <iostream>
#include <cstdlib>
#include <memory>

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
  // create a tree

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 
  bool readOnly = false;
  bool createNew = true;
  std::string options = ""; // use default: zip compression
  boost::shared_ptr<AIDA::ITree> tree(tf->create("exaTree.aida","XML",readOnly,createNew, options)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  // Creating a histogram managed by the tree (cannot be smart pointers) 
  AIDA::IHistogram1D * h1p = factory->createHistogram1D("1", "Example histogram.", 50, 0, 50);
  AIDA::IHistogram1D * h2p = factory->createHistogram1D("2", "Example histogram.", 50, 0, 50);
  AIDA::IHistogram1D * h3p = factory->createHistogram1D("7", "Example histogram.", 50, 0, 50);
  AIDA::IHistogram1D * h4p = factory->createHistogram1D("10", "Example histogram.", 50, 0, 50);
  AIDA::IProfile1D   * p1p = factory->createProfile1D  ("Example profile.", 50, 0, 50 );

  AIDA::IHistogram1D & h1 = *h1p;
  AIDA::IProfile1D & p1 = *p1p;

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h1.fill( x, 1. );
    h2p->fill( x+5, 1. );
    h3p->fill( x*2, 1. );
    h4p->fill( x/2, 1. );
  }

  for ( int i = 0; i < 1000; ++i ) {
     double x =  50. * static_cast<double>( std::rand() ) / RAND_MAX;
     double y = 100. * static_cast<double>( std::rand() ) / RAND_MAX;
     p1.fill( x, y);
     p1.fill( x, y);
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

  std::cout << "\ngoing to create file for gzip copy " << std::endl; 
  {
  // test copy to another tree 
  bool readOnly2  = false;
  bool createNew2 = true;
  std::string options2 = "gzip"; // this is now default in iAIDA
  boost::shared_ptr<AIDA::ITree> tree2(tf->create("exaTreeCopyGzip.aida","XML",readOnly2, createNew2, options2)); 
  // create histogram factory
  boost::shared_ptr<AIDA::IHistogramFactory> factory2( af->createHistogramFactory(*tree2) );
  factory2->createCopy("1",h1);
  factory2->createCopy("2",*h2p);
  factory2->createCopy("7",*h3p);
  factory2->createCopy("10",*h4p);
  factory2->createCopy("Example profile.",p1);
  tree2->commit();
  tree2->close(); 
  }

  std::cout << "\ngoing to create file for bzip copy " << std::endl; 
  {
  // test copy to another tree 
  bool readOnly2  = false;
  bool createNew2 = true;
  std::string options2 = "bzip"; // this is now default in iAIDA
  boost::shared_ptr<AIDA::ITree> tree2(tf->create("exaTreeCopyBzip.aida","XML",readOnly2, createNew2, options2)); 
  // create histogram factory
  boost::shared_ptr<AIDA::IHistogramFactory> factory2( af->createHistogramFactory(*tree2) );
  factory2->createCopy("1",h1);
  factory2->createCopy("2",*h2p);
  factory2->createCopy("7",*h3p);
  factory2->createCopy("10",*h4p);
  factory2->createCopy("Example profile.",p1);
  tree2->commit();
  tree2->close(); 
  }

  std::cout << "\ngoing to create file for uncompressed copy " << std::endl; 
  {
  // test copy to another tree 
  bool readOnly3  = false;
  bool createNew3 = true;
  std::string options3 = "uncompress"; 
  boost::shared_ptr<AIDA::ITree> tree3(tf->create("exaTreeCopyUC.aida","XML",readOnly3, createNew3, options3)); 
  // create histogram factory
  boost::shared_ptr<AIDA::IHistogramFactory> factory3( af->createHistogramFactory(*tree3) );
  factory3->createCopy("1",h1);
  factory3->createCopy("2",*h2p);
  factory3->createCopy("7",*h3p);
  factory3->createCopy("10",*h4p);
  factory3->createCopy("Example profile.",p1);
  tree3->commit();
  tree3->close(); 
  }
  std::cout << "closing the tree " << std::endl; 
  tree->close(); 

  std::cout << "That's it !" << std::endl;

  return 0;
}
