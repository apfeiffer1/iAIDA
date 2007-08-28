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
