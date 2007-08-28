# ifndef FML_GaussModelFunction_h__included__
# define FML_GaussModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

/**
 * \file Implementation for AIDA function component
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

# include "FML/Function.h"

# include <math.h>

namespace fml
{
  // namespace FML
    // {

      /**
       * Gauss model function.
       *
       * Three parameters: "mean"  [index=0], 
       *                   "sigma" [index=1], 
       *                   "amp"   [index=2].
       *
       */

      class GaussModelFunction : public Function
      {
      public:
	
	GaussModelFunction();
	
	GaussModelFunction(double mean, double sigma, double amp);

	double value(const std::vector<double>& x) const;
	const std::vector<double>& gradient(const std::vector<double>& x)  const;  
	const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
 	

	double mean() const;
	double sigma() const;
	double amp() const; 
	
	static const int MEAN_IDX = 0;
	static const int SIGMA_IDX = 1;
	static const int AMP_IDX = 2;

	/// runtime metatype identifier: "G".
	static std::string functionTypename();

      private:

	void constructor_init();
      };
   
      inline double GaussModelFunction::mean() const 
      { return parameters()[MEAN_IDX];  }

      inline double GaussModelFunction::sigma() const 
      { return parameters()[SIGMA_IDX]; }

      inline double GaussModelFunction::amp() const
      { return parameters()[AMP_IDX]; }

  // }
}
  
# endif
