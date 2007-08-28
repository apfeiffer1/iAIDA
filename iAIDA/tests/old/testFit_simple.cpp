/* test conversion of an AIDA function from AIDA to ROOT */ 

#include "AIDA_Proxy/AIDA_Proxy.h"
#include "CLHEP/Random/RandFlat.h"

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
  pi_aida::Function f("P0"); 

  /*
  std::vector<double> par(1); 
  par[0] = 1; 
  f.setParameters(par);
  */

  int nBins = 500000; 
  double xmin = 0; 
  double xmax = 200; 

  pi_aida::Histogram1D h1("h1","Input data",nBins,xmin,xmax);
  for (int i = 0; i < 100*nBins; ++i) { 
    // generate Poisson
    double y = RandFlat::shoot();
    h1.fill(y); 
  }



  // init paramters
  std::vector<double> iniPar(1); 
  iniPar[0] = 1; 

  //f.setParameters(iniPar); 
  // fit old minuit
  //fit(&h1,&f,"minuit");
  // reset init values 
  //f.setParameters(iniPar); 
  // fit with LCG_Minuit
  fit(&h1,&f,"lcg_minuit");

  // save histo
  /* 
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
  */

  return 0;
 
}
