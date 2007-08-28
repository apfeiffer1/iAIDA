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

int main( int, char** )
{

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> treeF ( af->createTreeFactory() );
  boost::shared_ptr<AIDA::ITree> tree ( treeF->create() );
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  // Creating a histogram
  typedef boost::shared_ptr<AIDA::IHistogram2D> H2P;
  typedef boost::shared_ptr<AIDA::IProfile2D> P2P;
  H2P h2p(factory->createHistogram2D("Example histogram.", 10, 0, 50, 10, 0, 50 ) );
  P2P p2p(factory->createProfile2D("Example profile.", 10, 0, 50, 10, 0, 50 ) );

  AIDA::IHistogram2D & h2 = *h2p;
  AIDA::IProfile2D & p2 = *p2p;

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 20000; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    double y = 10 + 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h2.fill( x, y, 0.5 );
    p2.fill( x,  10., 0.5 );
  }

  {
    // Printing some statistical values of the histogram
    std::cout << "MeanX : " << h2.meanX() << std::endl;
    std::cout << "RMSX  : " << h2.rmsX() << std::endl;
    std::cout << "MeanY : " << h2.meanY() << std::endl;
    std::cout << "RMSY  : " << h2.rmsY() << std::endl;
    
    // Printing the contents of the histogram
    std::cout << "X value     entries    Y value (height)" << std::endl;
    const AIDA::IAxis& xAxis = h2.xAxis();
    const AIDA::IAxis& yAxis = h2.yAxis();
    for ( int iBinX = 0; iBinX < xAxis.bins(); ++iBinX ) {
      for ( int iBinY = 0; iBinY < yAxis.bins(); ++iBinY ) {
	std::cout << h2.binMeanX( iBinX, iBinY )
		  << "       " << h2.binMeanY( iBinX, iBinY )
		  << "       " << h2.binEntries( iBinX, iBinY )
		  << "       " << h2.binHeight( iBinX, iBinY )
		  << std::endl;
      }
    }
  }
  std::cout << std::endl;

  
  {

    // Printing some statistical values of the profile
    std::cout << "MeanX : " << p2.meanX() << std::endl;
    std::cout << "RMSX  : " << p2.rmsX() << std::endl;
    std::cout << "MeanY : " << p2.meanY() << std::endl;
    std::cout << "RMSY  : " << p2.rmsY() << std::endl;
    
    // Printing the contents of the histogram
    std::cout << "X value     entries    Y value    Y rms" << std::endl;
    const AIDA::IAxis& xAxis = p2.xAxis();
    const AIDA::IAxis& yAxis = p2.yAxis();
    for ( int iBinX = 0; iBinX < xAxis.bins(); ++iBinX ) {
      for ( int iBinY = 0; iBinY < yAxis.bins(); ++iBinY ) {
	std::cout << p2.binMeanX( iBinX, iBinY )
		  << "       " << p2.binMeanY( iBinX, iBinY )
		  << "       " << p2.binEntries( iBinX, iBinY )
		  << "       " << p2.binHeight( iBinX, iBinY )
		  << std::endl;
      }
    }

  }
  

  // Printing the annotation values of the histogram
  std::cout << std::endl << "Current annotation items/values : " << std::endl;
  const AIDA::IAnnotation& annotation = h2.annotation();
  for ( int annotationIndex = 0; annotationIndex < annotation.size(); ++annotationIndex ) {
    std::string key = annotation.key( annotationIndex );
    std::cout << annotation.key( annotationIndex ) << " : "
	      << annotation.value( annotationIndex ) << std::endl;
  }

  return 0;
}
