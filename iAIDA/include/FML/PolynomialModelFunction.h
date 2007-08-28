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
