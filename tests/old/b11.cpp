#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  // Creating a tree in aida format (compress XML files) 
  std::auto_ptr<AIDA::ITree> tree( tf->create("dps.aida","xml",false,true) );
  // Creating a tree in uncompress XML
  //std::auto_ptr<AIDA::ITree> tree( tf->create("dps.aida","xml",false,true,"uncompress") );

  // Creating a data point set  factory, which will be handled by the tree
  std::auto_ptr<AIDA::IDataPointSetFactory> dpsf( af->createDataPointSetFactory( *tree ) );

  // Creating a 2 D data point set (x-y) 
  AIDA::IDataPointSet & dpsa = *( dpsf->create( "my_dps", "Example of an x-y data point set", 2) );

  // create some vector of points to fill the data point set 
  std::vector<double> x,y,ex,ey;
  std::srand(0); 

  int nPoints = 10; 
  for (int i = 0; i < nPoints; ++i) { 
    x.push_back(i); 
    y.push_back( 10 * static_cast<double>( std::rand() ) / RAND_MAX); 
    ex.push_back(0.5); 
    ey.push_back(1); 
  }
  
  // Filling the data point set with the x-y vector and errors 

  for ( int i = 0; i < nPoints; ++i ) {
    dpsa.addPoint();
    AIDA::IDataPoint & point = *(dpsa.point( i ));
    AIDA::IMeasurement& mX = *(point.coordinate( 0 ) );
    mX.setValue( x[i] );
    // set errors if different from zero
    mX.setErrorPlus( ex[i] );
    mX.setErrorMinus( ex[i] );
    
    AIDA::IMeasurement& mY = *(point.coordinate( 1 ) );
    mY.setValue( y[i] );
    mY.setErrorPlus( ey[i] );
    mY.setErrorMinus( ey[i] );
  }


  std::cout << "DataPointSet :       " << dpsa.title() << std::endl; 
  std::cout << "Number of points :   " << dpsa.size() << std::endl;
  std::cout << "Dimension :          " << dpsa.dimension() << std::endl;    


  
  // plotting...
  // Creating the plotter factory
  /*
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );

  // Creating a plotter
  std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
  plotter->show();

 // plotting the data point set 

  plotter->currentRegion().plot( dpsa, "annotation" );
  plotter->refresh();
 
  std::cout << "Press <ENTER> to continue" << std::endl;
  std::cin.get();

  */

  // Flushing the histograms into the file
  tree->commit();


  // Explicitly closing the tree
  tree->close();


  
  return 0;
}
