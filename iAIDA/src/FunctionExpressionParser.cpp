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

# include "FML/FunctionExpressionParser.h"

# include "FML/IFMLFunction.h"
# include "FML/FunctionCatalogue.h"
# include "FML/FunctionSetterHelper.h"

# include "FML/SumOfModelFunction.h"

#include "FML/Util/SimpleTokenizer.h"

namespace fml
{
// namespace FML
// {

  std::auto_ptr<IFMLFunction> createExpressionFunction(std::string expr, FunctionCatalogue *)
  {
    SimpleTokenizer tk("+");

    // ignore case

  // PENDING: Solaris error, Linux ok

#ifdef __sun
   std::string::iterator itr = expr.begin();
   while ( itr != expr.end() ) {
     *itr = toupper(*itr);
     itr++;
   }
#else
  transform(expr.begin(), expr.end(), expr.begin(), toupper);
#endif
    
  std::vector<std::string> tokens = tk.tokenize(expr);

  std::auto_ptr<IFMLFunction> fnull; // null ptr at the beginning

  /// if multiple tokens, model function is a sum (with member ownership)
  if(tokens.size()>1) 
    {
      std::auto_ptr<SumOfModelFunction> new_model(new SumOfModelFunction(true));

      if(!new_model.get()) 
	return fnull; // memory allocation failed

      std::vector<std::string>::iterator tok;

      // append all the components to the sum
      for(tok = tokens.begin(); tok!=tokens.end(); ++tok)
	{
	  //std::auto_ptr<IFMLFunction> new_component(FunctionCatalogue::loadAndCreateFunction(*tok,cat));
	  std::auto_ptr<IFMLFunction> new_component(FunctionCatalogue::createFunction(*tok));

	  if(!new_component.get()) return fnull;

	  // new_model takes ownership
	  new_model->append(new_component.release()); 
	}

      // check if function is normalized then append params
      if (new_model->isNormalized())
        new_model->appendAmpParameters();


      // set name of the function to user's provided expression
      FunctionSetterHelper fsh(new_model.get());
      fsh.setFunctionName(expr);

      return std::auto_ptr<IFMLFunction>(new_model.release());
    }
  else
    return fnull; // plain function identifier

  }

// }
}

