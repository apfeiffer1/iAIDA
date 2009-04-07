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

# ifndef FML_Function_h__included__
# define FML_Function_h__included__

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
 * \file FML Function
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

#include <memory> // for auto_ptr

# include <string>
# include <vector>

# include "FML/IFMLFunction.h"

namespace fml 
{
  // namespace FML
  // {

/**
 * Partial implementation of IFunction - provides basic services
 * automatically and needs only to override value() in a subclass.
 *
 * Derived classes  should at  least implement  value().   By default
 * gradient  is not available but  derived classes may  change this by
 * overriding grad() and passing true in Function's constructor.
 *  
 */

class Function : virtual public IFMLFunction
{
 public:

  virtual ~Function();

  /// This method must be defined in the derived class.
  virtual double value(const std::vector<double>& x) const = 0;

  int dimension() const;

  /**
   * Return true if functions are of the same class.
   *
   * This implementation is sufficient if distinct function objects
   * are defined in distinct classes.  Base version (from this class)
   * should be called in overriden versions of isEqual().  
   */
  bool isEqual(const IFMLFunction * f) const;

  /// By default return empty vector.
  const std::vector<double>& gradient(const std::vector<double>& x)  const;  

  /// Return value is specified in the constructor and is left for subclasses
  /// to decide.
  bool providesGradient() const;

  /// Return default name string "x[i]".
  std::string variableName(int i) const;

  const std::vector<std::string>& variableNames() const;
  
  /// Set new vector of parameters.
  bool setParameters(const std::vector<double>& params);
 
  bool setParameter(int ix, double x);

  const std::vector<double>& parameters() const;

  int numberOfParameters() const;





  /// Return default parameter names "p[i]".
  const std::vector<std::string>& parameterNames() const;

  /**
   * Set parameter by name.
   */
  bool setParameter(std::string name, double x);
  
  double parameter(std::string name) const;
  
  int indexOfParameter(std::string name) const;

  std::string codeletString() const;
  std::string functionName() const;

  /// Do not provide normalization (PENDING: for later)
  bool providesNormalization() const;
  void normalize(bool on);
  bool isNormalized() const;
  
  const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
  
  bool providesParameterGradient() const;
  
  RangeSet * normalizationRange(int iAxis) const; 
  void includeNormalizationAll();
  void excludeNormalizationAll();
  // added for convenience
  bool setNormalizationRange(int axis, const RangeSet & rs); 

  /// Default implementation using _default_clone() and _copy_state_to().
  std::auto_ptr<IFMLFunction> clone() const;

  /// Function IS streamable by default.
  bool isStreamable() const;

 protected:

  /**
   * Make a copy of internal state, false if error. 
   * The type of this must match the type of target otherwise false.
   */
  virtual bool _copy_state_to(IFMLFunction &target) const;

  /**
   * Constructor for derived classes.
   *
   * Construct function with given dimensionality of the domain space (dimx).
   * If derived class intends to provide gradient then flag 
   * provides_grad must be explicitly set true.
   *
   * Typically in the derived class you want to override the default 
   * names of parameters.
   *
   * Function name is assumed to be equal to function typeid, so if it is not
   * the case (e.g. for polynomials) then you should set it manually in your
   * derived constructor.
   */
  explicit Function(int dimx, int parnum, bool provides_grad, bool provides_param_grad, bool provides_norm, std::string function_typeid);

  /// Buffer for gradient vector - to be used by derived classes.
  mutable std::vector<double> m_grad_buf;

  /// Parameter gradient buffer.
  mutable std::vector<double> m_param_grad_buf;

  bool setDimension(int dimx);
  bool setNumberOfParameters(int parnum);
  bool setProvidesGradient(bool provides_gradient);
  bool setParName(int ix, std::string name);
  bool setVarName(int ix, std::string name);
  bool setCodeletString(std::string name);
  bool setFunctionName(std::string name);
  bool setProvidesParameterGradient(bool yes);
  bool setNormalization(bool provides, bool is_normalized);


  /// do nothing by default and return true.
  //  bool _postStreamThis(std::string codelet_string, const FunctionCatalogue* cat = 0); 

  /// do nothing by default and return true.
  //  bool _preStreamThis(std::string codelet_string, const FunctionCatalogue* cat = 0); 

  /// do nothing by default and return true.
  bool _constructThis(std::string functionName, const FunctionCatalogue* cat = 0); 

 private:

  bool m_provides_grad;
  std::vector<std::string> m_var_names;
  std::vector<std::string> m_par_names;
  std::vector<double> m_params;
  std::string m_codelet_string;
  std::string m_function_name;
  // ranges 
  std::vector<RangeSet *> m_ranges; 

  bool m_provides_param_grad;
  bool m_provides_normalization;
  bool m_is_normalized; 

};



  // }
}


# endif
