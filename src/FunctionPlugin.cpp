# include "FML/FunctionPlugin.h"

# include "FML/FunctionNameMatcher.h"

namespace fml
{

// namespace FML
// {

    /// return plugin id string corresponding to the nameId (possibly matched
    /// and transformed), return "" if no name match
    std::string matchAndQueryFunctionPluginLocator(std::string nameId, const FunctionPluginLoader::Locator& locator )
    {
      FunctionNameMatcher matcher;

      std::vector<std::string> vn = locator.getIDVector();

      for(int unsigned i=0; i<vn.size(); ++i)
	{
	  if(vn[i] == nameId) return vn[i];

	  std::string opt = locator.getOPT(vn[i]);
	  if(!opt.empty())
	    {
	      matcher.setPattern(opt);
	      matcher.setParameter("typeid",vn[i]);
	      if(matcher.match(nameId))
		return vn[i];
	    }
	}

      return std::string();
    }

    FunctionPlugin * matchAndQueryFunctionRegistry(std::string nameId)
    {
      
      FunctionNameMatcher matcher;

      std::vector<FunctionPlugin*> v;

      v = FunctionRegistry::global_registry().get_vector();
      
      for(unsigned int i=0; i<v.size(); ++i)
	{
	  assert(v[i]);

	  if(v[i]->id() == nameId) return v[i];

	  if(!v[i]->opt().empty())
	    {
	      matcher.setPattern(v[i]->opt());
	      matcher.setParameter("typeid",v[i]->id());
	      if(matcher.match(nameId))
		return v[i];
	    }
	}

      return 0;
    }
// }
}
