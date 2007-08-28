/* test conversion of an AIDA function from AIDA to ROOT */ 

# include "FML/GaussModelFunction.h"
# include "FML/ExponentialModelFunction.h"
# include "FML/SumOfModelFunction.h"

# include "FML/DataVector.h"
# include "FML/Fitter.h"
# include "FML/FitResult.h"
# include "FML/FitConfig.h"

#include "AIDA_Proxy/AIDA_Proxy.h"
#include "CLHEP/Random/RandPoisson.h"

#include "SealUtil/SealTimer.h"
//#include "SealUtil/SealChrono.h"


#include <fstream>
#include <iostream>

/**
 *  
 *   Simple example of fitting.
 */


void fit(pi_aida::Histogram1D * h1, pi_aida::Function * f, const std::string & engine) 
{ 
  // to measure the time 
  seal::SealTimer timer(std::string(engine+" time"),true,std::cout); 

 
  /// create fitter with engine
  pi_aida::Fitter fitter("Chi2",engine); 

   /// perform fit
  AIDA::IFitResult* fitResult = fitter.fit(*h1,*f); 

  if( fitResult )
    {
      std::cout << "fitting successful!\n";
      std::cout << "Fit result : chi2 / ndf : " << fitResult->quality() << " / " << fitResult->ndf() << std::endl;
      const std::vector<std::string>& parNames = fitResult->fittedParameterNames();
      const std::vector<double>& par = fitResult->fittedParameters();
      const std::vector<double>& epar = fitResult->errors();

      for ( unsigned int i = 0; i < par.size(); ++i ) {
	std::cout << parNames[i] << " = " << par[i] << " +/- " << epar[i] << std::endl;
      }
    }
  else
    std::cout << "fitting failed!\n";

  return; 
}



int main( int /* argc */ , char** /*argv */ )
{
  /// define model function - take a predefined Gaussian
  //  give mean, sigma, amp 
  pi_aida::Function f("G+E"); 

  std::vector<double> par(5); 
  par[0] = 115; 
  par[1] = 5; 
  par[2] = 200; 
  par[3] = 7;
  par[4] = -0.01; 
  f.setParameters(par);


  int nBins = 10000; 
  double xmin = 0; 
  double xmax = 200; 
  double dx = (xmax-xmin)/static_cast<double>(nBins); 
  std::vector< std::vector< double> > x(nBins); 
  std::vector<double> y(nBins); 
  std::vector<double> ey(nBins); 

  pi_aida::Histogram1D h1("h1","Input data",nBins,xmin,xmax);
  for (int i = 0; i < nBins; ++i) { 
    x[i].push_back( xmin + dx* ( static_cast<double>(i) + 0.5 ) );
    double mu = f.value(x[i]); 
    // generate Poisson
    y[i] = RandPoisson::shoot(mu);
    ey[i] = sqrt(y[i]); 
    h1.fill(x[i].front() ,y[i]); 
  }



  /// define data vector...
  /* 
  iAIDA::FML::DataVector  v;
  v.reserve(nBins); 

  for (int i = 0; i < nBins; ++i)  
    v.push_back(x[i],y[i],ey[i],ey[i] ); 
 
  std::cout << "Number of data points " << v.nPoints() << std::endl; 

  */


  // init paramters
  std::vector<double> iniPar = par; 
  iniPar[0] = 100; iniPar[1] = 4; iniPar[2] = 100; 
  iniPar[3] = 5; iniPar[4] = -0.007;

  f.setParameters(iniPar); 
  // fit old minuit
  //fit(&h1,&f,"minuit");
  // reset init values 
  f.setParameters(iniPar); 
  // fit with LCG_Minuit
  fit(&h1,&f,"lcg_minuit");

  // save histo
  pi_aida::Proxy_Store store("testFit.aida","XML",pi_aida::RECREATE);
  store.write(h1);
  // save functions
  pi_aida::Function func0("f0","G+E");
  pi_aida::Function func1("f1","G+E");
  func0.setParameters(par);
  func1.setParameters(f.parameters() ); 
  store.write(func0); 
  store.write(func1);
  store.close(); 

  return 0;
 
}
