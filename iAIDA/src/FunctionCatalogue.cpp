# include "FML/FunctionCatalogue.h"

# include "FML/FunctionSetterHelper.h"

# include "FML/CodeletStringParser.h" // also for tokenize()
# include "FML/FunctionExpressionParser.h" // for E+G and similar expressions

# include "FML/Util/PluginTools.h"

# include <fstream>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif


namespace fml
{
  // namespace FML
  // {

    std::auto_ptr<IFMLFunction> FunctionCatalogue::createFunction(std::string nameId )
    {
      // parse expression
      std::auto_ptr<IFMLFunction> fexpr(createExpressionFunction(nameId ) );
      
      if(fexpr.get())
	return fexpr;
      
      // plain function identifier

      std::auto_ptr<IFMLFunction> fnull; // null ptr at the beginning

      FunctionPlugin *p = matchAndQueryFunctionRegistry(nameId);
      
      if(!p) // plugin not registered
	{ 
	    return fnull; 
	}	      
      
      assert(p);

      // need to do this to avois crach on amd 64 (why ???)
      Util::to_string(1); 

      // create function instance from plugin
      std::auto_ptr<IFMLFunction> ff(p->create());

      if(!ff.get()) return fnull;

      FunctionSetterHelper fsh(ff.get());

      if(!fsh._constructThis(nameId) ) return fnull;

      return ff;
    }
    
    std::vector<std::string> FunctionCatalogue::listAvailableFunctions() 
    {
      std::ostringstream buf;
      Util::list_plugin<FunctionRegistry>(buf);
#ifndef BADENDS
      buf << std::ends;
#endif
      std::string plug_list = buf.str();

      std::vector<std::string> list_buf = tokenize(plug_list,"\n");

      return list_buf;
    }
    
  // }
}
