# include "FML/NonlinearConstraint.h"

namespace fml
{
// namespace FML
// {
  /// Create non-linear constraint with given function.
  NonlinearConstraint::NonlinearConstraint(IFMLFunction &f, const Bound &b) : 
    SpaceConstraint(f.dimension(),b), m_fun(f.clone()) {}

  IFMLFunction &NonlinearConstraint::function() const 
  {
    assert(m_fun.get());
    return *m_fun.get(); 
  }

  NonlinearConstraint::NonlinearConstraint(const NonlinearConstraint& c) : SpaceConstraint(c.dimension(), c.bound()), m_fun(0)
  {
    //    SpaceConstraint::copyStateFrom(c);
    std::auto_ptr<IFMLFunction> nf(c.function().clone());
    m_fun = nf;
  }

  NonlinearConstraint& NonlinearConstraint::operator = (const NonlinearConstraint& c)
  {
    SpaceConstraint::copyStateFrom(c);
    std::auto_ptr<IFMLFunction> nf(c.function().clone());
    m_fun = nf;
    return *this;
  }

  bool NonlinearConstraint::operator == (const NonlinearConstraint& c ) const
  {
    return bound()==c.bound() && function().isEqual(&c.function());
  }
// }
}
