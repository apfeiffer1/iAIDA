# ifndef FMLLIB_ExponentialModelFunction_h__included__
# define FMLLIB_ExponentialModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"

# include <math.h>

# include <assert.h>

namespace fml {
// namespace FML {

/**
 * Exponential model function exp(amp+slope*x) = exp(amp)*exp(slope*x).
 *
 * Order of parameters in the vector: [0]=amp, [1]=slope.
 * Implements gradient and parameterGradient.
 *
 * \ingroup FML_LIB
 */

class ExponentialModelFunction : public Function
{
 public:

  /// f(x) = exp(x), that is: amp = 0.0, slope = 1.0.
  ExponentialModelFunction();

  /// f(x) = exp(amp+slope*x).
  ExponentialModelFunction(double amp, double slope);

  double value(const std::vector<double>& x_vec) const;

  const std::vector<double>& gradient(const std::vector<double>& x_vec) const;

  const std::vector<double>& parameterGradient(const std::vector<double>& x_vec) const;

  /// quick access to parameter: slope
  double slope() const;

  /// quick access to parameter: amp
  double amp() const;

  static const int AMP_IDX = 0;
  static const int SLOPE_IDX = 1;
    
  /// runtime metatype identifier: "E".
  static std::string functionTypename();

 private:
  void constructor_init();

};

// }
}
# endif
