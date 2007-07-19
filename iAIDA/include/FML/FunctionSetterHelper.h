#ifndef FML_FUNCTIONSETTERHELPER_H
#define FML_FUNCTIONSETTERHELPER_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

/**
 * \file FML Functions
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

namespace fml 
{
  // namespace FML
  // {

    class IFMLFunction;
    class FunctionCatalogue;
    class RangeSet;

    /**
     * Function setter helper allows to modify the internal state of the function object.
     * The usage is restricted to streaming and should be considered internal.
     */
    class FunctionSetterHelper
    {
    public:
      explicit FunctionSetterHelper(IFMLFunction*);
      //      explicit FunctionSetterHelper(ModelFunction*);

      bool setDimension(int dimx);
      bool setNumberOfParameters(int parnum);
      bool setProvidesGradient(bool provides_gradient);

      bool setParName(int ix, std::string name);
      bool setVarName(int ix, std::string name);

      bool setCodeletString(std::string name);
      bool setFunctionName(std::string name);

      /// setters specific to ModelFunction

      bool setProvidesParameterGradient(bool yes);
      bool setNormalization(bool provides, bool is_normalized);

      bool setNormalizationRange(int ix, const RangeSet & rs); 
      
      bool _constructThis(std::string typenameId, const FunctionCatalogue* cat = 0);

      /*
      bool _postConstructThis(std::string codelet_string, const FunctionCatalogue* cat = 0);
      bool _preConstructThis(std::string codelet_string, const FunctionCatalogue* cat = 0);
      */

    private:
      IFMLFunction *m_function;
      //      ModelFunction *m_modelfun;
    };
    //}
}


# include "FML/IFMLFunction.h"

namespace fml
{

//namespace FML
//{

    inline FunctionSetterHelper::FunctionSetterHelper(IFMLFunction* f) : m_function(f)//, m_modelfun(0)
    {}

    //    inline FunctionSetterHelper::FunctionSetterHelper(ModelFunction* mf) : m_function(mf), m_modelfun(mf)
    // {}
    
    inline bool FunctionSetterHelper::setDimension(int dimx)
    {
      if(!m_function) return false;

      return m_function->setDimension(dimx);
    }

    inline bool FunctionSetterHelper::setNumberOfParameters(int parnum)
    {
      if(!m_function) return false;

      return m_function->setNumberOfParameters(parnum);
    }
    

    inline bool FunctionSetterHelper::setProvidesGradient(bool provides_gradient)
    {
      if(!m_function) return false;

      return m_function->setProvidesGradient(provides_gradient);
    }
    
    
    inline bool FunctionSetterHelper::setParName(int ix, std::string name)
    {
      if(!m_function) return false;

      return m_function->setParName(ix,name);
    }
    

    inline bool FunctionSetterHelper::setVarName(int ix, std::string name)
    {
      if(!m_function) return false;

      return m_function->setVarName(ix,name);
    }
    
    
    inline bool FunctionSetterHelper::setCodeletString(std::string name)
    {
      if(!m_function) return false;

      return m_function->setCodeletString(name);
    }
    
    inline bool FunctionSetterHelper::setFunctionName(std::string name)
    {
      if(!m_function) return false;

      return m_function->setFunctionName(name);
    }
    
    // setters specific to ModelFunction
    
    inline bool FunctionSetterHelper::setProvidesParameterGradient(bool yes)
    {
      //      if(!m_modelfun) return false;
      //      return m_modelfun->setProvidesParameterGradient(yes);

      return m_function->setProvidesParameterGradient(yes);
    }
    

    inline bool FunctionSetterHelper::setNormalization(bool provides, bool is_normalized)
    {
      //      if(!m_modelfun) return false;
      //      return m_modelfun->setNormalization(provides,is_normalized);

      return m_function->setNormalization(provides,is_normalized);
    }

    inline bool FunctionSetterHelper::setNormalizationRange(int ix, const RangeSet & rs)
    {
      // should not be possible for unnormalized functions ??
      return m_function->setNormalizationRange(ix,rs);
    }



    inline bool FunctionSetterHelper::_constructThis(std::string typenameId, const FunctionCatalogue* cat)
      {
	return m_function->_constructThis(typenameId, cat);
      }
    /*
inline bool FunctionSetterHelper::_postConstructThis(std::string codelet_string, const FunctionCatalogue* cat)
{
  return m_function->_postConstructThis(codelet_string, cat);
}

inline bool FunctionSetterHelper::_preConstructThis(std::string codelet_string, const FunctionCatalogue* cat)
{
  return m_function->_preConstructThis(codelet_string, cat);
}
  */
    
  // }
}

#endif
