# include "FML/Bound.h"

# include <iostream>

namespace fml
{
// namespace FML {
  
std::ostream& operator << (std::ostream& os, const Bound& b)
{
  if(b.isFixed())
    return os << " fixed-at " << b.lowerBound();

  if(b.isBound())
    return os << " lower " << b.lowerBound() << " upper " << b.upperBound();

  return os << " unbound";
}

// }
}
