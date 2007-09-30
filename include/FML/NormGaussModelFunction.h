//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

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
