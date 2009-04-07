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
