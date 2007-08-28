# include "FML/Minimizer.h"

# include "FML/DomainConstraints.h"
# include "FML/MinimizerControlParams.h"

//DomainConstraints default_constraints(0);
//MinimizerControlParams default_cparams;

# include <iomanip>

namespace fml
{

// namespace FML {

MinimizationVariable empty_variable;


Minimizer::Minimizer() : m_constr(0)
{
  _constructor_init(0);
}

Minimizer::Minimizer(IFMLFunction *objfun) : m_constr(0)
{
  _constructor_init(objfun);
}


Minimizer::~Minimizer()
{
  doResetState();
}

void Minimizer::_constructor_init(IFMLFunction *objfun)
{
  m_objfun = objfun;

  if(m_objfun)
    {
      _set_variables_to_default();
      _set_constraints_to_default();
      _set_control_parameters_to_default();
    }

  m_printlevel = 0;
  m_warningsflag = true;
  m_minosflag = false;
}

void Minimizer::reset(IFMLFunction *objfun)
{
  if(objfun == 0) return;

  // unconditional substitution so dimension may change
  m_objfun = objfun;


  _set_variables_to_default();
  _set_constraints_to_default();
  _set_control_parameters_to_default();

  doResetState();
}

void Minimizer::_set_variables_to_default()
{
  m_vars = std::vector<MinimizationVariable>(m_objfun->dimension());
}

void Minimizer::_set_constraints_to_default()
{
  m_constr = DomainConstraints(m_objfun->dimension());
}

void Minimizer::_set_control_parameters_to_default()
{
  m_cparams =  m_cparams_defaults;
}

bool Minimizer::setVariable(int idx, const MinimizationVariable &v)
{
  if(!objectiveFunctionDefined()) return false;
  if(!indexIsValid(idx)) return false;
  m_vars[idx] = v;
  doResetState();
  return true;
}

const MinimizationVariable& Minimizer::variable(int idx) const
{
  if(!objectiveFunctionDefined()) return empty_variable;

  return indexIsValid(idx) ? m_vars[idx] : empty_variable;
}

void Minimizer::setVariableToDefault(int idx)
{
  if(indexIsValid(idx)) 
    {
      m_vars[idx] = MinimizationVariable();
      doResetState();
    }
}

  
bool Minimizer::setFunction(IFMLFunction *objfun)
{
  if(!objectiveFunctionDefined() || objfun == 0) return false;
  
  // only if dimensionality matches old obj function
  if(m_objfun->dimension() == objfun->dimension())
    {
      m_objfun = objfun;
      doResetState();
      return true;
    }
    else
    return false;
}

bool Minimizer::setVariables(std::vector<MinimizationVariable> init_vars)
{
  if(!objectiveFunctionDefined()) return false;

  if( static_cast<int>(init_vars.size()) == m_objfun->dimension())
    {
      m_vars = init_vars;
      doResetState();
      return true;
    }
    else
    return false;
}

void Minimizer::setVariablesToDefault()
{
  if(!objectiveFunctionDefined()) return;
  _set_variables_to_default();
  doResetState();
}

bool Minimizer::setConstraints(const DomainConstraints& c)
{
  if(!objectiveFunctionDefined()) return false;

  if(c.dimension()==m_objfun->dimension() || c.isUnconstrained())
    {
      m_constr = c;
      doResetState();
      return true;
    }
    else
    return false;
}

void Minimizer::setConstraintsToDefault()
{
  if(!objectiveFunctionDefined()) return;

  _set_constraints_to_default();
  doResetState();
}

void Minimizer::setControlParameters(const MinimizerControlParams& p)
{
  if(!objectiveFunctionDefined()) return;
  m_cparams = p;
  doResetState();
}

void Minimizer::setControlParametersToDefault()
{
  if(!objectiveFunctionDefined()) return;
  _set_control_parameters_to_default();
  doResetState();
}


const MinimizerControlParams& Minimizer::defaultControlParameters() const
{
  return m_cparams_defaults;
}


void Minimizer::printSetup(std::ostream& os) const
{

  os << std::endl << "Minimizer setup:";

  printTable(variables(),os);

  // PENDING: pretty printout not finished yet
  os << std::endl << "constraints:";

  os << std::endl << "control parameters:";

  os << std::endl;

}


bool Minimizer::printResults(std::ostream& os) const 
{

  os << std::endl << "Minimizer results: ";

  if(!m_result.isValid())
    {
      os << std::endl << "*not available*" << std::endl; 
      return false;
    }

  printTable(m_result.variables(),os);

  return true;
}

// }
}


