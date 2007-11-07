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

# ifndef FMLLIB_PolynomialModelFunction_h__included__
# define FMLLIB_PolynomialModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"


namespace fml {
// namespace FML {

/**
 * Polynomial function of order n.
 *
 * Polynomial of order n has n+1 parameters. Names of parameters:
 * "a0","a1",..."an"
 */

class PolynomialModelFunction : public Function
{
 public:

  /**
   * Construct polynomial of order 0 (1 parameter).
   * Set default parameter values (1.0).
   */
  PolynomialModelFunction();

  /**
   * Construct polynomial of order n (n+1 parameters).
   * Set default parameter values (1.0).
   */
  PolynomialModelFunction(int n);

  /**
   * Construct polynomial based on parameter vector.
   * Parameter vector must not be empty.
   * Degree of polynomial is a number of parameters - 1.
   */
  PolynomialModelFunction(const std::vector<double>& params);

  /** Degree of polynomial.
   * Equal to numberOfParameters()-1.
   */
  int degree() const { return numberOfParameters()-1; }

  double value(const std::vector<double>& x) const;
  const std::vector<double>& gradient(const std::vector<double>& x) const;
  const std::vector<double>& parameterGradient(const std::vector<double>& x) const ;

  /// runtime metatype identifier: "Pn".
  static std::string poly_typeid;  // P
  static std::string poly_pattern; // "@typeid@$iDegree$"

protected:

  bool _constructThis(std::string functionName, const FunctionCatalogue* cat = 0);
 private:

  void constructor_init(int npar);

};

// }
}

# endif
