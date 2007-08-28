# include "FML/FitConfig.h"

# include "FML/FitPlugin.h"
# include "FML/MinimizerPlugin.h"

# include "FML/DataVector.h"

namespace fml
{
  // namespace FML
  // {

   FitConfig::FitConfig() 
   {
     // defaults  configurations
     m_method_name = "chi2";
     m_engine_name = "minuit2";
     m_errorUP = 1.0; 
     m_useMinos = false; 
     m_printLevel = 0; 
   }

   bool FitConfig::setEngine(std::string name)
   {
     
     // backward compatibility
     if (name == "lcg_minuit") name = "minuit2";

     if(!MinimizerRegistry::global_registry().query(name))
       return false;
     else
       {
	 m_engine_name = name;
	 return true;
       }
   }

   std::string FitConfig::engineName() const
   {
     return m_engine_name;
   }

   const FitParameterSettings & FitConfig::fitParameterSettings(std::string name) const
   {
     return const_cast<FitConfig*>(this)->fitParameterSettings(name);
   }

   FitParameterSettings & FitConfig::fitParameterSettings(std::string name)
   {
     FitParMap::iterator pos = m_fitpar.find(name);

     if( pos == m_fitpar.end())
       {
 	 m_fitpar_names.push_back(name);

	 pos = m_fitpar.insert(make_pair(name,FitParameterSettings(name))).first;

	 return pos->second;
       }
     else
       {
	 return pos->second;
       }
   }

   const std::vector<std::string>  & FitConfig::listParameterSettings() const
   {
     return m_fitpar_names;
   }

   void FitConfig::resetParameterSettings()
   {
     m_fitpar.clear();
     m_fitpar_names.clear();
   }

    void FitConfig::setConstraints(const DomainConstraints& c)
    {
      m_constr = c;
    }

    DomainConstraints& FitConfig::constraints()
    {
      return m_constr;
    }

    const DomainConstraints& FitConfig::constraints() const
    {
      return m_constr;
    }

    void FitConfig::resetConstraints()
    {
      m_constr = DomainConstraints();
    }


   bool FitConfig::setFitMethod(std::string name)
   {
     if(!FitMethodRegistry::global_registry().query(name)) return false;
     m_method_name = name;
     return true;
   }

   std::string FitConfig::fitMethodName() const
   {
     return m_method_name;
   }

    std::auto_ptr<Minimizer> FitConfig::createMinimizer() const
    {
      MinimizerPlugin *minimizer_plugin = 0;

      // handle gracefully default name
      if(engineName() == "")
	{
	std::vector<MinimizerRegistry::plugin_t*> v;

	v = MinimizerRegistry::global_registry().get_vector();

	if(v.size()>0)
	  minimizer_plugin = v[0];
	else
	  {
	    INFO_MSG("FML::fitter: unable to find default minimizer engine");
	  }
	}
      else
	minimizer_plugin = MinimizerRegistry::global_registry().query(engineName());

      if(!minimizer_plugin) 
	{
	  INFO_MSG(std::string("FML::fitter: unregistered minimizer engine: ") + engineName());
	  return std::auto_ptr<Minimizer>();
	}

      std::auto_ptr<Minimizer> minimizer_ptr(minimizer_plugin->create());
      
      if(!minimizer_ptr.get())
	{
	  INFO_MSG("FML::fitter: new allocation failure for: Minimizer");
	}


      return minimizer_ptr;
    }

    std::auto_ptr<IFMLFunction> FitConfig::createFitFunction(IFMLFunction &model, IFMLDataIterator& it) const
    {

      FitMethodPlugin *method_plugin = FitMethodRegistry::global_registry().query(fitMethodName());

      if(!method_plugin) 
	{
	  INFO_MSG(std::string("FML::fitter: unregistered fit method: ") + fitMethodName());
	  return std::auto_ptr<IFMLFunction>();
	}

      std::auto_ptr<FitMethodFunction> method_ptr(method_plugin->create());
      
      if(!method_ptr.get())
	{
	  INFO_MSG(std::string("FML::fitter: new allocation failure for: ") + fitMethodName());
	  return std::auto_ptr<IFMLFunction>();
	}
      
      method_ptr->connectModel(it, model);

      return std::auto_ptr<IFMLFunction>(method_ptr.release());

    }


    void FitConfig::trim(const std::vector<std::string>& /* parameter_names */)
    {
      // FIXME: to be implemented
    }

    void FitConfig::setPrintLevel(int level)
    { 
      m_printLevel = level; 
    }

    void FitConfig::switchMinosErrors(bool flag)
    { 
      m_useMinos = flag;  
    }

    void FitConfig::setErrorUP(double eUP) 
    { 
      m_errorUP = eUP;
    }

    int FitConfig::printLevel() const
    {
      return m_printLevel; 
    }
    bool FitConfig::useMinosErrors() const
    { 
      return m_useMinos;
    }
    double FitConfig::errorUP() const
    { 
      return m_errorUP; 
    }

    MinimizerControlParams & FitConfig::minimizerParams( )  
    {
      return m_minimizer_par; 
    }

    const MinimizerControlParams & FitConfig::minimizerParams( ) const 
    {
      return m_minimizer_par; 
    }

    void FitConfig::setMinimizerParams(const MinimizerControlParams & mcp) { 
      m_minimizer_par = mcp; 
    }


  // }
}
