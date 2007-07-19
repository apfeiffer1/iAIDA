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

  void doIt() { book(); fill(); check(); }

  // possibly these could be protected
  void book();
  void fill();
  void check();

private:
  // the factories and the tree
  AIDA::IAnalysisFactory *af;
  AIDA::ITree *tree;
  AIDA::IHistogramFactory *factory;

  // the histograms
  typedef AIDA::IHistogram1D* H1P;
  typedef AIDA::IProfile1D* P1P;
  H1P h1p;
  P1P p1p;
};

ExaH1::ExaH1() {
  af = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory *tf( af->createTreeFactory() );
  tree = tf->create();
  factory = af->createHistogramFactory( *tree );
}

ExaH1::~ExaH1() {
  tree->commit();
  tree->close();
  delete tree;  // this also deletes the histograms from memory
  delete factory;
  delete af;
}

void ExaH1::book() {
  // Creating the histograms
  h1p = factory->createHistogram1D("Example histogram.", 50, 0, 50 );
  if (h1p == 0) {
    std::cerr << "ERROR booking histogram" << std::endl;
  }
  p1p = factory->createProfile1D( "Example profile.", 50, 0, 50 );
  if (h1p == 0) {
    std::cerr << "ERROR booking histogram" << std::endl;
  }
}

void ExaH1::fill() {

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h1p->fill( x, 0.5 );
    p1p->fill( x, -10., 0.5 );
    p1p->fill( x,  10., 0.5 );
  }
}

void ExaH1::check() {

  {
    // Printing some statistical values of the histogram
    std::cout << "Mean : " << h1p->mean() << std::endl;
    std::cout << "RMS  : " << h1p->rms() << std::endl;
    
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
  std::cout << std::endl;
  {
    // Printing some statistical values of the profile
    std::cout << "Mean : " << p1p->mean() << std::endl;
    std::cout << "RMS  : " << p1p->rms() << std::endl;
    
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

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h1p->annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
    std::string key = annotation.key( annotationIndex );
    std::cout << annotation.key( annotationIndex ) << " : "
	      << annotation.value( annotationIndex ) << std::endl;
  }
}

int main( int, char** )
{

  ExaH1 exH1;
  exH1.doIt();
  std::cout << "That's it !" << std::endl;
  return 0;
}
