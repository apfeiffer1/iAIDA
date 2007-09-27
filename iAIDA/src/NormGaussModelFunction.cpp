# include "FML/NormGaussModelFunction.h"

#include <iostream>
#include <cmath>

#include "FML/RangeSet.h"

//# include "FML/CodeletStringParser.h" // codelet setting & parsing probably can be automatized in a base class

#include <assert.h>


#include <math.h>

#include <string>
#include <float.h>

#ifdef HAVE_GSL 
#include "FML/gsl/gsl_sf_erf.h"
#endif

// CHLEP util to calculate define integral 
//#include "CLHEP/GenericFunctions/Gaussian.hh"
//#include "CLHEP/GenericFunctions/DefiniteIntegral.hh"
//#include "CLHEP/GenericFunctions/Parameter.hh"


#define PI 3.14159265359

# include "FML/Util/Plugin.h"

#define DIM 1
#define NPAR 2
#define PROVIDE_GRAD false
#define PROVIDE_PARGRAD false
#define PROVIDE_NORM true
#define TYPE_NAME "GN"    

/// constructor :   calls FML Function constructor 

namespace fml 
{
  // namespace FML
  // {


NormGaussModelFunction::NormGaussModelFunction() : 
  Function(DIM, NPAR, PROVIDE_GRAD, PROVIDE_PARGRAD,  PROVIDE_NORM, NormGaussModelFunction::functionTypename() )
{
  std::vector<double> p(NPAR);
  
  p[MEAN_IDX] = 0.0;
  p[SIGMA_IDX] = 1.0;
  
  Function::setParameters(p);
  normalize(true);

  constructor_init();
}


NormGaussModelFunction::NormGaussModelFunction(double mean, double sigma) : 
  Function(DIM, NPAR, PROVIDE_GRAD, PROVIDE_PARGRAD,  PROVIDE_NORM, NormGaussModelFunction::functionTypename() )
{
  std::vector<double> p(NPAR);
  
  p[MEAN_IDX] = mean;
  p[SIGMA_IDX] = sigma;
  
  Function::setParameters(p);
  normalize(true);

  constructor_init();
}


std::string NormGaussModelFunction::functionTypename()
{
  return std::string(TYPE_NAME);
}


void NormGaussModelFunction::constructor_init()
{
  setParName(MEAN_IDX,"mean");
  setParName(SIGMA_IDX,"sigma");
  m_first = true; 
  
}



double NormGaussModelFunction::value(const std::vector<double>& x) const
{
  double y = (x[0]-mean())/sigma();
  /// area is for unit amplitudes 
  double val = 0; 
  if (area() < FLT_MIN ) 
    val =  FLT_MAX; 
  else 
    val =  exp(-0.5*y*y)/area();

  //cout << " area = " << area() << " x = " << x[0] << " value = " << val << endl; 
  return val; 
}


/// return area excluding the amplitude 
double NormGaussModelFunction::area()  const 
{ 

  RangeSet & rangeSet = *normalizationRange(0); 
  const std::vector<double> & minX = rangeSet.lowerBounds(); 
  const std::vector<double> & maxX = rangeSet.upperBounds();  

  /* debug 
  if (m_first) { 
    cout << " Area::ranges " << minX.size() << "  " 
	 << minX[0] << " <   < " << maxX[0] << endl; 
 
    cout << " Parameters - mean = " << mean() 
         << "    sigma = " << sigma() << endl; 
    m_first = false; 
  }
  */
 

  double sig = sigma(); 
  if ( sig < 0) sig = -sigma();

  assert (minX.size() == maxX.size() ); 
  int nRanges = minX.size();
  // calculate Gaussian area
  // case a range different -inf + inf is defined..
  if (nRanges > 1 || maxX[0] != rangeSet.PLUS_INF() || minX[0] != rangeSet.MINUS_INF() ) { 


    // calculate reduced variable 
    double totArea = 0; 
    for (int i = 0; i < nRanges; ++i)  
      totArea += integral(minX[i],maxX[i]);

    return totArea; 
  } 
  else 
    return  sig*std::sqrt(2.0*PI); 
}



// calculate erfc = 2/std::sqrt(pi)*int(e-t2)dt

double NormGaussModelFunction::integral( double x1, double x2) const { 
  // calculate reduced variables 
  double t1 = ( x1 - mean() ) / sigma()  / std::sqrt(2.0);
  double t2 = ( x2 - mean() ) / sigma() / std::sqrt(2.0);

  double a = sigma() * std::sqrt( PI/2.0); 

  return a * (erfc(t1)  - erfc(t2) ); 
    
}

double NormGaussModelFunction::erfc ( double x) const { 

#ifdef HAVE_GSL 
  return gsl_sf_erfc( x); 
#else 

  // calculate here following algorithm in HippoDraw
  // Compute the complementary error function erfc(x).
  // Erfc(x) = (2/std::sqrt(pi)) Integral(exp(-t^2))dt between x and infinity
  //
  //--- Nve 14-nov-1998 UU-SAP Utrecht
  
  // The parameters of the Chebyshev fit
  const double 
    a1 = -1.26551223,   a2 = 1.00002368,
    a3 =  0.37409196,   a4 = 0.09678418,
    a5 = -0.18628806,   a6 = 0.27886807,
    a7 = -1.13520398,   a8 = 1.48851587,
    a9 = -0.82215223,  a10 = 0.17087277;

   double v = 1; // The return value

   double z = x>=0 ? x : -x; //absolute value

   if (z <= 0) return v; // erfc(0)=1

   double t = 1/(1+0.5*z);

   v = t*exp((-z*z) +a1+t*(a2+t*(a3+t*(a4+t*(a5+t*(a6+t*(a7+t*(a8+t*(a9+t*a10)))))))));

   if (x < 0) v = 2-v; // erfc(-x)=2-erfc(x)

   return v;

#endif
}


// implement this if you want to provide it (optional) 
/*
const std::vector<double>& NormGaussModelFunction::gradient(const std::vector<double>& x) const 
{
}
*/
// implement this if you want to provide it (optional)
/*
const std::vector<double>& NormGaussModelFunction::parameterGradient(const std::vector<double>& x) const 
{
}
*/

static Util::Plugin<IFMLFunction,NormGaussModelFunction> plugin(NormGaussModelFunction::functionTypename()); 


// }
}
