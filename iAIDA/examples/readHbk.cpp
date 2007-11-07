#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <typeinfo>
#include <boost/shared_ptr.hpp>

class ExaH1 {

public:
  ExaH1 ();
  ~ExaH1 ();

  void doIt(bool verbose=false) { read(); check(verbose); }

  // possibly these could be protected
  void read();
  void check(bool verbose=false);

private:
  // the factories and the tree
  AIDA::IAnalysisFactory *af;
  AIDA::ITree *tree;

  // the histograms
  typedef AIDA::IHistogram1D* H1P;
  typedef AIDA::IProfile1D* P1P;
  H1P h1p;
  P1P p1p;
};

ExaH1::ExaH1() {
  af = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory *tf( af->createTreeFactory() );
  bool readOnly = true;
  bool createNew = false;
  tree = tf->create("example.hbk","HBOOK", readOnly, createNew);
  if (!tree) { 
    std::cerr << "ERROR: cannot create hbook tree" << std::endl;
  }
}

ExaH1::~ExaH1() {
  tree->commit();
  tree->close();
  delete tree;  // this also deletes the histograms from memory
  delete af;
}

void ExaH1::read() {

  tree->ls();



  AIDA::IManagedObject * o;

  h1p = dynamic_cast< AIDA::IHistogram1D * >( tree->find("10") );
  if ( h1p == 0 ) {
    std::cerr << "ERROR reading histo   10 from file" << std::endl;
  }

  o = tree->find("20");
  if ( o == 0 ) {
    std::cerr << "ERROR reading object 20 from file" << std::endl;
  }
  p1p = dynamic_cast< AIDA::IProfile1D   * >( o );
  if ( p1p ==0 ) {
    std::cerr << "ERROR reading profile 20 from file" << std::endl;
  }

}

void ExaH1::check(bool verbose) {
  {
    // Printing some statistical values of the profile
    std::cout << "Title  : " << h1p->title() << std::endl;
    std::cout << "Entries: " << h1p->entries() << std::endl;
    std::cout << "Mean   : " << h1p->mean() << std::endl;
    std::cout << "RMS    : " << h1p->rms() << std::endl;

    if (verbose) {
       // Printing the contents of the histogram
       std::cout << "X value     entries    Y value (height)" << std::endl;
       const AIDA::IAxis& xAxis = h1p->axis();
       for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
         std::cout << h1p->binMean( iBin )
           	<< "       "   << h1p->binEntries( iBin )
           	<< "         " << h1p->binHeight( iBin )
           	<< std::endl;
       }
    }
  }
  std::cout << std::endl;
  {

    // Printing some statistical values of the profile
    std::cout << "Title  : " << p1p->title() << std::endl;
    std::cout << "Entries: " << p1p->entries() << std::endl;
    std::cout << "Mean   : " << p1p->mean() << std::endl;
    std::cout << "RMS    : " << p1p->rms() << std::endl;
    
    if (verbose) {
      // Printing the contents of the histogram
      std::cout << "X value     entries    Y value    Y rms" << std::endl;
      const AIDA::IAxis& xAxis = p1p->axis();
      for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
	std::cout << p1p->binMean( iBin )
		  << "       " << p1p->binEntries( iBin )
		  << "         " << p1p->binHeight( iBin )
		  << "         " << p1p->binRms( iBin )
		  << std::endl;
      }
    }
  }

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1p->annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
    std::string key = annotation.key( annotationIndex );
    std::cout << annotation.key( annotationIndex ) << " : "
	      << annotation.value( annotationIndex ) << std::endl;
  }
}

int main( int argc, char** )
{

  std::cout << "\n--------------------------------------------------------------------------------\n" << std::endl;
  std::cout << "readHbk> starting" << std::endl;

  ExaH1 exH1;
  if (argc > 1) {
    exH1.doIt();
  } else {
    exH1.doIt(true);
  }
  std::cout << "readHbk> That's it !" << std::endl;
  std::cout << "\n--------------------------------------------------------------------------------\n" << std::endl;
  return 0;
}
