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

namespace AIDA_Helper {
 void dumpInfo(AIDA::IHistogram1D * hist1d, bool verbose) {
  {
    // Printing some statistical values of the profile
    std::cout << "Title  : " << hist1d->title() << std::endl;
    std::cout << "Entries: " << hist1d->entries() << std::endl;
    std::cout << "Mean   : " << hist1d->mean() << std::endl;
    std::cout << "RMS    : " << hist1d->rms() << std::endl;

    if (verbose) {
       // Printing the contents of the histogram
       std::cout << "X value     entries    Y value (height)" << std::endl;
       const AIDA::IAxis& xAxis = hist1d->axis();
       for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
         std::cout << hist1d->binMean( iBin )
           	<< "       "   << hist1d->binEntries( iBin )
           	<< "         " << hist1d->binHeight( iBin )
           	<< std::endl;
       }
    }
  }
 }

 void dumpInfo(AIDA::IProfile1D * prof1d, bool verbose) {
   {
     // Printing some statistical values of the profile
     std::cout << "Title  : " << prof1d->title() << std::endl;
     std::cout << "Entries: " << prof1d->entries() << std::endl;
     std::cout << "Mean   : " << prof1d->mean() << std::endl;
     std::cout << "RMS    : " << prof1d->rms() << std::endl;

     if (verbose) {
       // Printing the contents of the histogram
       std::cout << "X value     entries    Y value    Y rms" << std::endl;
       const AIDA::IAxis& xAxis = prof1d->axis();
       for ( int iBin = 0; iBin < xAxis.bins(); ++iBin ) {
 	std::cout << prof1d->binMean( iBin )
 		  << "       " << prof1d->binEntries( iBin )
 		  << "         " << prof1d->binHeight( iBin )
 		  << "         " << prof1d->binRms( iBin )
 		  << std::endl;
       }
     }
   }
 }
}; // end namespace AIDA_Helper

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
  H1P h2p;
  P1P p1p;
};

ExaH1::ExaH1() : h1p(0), h2p(0), p1p(0) {
  af = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory *tf( af->createTreeFactory() );
  bool readOnly = true;
  bool createNew = false;
  tree = tf->create("example.root","root", readOnly, createNew);
  if (!tree) { 
    std::cerr << "ERROR: cannot create root tree" << std::endl;
  }
}

ExaH1::~ExaH1() {
  tree->commit();
  tree->close();
  delete tree;  // this also deletes the histograms from memory
  delete af;
}

void ExaH1::read() {

   std::cout << "File contains:" << std::endl;
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
  h2p = dynamic_cast< AIDA::IHistogram1D * >( tree->find("20") );
  if ( h2p == 0 ) {
     p1p = dynamic_cast< AIDA::IProfile1D   * >( o );
     if ( p1p ==0 ) {
        std::cerr << "ERROR reading profile/histogram 20 from file" << std::endl;
     }
  }
}


void ExaH1::check(bool verbose) {
   
   std::cout << std::endl;
   if (h1p) AIDA_Helper::dumpInfo(h1p,verbose);
   
   std::cout << std::endl;
   if (h2p) AIDA_Helper::dumpInfo(h2p,verbose);
   
   std::cout << std::endl;
   if (p1p) AIDA_Helper::dumpInfo(p1p,verbose);

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
  std::cout << "readRoot> starting" << std::endl;

  ExaH1 exH1;
  if (argc > 1) {
    exH1.doIt();
  } else {
    exH1.doIt(true);
  }
  std::cout << "readRoot> That's it !" << std::endl;
  std::cout << "\n--------------------------------------------------------------------------------\n" << std::endl;
  return 0;
}
