# include "FML/Function.h"

# include "FML/CodeletStringParser.h"
# include "FML/FunctionNameMatcher.h"

# include "FML/FunctionCatalogue.h"
# include "FML/FunctionSetterHelper.h"

# include "FML/RangeSet.h"
 
# include "FML/Util/util.h"
//# include "FML/HepUtilities/string_conversion.h"

# include "FML/Util/debug.h"

# include <iostream>
# include <string>
# include <algorithm>

# include <typeinfo>

using std::cerr;
using std::endl;


namespace fml  
{
 
  // namespace FML
  // {

    Function::Function(int dimx, int parnum, bool provides_grad, bool provides_param_grad, bool provides_norm, std::string function_typeid)
      : m_grad_buf(dimx),
	m_param_grad_buf(parnum),
	m_provides_grad(provides_grad),
	m_var_names(dimx),
	m_par_names(parnum),
	m_params(parnum), 
	m_provides_param_grad(provides_param_grad),
	m_provides_normalization(provides_norm), 
	// if function provide normalization - default is not normalized
	m_is_normalized(false)  
    {

      for(int i=0; i<parnum; ++i)
	{
	  m_par_names[i] = "p[" + Util::to_string(i) + "]";
	  m_params[i] = 0.0;
	}
      
      for(int i=0; i<dimx; ++i)
	{
	  m_var_names[i] = "x[" + Util::to_string(i) + "]";
	}

      // create ranges  
      if (m_provides_normalization) { 
	for (int i = 0; i < dimx; ++i) 
	  m_ranges.push_back(new RangeSet()); 
      }

      m_function_name = function_typeid;

      CodeletStringParser cp;
      cp.typenameId = function_typeid;
      m_codelet_string = cp.write();
      /* The NOTE BELOW is true but I cannot find the way to make it in pratice
       */

      /* automatic registration if the plugin has not been created by the 
         developper. clone() requires functions to be registered.
         if you have a simple program where you define your functions on the 
	 fly you rather do not want to be bothered by remembering to 
	 override clone() or create plugin. so, the plugin is created and
	 registred with the first instance. since at this point we have only
	 the function_typeid (and we do not have pattern) this will work only
	 for simple functions (i.e. whose names are not parameterised like P5)
      */

    }

    // destructors: delete ranges 
    Function::~Function() 
    { 
      for (unsigned int i=0; i < m_ranges.size(); ++i) 
	if (m_ranges[i]) delete m_ranges[i]; 
    }

    int Function::dimension() const { return m_var_names.size(); }   
    
    const std::vector<double>& Function::gradient(const std::vector<double>& x)  const  
    { return m_grad_buf; }

    bool Function::providesGradient() const { return m_provides_grad; }

    std::string Function::variableName(int i) const 
    {
      if(i<0 || i>= static_cast<int>( m_var_names.size() ) ) return "";
      else
	return m_var_names[i]; 
    }

    const std::vector<std::string>& Function::variableNames() const 
    {
      return m_var_names;
    }
  
    bool Function::setParameters(const std::vector<double>& params)
    {
      // PENDING: one of the two strategies to deal with logic errors!
      
      //      assert(m_params.size()==params.size());
      
      if(m_params.size()!=params.size()) 
	{
	  INFO_MSG(std::string("FML::Function::setParameters(vector): vector size mismatch (probable programming error in the application) in function: ") + functionName());
	  return false;
	}
      
      m_params = params;
      return true;
    }

   const std::vector<double>& Function::parameters() const { return m_params; }

   int Function::numberOfParameters() const
  {
    return m_params.size();
  } 
   
    const std::vector<std::string>& Function::parameterNames() const
    {
      return m_par_names;
    }


    bool Function::setParameter(std::string name, double val)
    {
      int ix = indexOfParameter(name);

      return setParameter(ix,val);
    }    

    bool Function::setParameter(int ix, double val)
    {
      if(ix<0 || ix > numberOfParameters()) { 
	INFO_MSG(std::string("FML::Function::SetParameter() -Error:  invalid index ") );
	return false; 
      }
      m_params[ix] = val;
      
      return true;
    }

    double Function::parameter(std::string name) const
    {
      int ix = indexOfParameter(name);
      
      return ix<0 ? 0.0 : m_params[ix];
    }
    

    int Function::indexOfParameter(std::string name) const
    {
      std::vector<std::string>::const_iterator pos;
      pos = std::find(m_par_names.begin(), m_par_names.end(), name);
      
      if(pos == m_par_names.end()) return -1;
      else
	return &(*pos)-&m_par_names[0];
    }
    
    bool Function::isEqual(const IFMLFunction * f) const
    {
      if(f)
	return typeid(*this) == typeid(*f);
      else
	return false;
    }

    /// This method may be used in the derived constructor to change default 
    /// names of parameters.
    bool Function::setParName(int ix, std::string name)
    {
      if(ix<0 || ix>= static_cast<int> (m_par_names.size()) ) return false;
      
      m_par_names[ix] = name;
      
      return true;
    }
    
    /// This method may be used in the derived constructor to change default 
    /// names of variables.
    bool Function::setVarName(int ix, std::string name)
    {
      if(ix<0 || ix>= static_cast<int>( m_var_names.size()) ) return false;
      
      m_var_names[ix] = name;
      
      return true;
    }

    std::string Function::codeletString() const
    {
      return m_codelet_string;
    }

    bool Function::setCodeletString(std::string name)
    {
      m_codelet_string = name;
      return true;
    }

    std::string Function::functionName() const
    {
      return m_function_name;
    }

    bool Function::setFunctionName(std::string name)
    {
      m_function_name = name;
      return true;
    }

    bool Function::setDimension(int dimx)
    {
      if(dimx>0)
	{
	  m_var_names.resize(dimx);
	  m_grad_buf.resize(dimx);

	  for(int i=0; i<dimx; ++i)
	    {
	      m_var_names[i] = "x[" + Util::to_string(i) + "]";
	    }
	  
	  return true;
	}
      else
	return false;
    }

    bool Function::setNumberOfParameters(int parnum)
    {
      m_par_names.resize(parnum);
      m_params.resize(parnum);
      m_param_grad_buf.resize(parnum);
      return true;
    }

    bool Function::setProvidesGradient(bool yes)
    {
      m_provides_grad = yes;
      return true;
    }

    // ------------------- advanced model functionality -------------------
    // need to be implemented an FML range

    /// Do not provide normalization (PENDING: for later)
    bool Function::providesNormalization() const { return m_provides_normalization; }
    void Function::normalize(bool on) { m_is_normalized = on; }

    bool Function::isNormalized() const { return m_is_normalized; }
  
    const std::vector<double>& Function::parameterGradient(const std::vector<double>& x) const { return m_param_grad_buf; }
  
    bool Function::providesParameterGradient() const { return m_provides_param_grad; }
  
    RangeSet *  Function::normalizationRange(int iAxis) const 
    { 
      if (m_provides_normalization) {
	/** remove checks on dimension() , they take too much time  
	if (static_cast<int>( m_ranges.size() ) != dimension() ) { 
	  INFO_MSG(std::string("FML::Function::NormalizationRange: Error - Invalid size for vactor of rangesets " ) );
	  return 0; 
	}		   
	if (iAxis < 0 || iAxis > dimension()) { 
	  INFO_MSG(std::string("FML::Function::NormalizationRange: Error - Invalid axis index ") ); 
	  return 0; 
	}
	*/
	if (iAxis < 0 || iAxis > static_cast<int>(m_ranges.size()) ) { 
	  INFO_MSG(std::string("FML::Function::NormalizationRange: Error - Invalid axis index ") ); 
	  return 0; 
	}

	return m_ranges[iAxis]; 
      }
      else 
	return 0; 
    }
    void Function::includeNormalizationAll() 
    {
      for (int i = 0; i < dimension(); ++i) 
	m_ranges[i]->includeAll(); 
    }
    void Function::excludeNormalizationAll() 
    {
      for (int i = 0; i < dimension(); ++i) 
	m_ranges[i]->excludeAll(); 
    }

    bool Function::setNormalization(bool provides, bool is_normalized)
    {
      if(!provides && is_normalized) return false;
      
      m_provides_normalization = provides;

      m_is_normalized = is_normalized ; 
      // create ranges if were not creates before .
      if (m_provides_normalization) { 
	if ( m_ranges.empty() ) { 
	  // if range is empty create one with defaults -inf + inf 
	  for (int i = 0; i < dimension(); ++i) 
	    m_ranges.push_back(new RangeSet());
	} 
      }

      return true;
    }

    bool Function::setNormalizationRange(int ir, const RangeSet & rs)
    {
      unsigned int ix = ir; 
      if (!m_provides_normalization) return false; 
      if ( ix >= m_ranges.size()) return false; 
      // delete previous existing range
      if (m_ranges[ix]) delete m_ranges[ix]; 

      RangeSet * rs_new = new RangeSet(rs); 
      if (!rs_new) return false; 
      m_ranges[ix] = rs_new;

      return true;
    }

    bool Function::setProvidesParameterGradient(bool yes)
    {
      m_provides_param_grad = yes;
      return true;
    }

    bool Function::isStreamable() const
    {
      return true;
    }

  bool Function::_constructThis(std::string functionName, const FunctionCatalogue*)
  {
    setFunctionName(functionName);
    return true;
  }

  /// Default implementation using _copy_state_to().
  std::auto_ptr<IFMLFunction> Function::clone() const
  {
    std::auto_ptr<IFMLFunction> f(FunctionCatalogue::createFunction(this->functionName()));

    if(f.get() && _copy_state_to(*f.get())) 
      return std::auto_ptr<IFMLFunction>(f.release());
    else
      {
	INFO_MSG(std::string("FML: cannot clone function: ")+functionName());
	INFO_MSG(std::string("FML: this is a misbehaviour in your application"));
	INFO_MSG(std::string("FML: you should either create a plugin for this function"));
	INFO_MSG(std::string("FML: or define clone() method manually in your function class"));
	INFO_MSG(std::string("FML: please refer to FML user guide for details"));

	return std::auto_ptr<IFMLFunction>();
      }
  }

  bool Function::_copy_state_to(IFMLFunction &target) const
  {
    // do we need to check if the type is the same or not?
    //if(typeid(target) != typeid(this)) return false;
    //    else
      {
	FunctionSetterHelper tg(&target);

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

  // }
}
