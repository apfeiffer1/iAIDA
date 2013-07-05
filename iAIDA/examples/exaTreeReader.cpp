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

void dumpHist(const AIDA::IHistogram1D & h1) {
   // Printing some statistical values of the histogram
   std::cout << "\nHisto1D" << std::endl;
   std::cout << "Title: " << h1.title() << std::endl;
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

void dumpHist(const AIDA::IProfile1D & p1) {
   // Printing some statistical values of the profile
   std::cout << "\nProfile1D" << std::endl;
   std::cout << "Title: " << p1.title() << std::endl;
   std::cout << "Mean : " << p1.mean() << std::endl;
   std::cout << "RMS  : " << p1.rms() << std::endl;

    // Printing the contents of the profile
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

int readAndShow(std::string fileName, std::string compression) {

  // load a tree

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 
  bool readOnly  = true;
  bool createNew = false;
  std::string options = compression;
  boost::shared_ptr<AIDA::ITree> tree(tf->create(fileName,"XML",readOnly,createNew,options)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  std::cout << "\nstorename " << tree->storeName() << std::endl; 

  std::cout << "\ntree listing : " << std::endl; 
  // browse the tree
  tree->ls(); 

  // Creating a histogram managed by the tree (cannot be smart pointers) 
  AIDA::IHistogram1D * h1p = dynamic_cast<AIDA::IHistogram1D *>( tree->find("1") );
  AIDA::IHistogram1D * h2p = dynamic_cast<AIDA::IHistogram1D *>( tree->find("2") );
  AIDA::IHistogram1D * h3p = dynamic_cast<AIDA::IHistogram1D *>( tree->find("7") );
  AIDA::IHistogram1D * h4p = dynamic_cast<AIDA::IHistogram1D *>( tree->find("10") );
  AIDA::IProfile1D   * p1p = dynamic_cast<AIDA::IProfile1D   *>( tree->find("Example profile.") );

  if ( !h1p or !h2p or !h3p or !h4p or !p1p) {
     std::cerr << "ERROR loading histograms from file ... " << std::endl;
     return -1;
  }

  AIDA::IHistogram1D & h1 = *h1p;
  AIDA::IProfile1D & p1 = *p1p;

  // dumpHist(h1);
  dumpHist(p1);

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1.annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
    std::string key = annotation.key( annotationIndex );
    std::cout << annotation.key( annotationIndex ) << " : "
	      << annotation.value( annotationIndex ) << std::endl;
  }

  std::cout << "\nclosing the tree " << std::endl; 
  tree->close(); 

  std::cout << "\nDone!" << std::endl; 
  
  return 0;
  
}

int main( int, char** )
{
   std::cout << "\n================================================================================\n" << std::endl;
   int r1 = readAndShow("exaTree.aida", "gzip");
   int r2 = readAndShow("exaTreeCopyBzip.aida", "bzip");
   int r3 = readAndShow("exaTreeCopyUC.aida", "uncompress");
   std::cout << "\nresults from reading files: " << r1 << " " << r2 << " " << r3 << std::endl;
   if (r1+r2+r3 == 0) std::cout << "All OK."<< std::endl;
   std::cout << "\n================================================================================\n" << std::endl;

   std::cout << "That's it !" << std::endl;

}

