# ifndef FML_SpaceConstraint_h__included__
# define FML_SpaceConstraint_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Bound.h"

namespace fml
{

// namespace FML {

/**
 * Range constraint on given n-dimensional space.
 *
 * In  general  range   constraint  is  defined  as  \f$lower<=F(x)<=
 * upper\f$, where x is a  vector in n-dimensional space and F(x) is a
 * constraint  expression.   It   is  up  to  subclasses  to  specify
 * constraint expression.
 *
 * SpaceConstraint  refers to  domain space as  a whole. It uses Bound
 * for bounding the expression. 
 * Note that, Bound present in MinimizationVariable refers to only one 
 * variable of the domain space.
 *          
 * \ingroup FML_LIB
 */

class SpaceConstraint
{
 public:

  /// The bound of the expression.
  Bound& bound() { return m_bound; }
  const Bound& bound() const { return m_bound; }

  /// Dimensionality of the domain space. 
  int dimension() const { return m_dim; }

 protected:

  /// Constructor (accessible only in derived classes).

  SpaceConstraint(int _dim, Bound);
  
  void copyStateFrom(const SpaceConstraint&c) 
  {
    m_bound = c.m_bound;
    m_dim = c.m_dim;
  }

 private:

  Bound m_bound;
  int m_dim;
};

/** 
 * Constructor (accessible only in derived classes).
 * Dimensionality of domain space must be stated explicitly.
 */

inline SpaceConstraint::SpaceConstraint(int dim, Bound b) :
  m_bound(b), m_dim(dim)
{}

// }

}
# endif
