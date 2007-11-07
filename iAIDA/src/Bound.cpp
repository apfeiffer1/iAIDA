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
