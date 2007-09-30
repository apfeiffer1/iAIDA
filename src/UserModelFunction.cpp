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

# include "FML/UserModelFunction.h"


# include "FML/Util/Plugin.h"

static const std::string userFunctionName = "UserFunction";

namespace fml 
{
  // namespace FML
  // {

  // needed for plugin manager
  UserModelFunction::UserModelFunction() : 
    Function(1,0,false,false,false,userFunctionName) 
  {}

  /// Capture global function and specify dimensionality.
  UserModelFunction::UserModelFunction(Fun_ptr fptr, int dim, int npar) : 
    Function(dim,npar,false,false,false,userFunctionName), 
    m_fptr(fptr) 
  {}

static Util::Plugin<IFMLFunction,UserModelFunction> plugin(userFunctionName); 

// }
}
