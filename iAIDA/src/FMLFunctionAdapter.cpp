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

# include "AIDA_Function/FMLFunctionAdapter.h"
#include "AIDA_Function/AIDA_RangeSet.h"
#include "AIDA_Function/AIDA_FunctionAdapter.h"
#include "FML/RangeSet.h"

# include "FML/CodeletStringParser.h"
# include "FML/FunctionSetterHelper.h"
# include "FML/FunctionCatalogue.h"

# include "FML/Util/debug.h"

# include <typeinfo> 

namespace iAIDA  
{

  namespace AIDA_Function
  {

    FMLFunctionAdapter::FMLFunctionAdapter(std::auto_ptr<AIDA::Dev::IDevModelFunction> f, AIDA::Dev::IDevFunctionFactory * factory) : m_owner(true), m_function(f.release()), m_factory(factory)
    {
      assert(m_function);
      m_catalogue = 0;
    }

 FMLFunctionAdapter::FMLFunctionAdapter(AIDA::Dev::IDevModelFunction *f, AIDA::Dev::IDevFunctionFactory * factory) : m_owner(false), m_function(f), m_factory(factory)
    {
      assert(m_function);
      m_catalogue = 0;
    }

    FMLFunctionAdapter::~FMLFunctionAdapter() 
    {
      if(m_owner) delete m_function;
    }

    void FMLFunctionAdapter::useCatalogueIfNoFactory(fml::FunctionCatalogue *cat)
    {
      m_catalogue = cat;
    }

    double FMLFunctionAdapter::value(const std::vector<double>& x) const
    {
      return m_function->value(x);
    }

    int FMLFunctionAdapter::dimension() const 
    { 
      return m_function->dimension(); 
    }
    
    bool FMLFunctionAdapter::isEqual(const IFMLFunction * f) const
    {
      return typeid(f) == typeid(this);
    }


    const std::vector<double>& FMLFunctionAdapter::gradient(const std::vector<double>& x)  const  
    { 
      return m_function->gradient(x); 
    }

    bool FMLFunctionAdapter::providesGradient() const 
    { 
      return m_function->providesGradient(); 
    }

    std::string FMLFunctionAdapter::variableName(int i) const 
    { 
      return m_function->variableName(i); 
    }

    const std::vector<std::string>& FMLFunctionAdapter::variableNames() const 
    { 
      m_variableNames =  m_function->variableNames(); 
      return m_variableNames;
    }

    bool FMLFunctionAdapter::setParameters(const std::vector<double>& params)
    {
      m_function->setParameters(params);
      return true;
    }
    
    const std::vector<double>& FMLFunctionAdapter::parameters() const
    {
      return m_function->parameters();
    }
    
    int FMLFunctionAdapter::numberOfParameters() const
    {
      return m_function->numberOfParameters();
    }
    
    const std::vector<std::string>& FMLFunctionAdapter::parameterNames() const
    {
      
      m_parameterNames =  m_function->parameterNames();
      return  m_parameterNames;
    }
    
    bool FMLFunctionAdapter::setParameter(std::string name, double x)
    {
      return m_function->setParameter(name,x);     
    }

    bool FMLFunctionAdapter::setParameter(int ix, double x)
    {
      if(ix<0 || ix >= static_cast<int>(parameterNames().size()) ) return false;

      return m_function->setParameter(parameterNames()[ix], x);
    }
    
    double FMLFunctionAdapter::parameter(std::string name) const
    {
      return m_function->parameter(name);
    }
    
    int FMLFunctionAdapter::indexOfParameter(std::string name) const
    {
     return m_function->indexOfParameter(name);
    }
    

    std::string FMLFunctionAdapter::codeletString() const
    {
      return m_function->codeletString();
    }

    std::string FMLFunctionAdapter::functionName() const
    {
      fml::CodeletStringParser cp;

      if(!cp.read(codeletString())) return "";

      return cp.typenameId;
    }

      bool FMLFunctionAdapter::providesNormalization() const
      {
	return m_function->providesNormalization();
      }

      void FMLFunctionAdapter::normalize(bool on)
      {
	m_function->normalize(on);
      }

      bool FMLFunctionAdapter::isNormalized() const
      {
	return m_function->isNormalized();
      }

      const std::vector<double>& FMLFunctionAdapter::parameterGradient(const std::vector<double>& x) const
      {
	return m_function->parameterGradient(x);
      }
      
      bool FMLFunctionAdapter::providesParameterGradient() const
      {
	return m_function->providesParameterGradient();
      }

      fml::RangeSet * FMLFunctionAdapter::normalizationRange(int iAxis) const 
      { 	
	//return (m_function->getImplementation())->normalizationRange(iAxis); 
	AIDA::IRangeSet * rs = &m_function->normalizationRange(iAxis);
	AIDA_RangeSet * aida_rs = dynamic_cast< AIDA_RangeSet * >(rs); 
	assert(aida_rs); 
	return &aida_rs->getFMLRangeSet();
      }

      void FMLFunctionAdapter::includeNormalizationAll()
      {
	m_function->includeNormalizationAll();
      }

      void FMLFunctionAdapter::excludeNormalizationAll()
      {
	m_function->excludeNormalizationAll();
      }


    std::auto_ptr<fml::IFMLFunction> FMLFunctionAdapter::clone() const
    {
      if(m_factory)
	{
	  AIDA::IFunction *f1(m_factory->cloneFunction(m_function));

	  AIDA::Dev::IDevModelFunction *f2 = dynamic_cast<AIDA::Dev::IDevModelFunction*>(f1);

	  if(!f2)
	    {
	      INFO_MSG(std::string("FMLFunctionAdapter: cannot clone function: ")+functionName());
	      INFO_MSG(std::string("FMLFunctionAdapter: this is a misbehaviour in your application"));
	      INFO_MSG(std::string("FMLFunctionAdapter: factory of foreign AIDA implementation returns object which I cannot downcast to IDevModelFunction"));
	    }
	  
	  std::auto_ptr<fml::IFMLFunction> f3(new FMLFunctionAdapter(f2));

	  if(!f3.get())
	    {
	      INFO_MSG(std::string("FMLFunctionAdapter: new FAILED, cannot clone function: ")+functionName());
	    }

	  return f3;
	}

      // else (if m_factory == NULL)
      
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(this->functionName()));
      
      if(f.get() && _copy_state_to(*f.get())) 
	return std::auto_ptr<fml::IFMLFunction>(f.release());
      else
	{
	  INFO_MSG(std::string("FMLFunctionAdapter: cannot clone function: ")+functionName());
	  INFO_MSG(std::string("FMLFunctionAdapter: this is a misbehaviour in your application"));
	  INFO_MSG(std::string("FMLFunctionAdapter: you try to clone the FML wrapper for AIDA function without specyfing factory to clone"));
	  
	  INFO_MSG(std::string("FMLFunctionAdapter: cloning using function catalogue also failed"));
	  return std::auto_ptr<fml::IFMLFunction>();
	}
    }

  bool FMLFunctionAdapter::_copy_state_to(fml::IFMLFunction &target) const
  {
    // do we need to check if the type is the same or not?
    //if(typeid(target) != typeid(this)) return false;
    //    else
      {
	fml::FunctionSetterHelper tg(&target);

	tg.setDimension(dimension());
	tg.setNumberOfParameters(numberOfParameters());
	tg.setProvidesGradient(providesGradient());

	for(int i=0; i<numberOfParameters(); ++i)
	  tg.setParName(i, parameterNames()[i]);

	for(int i=0; i<dimension(); ++i) {
	  tg.setVarName(i, variableNames()[i]);
	  tg.setNormalizationRange(i,*normalizationRange(i));
	}

	tg.setCodeletString(codeletString());
	tg.setFunctionName(functionName());
	tg.setProvidesParameterGradient(providesParameterGradient());
	tg.setNormalization(providesNormalization(), isNormalized());

	target.setParameters(parameters());

	return true;
      }
  }

    bool FMLFunctionAdapter::isStreamable() const
    {
      return true;
    }

    //////////// DEVELOPER'S INTERFACE //////////////


    bool FMLFunctionAdapter::setDimension(int dim)
    {
      m_function->setDimension(dim);
      return true;
    }

    bool FMLFunctionAdapter::setNumberOfParameters(int parnum)
    {
      m_function->setNumberOfParameters(parnum);
      return true;
    }

    bool FMLFunctionAdapter::setProvidesGradient(bool yes)
    {
      m_function->setProvidesGradient(yes);
      return true;
    }


    bool FMLFunctionAdapter::setParName(int ix, std::string name)
    {
      std::vector<std::string> pn(parameterNames());

      if(ix>=0 && ix< static_cast<int>(pn.size()) ) pn[ix] = name;

      return m_function->setParameterNames(pn);
    }

    bool FMLFunctionAdapter::setVarName(int ix, std::string name)
    {
      std::vector<std::string> vn(variableNames());

      if(ix>=0 && ix < static_cast<int>(vn.size()) ) vn[ix] = name;

      return m_function->setVariableNames(vn);
    }
 
    bool FMLFunctionAdapter::setCodeletString(std::string codelet)
    {
      m_function->setCodeletString(codelet);
      return true;
    }

    bool FMLFunctionAdapter::setFunctionName(std::string fname)
    {
      fml::CodeletStringParser cp;

      if(!cp.read(codeletString())) return false;
      cp.typenameId = fname;
      setCodeletString(cp.write());

      return true;      
    }

    bool FMLFunctionAdapter::setProvidesParameterGradient(bool yes)
    {
      m_function->setProvidesParameterGradient(yes);
      return true;
    }

    bool FMLFunctionAdapter::setNormalization(bool provides, bool is_normalized)
    {
      m_function->setNormalization(provides,is_normalized);
      return true;
    }
    
    bool FMLFunctionAdapter::setNormalizationRange(int ix, const fml::RangeSet & rs)
    {
      // need to dynamic cast to implementation 
      iAIDA::AIDA_Function::AIDA_FunctionAdapter * impl_function = 
	dynamic_cast<iAIDA::AIDA_Function::AIDA_FunctionAdapter *>(m_function); 
      if (!impl_function) return false; 
      return impl_function->setNormalizationRange(ix,rs);
    }

    bool FMLFunctionAdapter::_constructThis(std::string /* functionName */ , const fml::FunctionCatalogue* /* cat */)
    {
      return true;
    }

    

  }
}
