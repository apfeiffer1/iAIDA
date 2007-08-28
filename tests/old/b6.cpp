#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

// Class declarations from CLHEP
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/DRand48Engine.h"

#include <sstream>

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );

  // Creating the plotter factory
  /*
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );

  // Creating a plotter
  std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
  plotter->show();
  */

  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr< AIDA::ITree > tree( tf->create() );

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );

  // Creating a function factory whose functions will be handled by the tree
  std::auto_ptr< AIDA::IFunctionFactory > ff( af->createFunctionFactory( *tree ) );

  // Creating a fit factory
  std::auto_ptr< AIDA::IFitFactory > fitf( af->createFitFactory() );
 
  // Creating a histogram
  AIDA::IHistogram1D& h1 = *( hf->createHistogram1D("MyHisto", "Gaussian Distribution", 100, 0, 100) );

  // Filling the histogram with random data following a gaussian distribution
  DRand48Engine randomEngine;
  RandGauss rgauss( randomEngine, 45, 10 );
  for ( int i = 0; i < 10000; ++i ) {
    h1.fill( rgauss.fire() );
  }

  std::cout << "Histogram :" << std::endl;
  std::cout << "  Title : " << h1.title() << std::endl;
  std::cout << "  Entries : " << h1.entries() << std::endl;
  std::cout << "  Mean : " << h1.mean() << std::endl;
  std::cout << "  Rms : " << h1.rms() << std::endl;

  // Creating the function which is going to be fitted with the histogram data
  AIDA::IFunction& gaussFun = *( ff->createFunctionByName("Gaussian","G") );

  // set function initial parameters
  gaussFun.setParameter("mean" , 50.);
  gaussFun.setParameter("sigma", 10.);
  gaussFun.setParameter("amp"  , 10.);

  // Creating the fitter
  std::auto_ptr<AIDA::IFitter> fitter( fitf->createFitter() );

  // Performing the fit
  AIDA::IFitResult& fitResult = *( fitter->fit( h1, gaussFun ) );

  // Printing the fit results
  std::cout << "Fit result : chi2 / ndf : " << fitResult.quality() << " / " << fitResult.ndf() << std::endl;
  const std::vector<std::string>& parNames = fitResult.fittedParameterNames();
  const std::vector<double>& par = fitResult.fittedParameters();
  const std::vector<double>& epar = fitResult.errors();

  for ( unsigned int i = 0; i < par.size(); ++i ) {
    std::cout << parNames[i] << " = " << par[i] << " +/- " << epar[i] << std::endl;
  }

  // Add in the annotation of the histogram the fit result
  AIDA::IAnnotation& annotation = h1.annotation();
  for ( unsigned int i = 0; i < par.size(); ++i ) {
    std::ostringstream os;
    os << par[i] << " +/- " << epar[i];
    annotation.addItem( parNames[i], os.str() );
  }

  // Remove some existing annotation items
  annotation.removeItem( "Overflow" );
  annotation.removeItem( "Underflow" );
  annotation.removeItem( "Extra Entries" );

  // Plotting the function with its annotation and then the histogram
  //plotter->currentRegion().plot( h1, "annotation" );
  //plotter->currentRegion().plot(gaussFun,"[0,100] overlay");
  //plotter->refresh();

  // Listing the tree contents
  std::cout << "Tree contents :" << std::endl;
  tree->ls();

  // Wait for the keyboard return to avoid destroying the plotter window too quickly.
  //std::cout << "Press <ENTER> to exit" << std::endl;
  //std::cin.get();

  return 0;
}
