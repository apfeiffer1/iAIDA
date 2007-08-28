# ifndef FML_MinimizerResult_h__included__
# define FML_MinimizerResult_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/ResultVariable.h"
# include "FML/Contour.h"

//#include "FML/CLHEP/Matrix/Matrix.h"
#include "FML/FMLMatrix.h"


namespace fml
{

// namespace FML {

/**
 * Solution, associated errors and other stats for all variables.
 *
 */

class MinimizerResult
{ 
 public:

  /// Construct an empty (invalid) result object.

  MinimizerResult();

  /// Construct a valid result object.

  MinimizerResult(int ndim, std::vector<ResultVariable> res, double up, double value, const FMLMatrix & cov, bool minos);

  /// Destructor.
  ~MinimizerResult();

  /// Return true for non-empty object.
  /// If false then methods that query the results return meaningless values.
  bool isValid() const { return m_isvalid; }

  /// Dimensionality of the domain space.
  int dimension() const;

  /// Vector of result variables.
  const std::vector<ResultVariable>& variables() const;

  /// Value of objective function.
  double minimumValue() const { return m_minvalue; }

  /// Value of UP parameter that was used to evaluate statistics.
  double errorUP() const;

  /// True if minos available.
  bool minosAvailable() const { return m_minos; }

  /**
   * Covariance matrix for all, non-fixed parameters.
   * Shall it be for *ALL* parameters, unlike Minuit?
   */
  const FMLMatrix& covMatrix() const;

  /**
   * Eliptical contour for n,m variables.
   * Precondition: 0<= n,m < dim().
   * PENDING: efficiency may be improved
   * Right now pointer to common vector buffer, in the future
   * maybe spearate buffers for each contour (speedup if re-requested).
   */
  /*const IVector**/Contour ellipticalContour(int n, int m) const;

  /**
   * Minos contour.
   * If !minosAvailabe(), then return 0.
   * PENDING: same comments as above
   */
  /*const IVector**/ Contour minosContour(int n, int m) const;

 private:

  int m_dim;
  std::vector<ResultVariable> m_variables;
  double m_errorup;
  FMLMatrix m_covmatrix;
  double m_minvalue;

  bool m_isvalid;

  bool m_minos;

};

// }
}

# endif
