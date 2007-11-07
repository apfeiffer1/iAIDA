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

# ifndef MINFIT_MinimizerControlParams_h__included__
# define MINFIT_MinimizerControlParams_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

namespace fml
{

// namespace FML {

/**
 * Minimizer execution control parameters.
 *
 * These parameters control the execution of minimization algorithm.
 *      - maximum number of iterations
 *      - tolerance
 *      - precision
 *      - error UP parameter
 *
 * Unless explictly set, parameters are "implicit", i.e. "I do not know
 * the value and I use whatever minimizer provides".
 * An implicit parameter will not be set in the minimization engine and 
 * the engine internal default will be used! 
 *
 * \ingroup FML_LIB
 */

class MinimizerControlParams
{
 public:

  /// All parameters are implicit.  
  MinimizerControlParams();

  /// @name Getting values of parameters.
                                                         //@{
  /// Maximum number of iterations.
  int maxIter() const { return m_maxit; }

  /// Tolerance.
  double tolerance() const { return m_tol; }

  /// Accuracy of computation (EPS).
  double precision() const { return m_prec; }

  /// strategy of minimization
  int strategy() const  { return m_strategy; } 

  /**
   *  "Parameter error" as defined in Minuit.
   * Migrad procedure uses it to drive minimization.
   * Should be 1.0 for Chi2 and 0.5 for negative log likelihood.
   */
  double errorUP() const { return m_up; } 
                                                         //@}

  /// @name Setting parameters explicitly (exact value will be used).
                                                         //@{
  void setMaxIter(int maxit) { m_maxit = maxit; }
  void setTolerance(double tol) { m_tol = tol; }
  void setPrecision(double prec) { m_prec = prec; }
  void setErrorUP(double up) { m_up = up; }
  void setStrategy(int strat) { m_strategy = strat; }
                                                         //@}

  /// @name Telling minimizer not to set a parameter explicitly.
                                                         //@{
  void useImplicitMaxIter() { m_maxit = 1000; }
  void useImplicitTolerance() { m_tol = 0.1; }
  void useImplicitPrecision() { m_prec = -1.0; }
  void useImplicitErrorUP() { m_up = 1.0; }
  void useImplicitStrategy() { m_strategy = 1; }
                                                         //@}

  /// @name Testing if a parameter is implicit.
                                                         //@{

  bool implicitMaxIter() const { return m_maxit == 0; }
  bool implicitTolerance() const { return m_tol <= 0.0; }
  bool implicitPrecision() const { return m_prec <= 0.0; }
  bool implicitErrorUP() const { return m_up <= 0.0; }
                                                         //@}


 private:

  int m_maxit, m_strategy;
  double m_tol, m_prec, m_up;

  /*
public:
  static const int    MAXITER_DEFAULT;
  static const double TOLERANCE_DEFAULT;
  static const double PRECISION_DEFAULT;
  static const double ERRORUP_DEFAULT;
  */
};

// }
}

# endif
