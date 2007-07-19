#ifndef FML_FUNCTIONEXPRESSIONPARSER_H
#define FML_FUNCTIONEXPRESSIONPARSER_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <memory>
# include <string>

//# include "FML/IFMLFunction.h"

namespace fml
{
// namespace FML
// {

  class IFMLFunction;
  class FunctionCatalogue;

  /**
   * Create a function from an algebraic expression.
   * If the expression is a simple function (so sub-expressions) return 0.
   */
  std::auto_ptr<IFMLFunction> createExpressionFunction(std::string expr, FunctionCatalogue *cat = 0);

// }
}


#endif
