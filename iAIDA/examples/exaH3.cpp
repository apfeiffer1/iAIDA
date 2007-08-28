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
  typedef boost::shared_ptr<AIDA::IHistogram3D> H3P;
  H3P h3p(factory->createHistogram3D("Example histogram.", 5, 0, 50, 5, 0, 50, 5, 0, 50 ) );

  AIDA::IHistogram3D & h3 = *h3p;

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 20000; ++i ) {
    double x = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    double y = 10 + 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    double z = 20 + 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    h3.fill( x, y, z, 0.5 );
  }

  {
    // Printing some statistical values of the histogram
    std::cout << "MeanX : " << h3.meanX() << std::endl;
    std::cout << "RMSX  : " << h3.rmsX() << std::endl;
    std::cout << "MeanY : " << h3.meanY() << std::endl;
    std::cout << "RMSY  : " << h3.rmsY() << std::endl;
    std::cout << "MeanZ : " << h3.meanZ() << std::endl;
    std::cout << "RMSZ  : " << h3.rmsZ() << std::endl;
    
    // Printing the contents of the histogram
    std::cout << "X value     entries    Y value (height)" << std::endl;
    const AIDA::IAxis& xAxis = h3.xAxis();
    const AIDA::IAxis& yAxis = h3.yAxis();
    const AIDA::IAxis& zAxis = h3.zAxis();
    for ( int iBinX = 0; iBinX < xAxis.bins(); ++iBinX ) {
      for ( int iBinY = 0; iBinY < yAxis.bins(); ++iBinY ) {
	for ( int iBinZ = 0; iBinZ < zAxis.bins(); ++iBinZ ) {
	  std::cout << h3.binMeanX( iBinX, iBinY, iBinZ )
		    << "       " << h3.binMeanY( iBinX, iBinY, iBinZ )
		    << "       " << h3.binEntries( iBinX, iBinY, iBinZ )
		    << "       " << h3.binHeight( iBinX, iBinY, iBinZ )
		    << std::endl;
	}
      }
    }
  }

  std::cout << std::endl;

  

  return 0;
}
