#ifndef IAIDA_FML_NORMGAUSSMODELFUNCTION 
#define IAIDA_FML_NORMGAUSSMODELFUNCTION 1
/**
 *  
 *  Normalized Gauss Function 
 *
 * 
 */

# include "FML/Function.h"

# include <math.h>


namespace fml 
{
  // namespace FML
  // {


    class NormGaussModelFunction : public Function
      {
      public:

	
	
	NormGaussModelFunction();

	NormGaussModelFunction(double mean, double sigma);
	
	// methods need to be implemented by the user 

	double value(const std::vector<double>& x) const;
	//const std::vector<double>& gradient(const std::vector<double>& x)  const;  
	//const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
 	
	/// runtime metatype identifier: "
	static std::string functionTypename();


	// helper methods 

	double mean() const;
	double sigma() const;

	double area() const; 

	static const int MEAN_IDX = 0;
	static const int SIGMA_IDX = 1;

      protected: 
	// calculate integrals from x1 and x2 ( x1 <= x2) 
	double integral( double x1, double x2) const; 

	// erfc 
	double erfc (double x) const;


      private:

	void constructor_init();

	mutable bool m_first; 
      };
   
      inline double NormGaussModelFunction::mean() const 
      { return parameters()[MEAN_IDX];  }

      inline double NormGaussModelFunction::sigma() const 
      { return parameters()[SIGMA_IDX]; }

  // }
}
  
# endif
