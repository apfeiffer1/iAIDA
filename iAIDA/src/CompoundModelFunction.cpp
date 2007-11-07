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

# include "FML/CompoundModelFunction.h"


# include "FML/CodeletStringParser.h"


# include "FML/Util/util.h"

namespace fml
{

// namespace FML
// {

  CompoundModelFunction::CompoundModelFunction(bool owned_members) : Function(0,0,false,false, false, "compound")
  {
    m_owned_members = owned_members;
    setFunctionName("");
  }

  CompoundModelFunction::~CompoundModelFunction()
  {
    if(m_owned_members)
      {
	std::vector<IFMLFunction*>::iterator it;

	for(it=m_members.begin(); it!=m_members.end(); ++it)
	  delete *it;
      }
  }
  
  bool CompoundModelFunction::append(IFMLFunction *member_fun)
  {
    if(member_fun)
      {
	// first member dictates the dimension
	if(m_members.empty())
	  {
	    setDimension(member_fun->dimension());
	    
	    for(int i=0; i<dimension(); ++i)
	      setVarName(i, member_fun->variableName(i));

	    // set initial value
	    setProvidesGradient(member_fun->providesGradient());
	    setProvidesParameterGradient(member_fun->providesParameterGradient());

	  }

	// all following members must match the dimension
	if(dimension() == member_fun->dimension())
	  {
	    // construct new parameter names with _i appended
	    std::vector<std::string> constructed_pnames; // = parameterNames();
	    int new_memb_index = m_members.size();
	    int new_par_number = member_fun->numberOfParameters();
	    for(int i=0; i<new_par_number; ++i)
	      {
		// make new parameter names
		
		std::string new_name = member_fun->parameterNames()[i];

		if(new_name != "")
		  new_name += "_" + Util::to_string(new_memb_index);

		constructed_pnames.push_back(new_name);
	      }

	    // add new member function
	    m_members.push_back(member_fun);

	    int old_par_number = numberOfParameters();

	    setNumberOfParameters(old_par_number+new_par_number);

	    for(int i=0; i<new_par_number; ++i)
	      {
		// first must create the cache
		m_param_owners.push_back(member_fun);
		m_param_shift.push_back(old_par_number);

		setParName(old_par_number+i, constructed_pnames[i]);
		setParameter(old_par_number+i, member_fun->parameters()[i]);
	      }


	    setProvidesGradient(providesGradient() && member_fun->providesGradient());
	    setProvidesParameterGradient(providesParameterGradient() && member_fun->providesParameterGradient());

	    // PENDING:
	    // here copy the normalization range and 

	    bool normalized = false;
	    bool provides_norm = providesNormalization() && 
	      member_fun->providesNormalization();

	    // if this already normalized normalize new member too
	    if(provides_norm)
	      normalized = isNormalized();

	    setNormalization(provides_norm, normalized);


	    CodeletStringParser cp, cp1;
	    
	    cp.read(codeletString());
	    cp1.read(member_fun->codeletString());
	    
	    cp.typenameId += "_" + cp1.typenameId;
	    
	    setCodeletString(cp.write());

	    return true;
	  }
      }

    return false;
  }
  
  
  const IFMLFunction* CompoundModelFunction::member(int i) const
  {
    if(i>=0 && i<numberOfMembers())
      return m_members[i];
    else
      return 0;
  }
  
  int CompoundModelFunction::numberOfMembers() const
  {
    return m_members.size();
  }

  bool CompoundModelFunction::setParameters(const std::vector<double>& params)
  {
    unsigned int np = numberOfParameters();
    
    //cout << " np = " << np << endl; 

    if(params.size()==np && np!=0)
      {
	for(unsigned int gi=0; gi<np; ++gi)
	  {
	    double val = params[gi];
	    //cout << " param " << gi << " = " << val; 

	    bool lret =  setParameter(gi,val);
	    if (!lret) return false; 
	    //	    if(!m_param_owners[gi]->setParameter(gi-m_param_shift[gi],val))
	    //  return false;
	  }	
	//cout << endl; 
      }
    else
      return false;

    return true;

    /* unoptimized version */
    /*
    if(params.size()==m_params.size()) 
      {
	std::vector<IFMLFunction*>::iterator it;
	
	int glob_index = 0;
	
	for(it=m_members.begin(); it!=m_members.end(); ++it)
	  {
	    for(int i=0; i<(*it)->numberOfParameters(); ++i)
	      {
		double val = params[glob_index];
		
		setParameter(glob_index,val);
		(*it)->setParameter(i,val);
		
		++glob_index;
	      }
	    
	  }
      }
    */
  }

  /*   
  
  bool CompoundModelFunction::membersHaveGradient() const
  {
    return m_memb_have_grad;
  }
  
  bool CompoundModelFunction::membersHaveParamGradient() const
  {
    return m_memb_have_param_grad;
  }
  */
  /*
  std::string variableName(int i) const
  {
     return m_members.empty() ? "" : m_members[0]->variableName(i);   
  }

  const std::vector<std::string>& variableNames() const
  {
     return m_members.empty() ? empty_vector_of_strings 
       : m_members[0]->variableNames();       
  }
  */

  bool CompoundModelFunction::setParameter(int ix, double x)
  {
    if(Function::setParameter(ix,x)) { 
      if ( ix < static_cast<int>(m_param_owners.size() ) && 
	   ix < static_cast<int>(m_param_shift.size() ) )
	return m_param_owners[ix]->setParameter(ix-m_param_shift[ix],x);
      else
	return true;
    }
    else
      return false;    
  }

  bool CompoundModelFunction::setParameter(std::string nm, double x)
  {
    return Function::setParameter(nm,x);
  }
// }
}
