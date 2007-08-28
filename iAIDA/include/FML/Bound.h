# ifndef FMLLIB_Bound_h__included__
# define FMLLIB_Bound_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

#include <assert.h>
#include <iosfwd>

namespace fml
{

// namespace FML {

  static const double MINUS_INF = -1e38; // use float max
  static const double PLUS_INF = 1e38;  


/**
 * Bound for a scalar quantity: \f$ x \in [lower,upper] \f$. 
 * 
 * Any scalar quantity \f$ x \f$ may be given one of following types of bounds:
 *
 *  - double-sided bound: \f$ x \in [lower,upper] \land lower<upper \f$
 *     
 *  - one-sided bounds: 
 *       upper bound \f$ x \in [-\infty, upper] \f$ 
 *       or lower bound \f$ x \in [lower, +\infty] \f$
 *
 *  - fixed bound corresponding to fixed variable: \f$ x = lower = upper\f$
 *
 * To create:
 *
 *  - double-sided bound use constructor Bound(lower,upper).
 *  - one-sided bound  call UpperBound(upper) or LowerBound(lower) or 
 *    Bound(MINUS_INF,upper) or Bound(lower,PLUS_INF)
 *  - fixed bound call FixedBound(value) 
 *
 * \ingroup FML_LIB
 */


class Bound
{
 public:


  /// Create empty bound (x is unconstrained/unbound).
  Bound() : m_lower(MINUS_INF), m_upper(PLUS_INF), m_fixed(false) {}

  /// Create double sided bound: [lower,upper] with precondition lower<=upper. 
  Bound(double lower, double upper) : m_lower(lower), m_upper(upper), m_fixed(false)
    {
      assert(lower<=upper);
    }
  
  /// Constructor function for upper bound.
  friend Bound UpperBound(double upper);

  /// Constructor function for lower bound.
  friend Bound LowerBound(double lower);

  /// Constructor function for fixed x.
  friend Bound FixedBound(double value);

  /// Get lower bound.
  double lowerBound() const { return m_lower; }

  /// Get upper bound.
  double upperBound() const { return m_upper; }

  /**
   * Shortcut for !(lowerBound() == MINUS_INF && upperBound() == PLUS_INF).
   */

  bool isBound() const { return !(m_lower == MINUS_INF && m_upper == PLUS_INF); }

  /// Return true if x is fixed.
  bool isFixed() const { return m_fixed; }

  void setFixed(bool on = true) { m_fixed = on; }

  bool hasUpperBound() { return m_upper!=PLUS_INF; }
  bool hasLowerBound() { return m_lower!=MINUS_INF; }
  bool hasDoubleBounds() { return ( hasUpperBound() &&  hasLowerBound() ); }

  /// Compare two bounds.

  bool operator == (const Bound& b) const
  {
    return m_lower == b.m_lower && m_upper == b.m_upper;
  }

 private:
  
  double m_lower, m_upper;
  bool m_fixed; 

  Bound make_upper_only(double upper) 
    { 
      m_upper = upper;
      m_lower = MINUS_INF; 
      m_fixed = false;
      return *this; 
    } 
  Bound make_lower_only(double lower) 
    {
      m_upper = PLUS_INF;
      m_lower = lower;
      m_fixed = false; 
      return *this; 
    }
  Bound make_fixed(double val) 
    {
      m_upper = PLUS_INF;
      m_lower = MINUS_INF; 
      m_fixed = true;
      return *this; 
    } 

};

/// Create one-sided upper bound: x <= upper.
inline Bound UpperBound(double upper) { return Bound().make_upper_only(upper); }

/// Create one-sided lower bound: lower <= x.
inline Bound LowerBound(double lower) { return Bound().make_lower_only(lower); }

/// Create fixed-variable bound: x = value.
inline Bound FixedBound(double value) { return Bound().make_fixed(value); }

std::ostream& operator << (std::ostream& os, const Bound& b);

// }
}
# endif

















