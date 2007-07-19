# ifndef FML_Minimizer_h__included__
# define FML_Minimizer_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/IFMLFunction.h"

# include "FML/MinimizationVariable.h"
# include "FML/DomainConstraints.h"
# include "FML/MinimizerControlParams.h"
# include "FML/MinimizerResult.h"

namespace fml
{

// namespace FML {

/**
 * Generic minimizer.
 *
 * IFMLFunction, DomainConstraints, vector of
 * MinimizationVariable and algorithm control parameters
 * (MinimizerControlParams) are input objects to the
 * minimizer. MinimizerResult object is an output object.
 *
 * The default constructor allows delayed initialization by calling reset().
 * If correctly initialized, it is guaranteed that some objective function is
 * defined which is the only obligatory object that must be provided
 * by the user. Objective function determines the type and
 * dimension of the problem space in which minimization is
 * performed. Other input objects must have the same 
 * dimension.
 *
 * The following rule holds true.  Setting object via an interface
 * Ixxxx is performed as a pointer assignment to external object.  In
 * all other cases the value of the object is stored locally.
 *
 * \anchor restart 
 *
 * Setting new input object restarts minimizer. This means that
 * previous minimization results (if exist) are forgotten and the
 * minimization algorithm will start from scratch next time minimize()
 * is called. The internal state of minimizer is state_reset.
 *
 * \anchor indexing
 *
 * The number of variables is determined by the dimension of
 * the problem space. Thus, indexing goes from 0 to dimension()-1.
 *
 * \ingroup FML_LIB
 */

class Minimizer
{
 public:

  /**
   * Set objective function, other inputs objects are set to default.
   */
  Minimizer(IFMLFunction *objfun);

  /**
   * No objective function defined. Minimizer is in the invalid state.
   * Minimizer will not accept setting input objects nor running the engine.
   * Delayed initialization may be done by calling reset().
   */

  Minimizer();

  virtual ~Minimizer();

  /**
   * Override objective function, forget all input objects and \ref restart 
   * minimizer.
   * 
   * Old objective function is overriden and new one may define
   * different domain space. All other input objects are forgotten.
   * The call is void if objfun == 0. 
   */
  void reset(IFMLFunction *objfun);

  /**
   * Set new objective function and \ref restart minimizer.  
   *
   * The domain space of new objective function must match 
   * the domain space of currently used objective function.
   * Return false, if it does not match.
   * The call is void if objfun == 0.
   */  
  bool setFunction(IFMLFunction *objfun);

  /**
   * Get currently used objective function object.
   */
  IFMLFunction *objectiveFunction() const { return m_objfun; }  

  /** 
   * Set new minimization variables and \ref restart minimizer.
   * 
   * The number of variables must match the dimension()
   * the domain space defined by objective function.
   * If it does not match then false is returned.
   * PEDNING: const & 
   */
  bool setVariables(std::vector<MinimizationVariable> init_vars);

  /**
   * Reset minimization variables to default and \ref restart minimizer.
   *
   * Existing bounds are removed and default starting point is selected.
   */
  void setVariablesToDefault();

  /**
   * Get currently used minimization variables for reading.
   */
  const std::vector<MinimizationVariable>& variables() const { return m_vars; }

  /**
   * Set one variable at a time (\ref indexing) 
   * and \ref restart minimizer.
   */
  bool setVariable(int idx, const MinimizationVariable &v);

  /**
   * Get one variable at a time (\ref indexing) for reading.
   */
  const MinimizationVariable &variable(int idx) const;

  /**
   * Set to default one variable at a time (\ref indexing)
   * and \ref restart minimizer.
   */
  void setVariableToDefault(int idx);

  /**
   * Set new domain constraints and \ref restart minimizer.
   *
   * Constraints space must match the domain space defined by 
   * objective function.
   * If it does not match then false is returned.
   *
   * Note that some engines (Minuit) does not allow constraints,
   * so this function may be overriden to always return false.
   */
  virtual bool setConstraints(const DomainConstraints& c);

  /**
   * Remove all existing constraints and \ref restart minimizer.
   */
  void setConstraintsToDefault();

  /**
   * Get current constraints.
   */
  const DomainConstraints &constraints() const { return m_constr; }

  /**
   * Set new control parameters and \ref restart minimizer.
   */
  void setControlParameters(const MinimizerControlParams& p);

  /**
   * Reset control parameters to default.
   */  
  void setControlParametersToDefault();

  /**
   * Get current minimizer control parameters.
   */
  const MinimizerControlParams& controlParameters() const { return m_cparams; }

  /**
   * Ask the underlying minimization engine for the default numbers it uses.
   * If the value of a parameter may not be retrieved, then it is set
   * as implicit.
   *
   * Note that these numbers may depend on other input objects 
   * (for example the number of constraints and variables - 
   * this is the case of Nag C minimizer).
   * 
   * Note for implementators:
   *
   * This method should be overriden in derived classes like this:
   *
   * \verbatim
    SpecificMinimizer::defaultControlParameters() const
    {
      /// obtain somehow current defaults, say into the object c
      MinimizerControlParameters c = ...;
      return _update_current_default_parameters(c);
    }
    \endverbatim  
   */
  virtual const MinimizerControlParams& defaultControlParameters() const = 0;

  /// Return true if minos errors computation is switched on.
  bool minosErrorsComputation() const { return m_minosflag; }

  /// Switch minos errors computation.
  void switchMinosErrors(bool flag) { m_minosflag = flag; }

  /// Return current print level (-1 = no output,0,1,2,3 = most verbose).
  int  printLevel() const { return m_printlevel; }

  /// Set print level.
  void setPrintLevel(int printlevel) { m_printlevel = printlevel; }

  /// Return true if warnings shall be printed to standard output
  /// by minimizer engine.
  bool warnings() const { return m_warningsflag; }

  /// Switch warnings flags.
  void switchWarnings(bool flag) { m_warningsflag = flag; }

  /// Print current setup on output stream.

  virtual void printSetup(std::ostream& os = std::cout) const;

  /// Print current results on output stream.
  /// Return false if not avaiable.

  virtual bool printResults(std::ostream& os = std::cout) const;

  /**
   * Run minimization algorithm.
   *
   * Return true if minimization terminated OK and the results are available.
   * Otherwise return false.
   */  
  virtual bool minimize() = 0;

  /**
   * Retrieve results of previously performed minimization.
   *
   * If results are not available returned object is no valid. 
   * For example:
   *
   * \verbatim
   
    obj = minimizer.result();
    if(obj.isValid())
     ... // do things
    else
     ... // results are not available
   
    \endverbatim
   */  
  virtual const MinimizerResult& result() const { return m_result; }

  /**
   * Number of variables (dimension of the domain-space).
   * Always equal to the dimension of objective function. 
   * If minimizer not initialized equal to 0.
   */
  int dimension() const 
    {
      return objectiveFunctionDefined() ? objectiveFunction()->dimension() : 0;
    }

 protected:

  /**
   * Indicate current state of minimizer.
   * If not run (or is reset) then state_reset.
   * If run successfully then state_success.
   * If run but failed and results are not available then state_fail.
   */
  enum {state_reset, state_success, state_fail} m_state;
  
  /**
   * Auxilary function that is called every time minimizer becomes reset.
   * May be overriden is subclasses in order to perform some extra cleanup
   * (like deleting the minimization engine).
   * If overriden, Minimizer::doResetState() must be called from overriding
   * method.
   */
  virtual void doResetState() 
    { 
      m_state = state_reset; 
      m_result = MinimizerResult(); // clear result object
    }

  /// Auxiliary function to set output result object.
  void _set_result(const MinimizerResult &r)
    {
      m_result = r;
    }


  /// Auxiliary funciton to set default control parameters buffer.
  /// Return the default parameters buffer.

  const MinimizerControlParams& _update_current_default_parameters(const MinimizerControlParams& c) const
    {
      m_cparams_defaults = c;
      return m_cparams_defaults;
    }

  

  /// True if variable index is valid. 
  /// Index is never valid if no objective function.

  bool indexIsValid(int idx) const
  {
    if(objectiveFunction()==0) return false;
    return idx >=0 && idx < objectiveFunction()->dimension(); 
  }

  bool objectiveFunctionDefined() const 
   { 
    return objectiveFunction() != 0; 
   }


 private:

  /// construction time initialization - common to all constructors
  void _constructor_init(IFMLFunction *objfun);

  /// unconditional assignments, objfun assumed to be defined!
  void _set_variables_to_default();
  void _set_constraints_to_default();
  void _set_control_parameters_to_default();
  
  IFMLFunction *m_objfun;
  
  std::vector<MinimizationVariable> m_vars;
  
  DomainConstraints m_constr;

  MinimizerControlParams m_cparams;

  mutable MinimizerControlParams m_cparams_defaults;

  MinimizerResult m_result;

  int m_printlevel;
  bool m_warningsflag, m_minosflag;
};

// }
}

# endif
