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

# include "FML/SumOfModelFunction.h"

# include <functional>
# include <algorithm>

#include "FML/Util/util.h"

namespace fml
{

// namespace FML
// {
  SumOfModelFunction::SumOfModelFunction(bool members_owned) : CompoundModelFunction(members_owned) , m_first(true), m_iAmpl(0)
  {
  }

  
  double SumOfModelFunction::value(const std::vector<double>& x) const
  {
    double res = 0.0;

    if (!isNormalized()) { 
      for(int i=0; i<numberOfMembers(); ++i)
	res += member(i)->value(x);
    }
    else { 
      // I might cache this later for improving performances 
      std::vector<double> params(numberOfParameters()); 
      params = parameters(); 
      int indexAmp = numberOfParameters() - numberOfMembers() + 1;
      double sumOfAmp = 0; 
      for(int i=0; i<numberOfMembers()-1; ++i) { 
	res += params[indexAmp+i]*member(i)->value(x);
	sumOfAmp += params[indexAmp+i];
	
      }

      // last item is 1- sum of the others 
      res += (1.-sumOfAmp)*member(numberOfMembers()-1)->value(x); 

    }

    return res;
  }
  
  const std::vector<double>& SumOfModelFunction::gradient(const std::vector<double>& x)  const
  {
    if(!providesGradient()) return m_grad_buf; // empty vector


    // zero the buffer
    std::fill(m_grad_buf.begin(),m_grad_buf.end(), 0.0); 

    // add all partial gradients
    for(int i=0; i<numberOfMembers(); ++i)
      std::transform(m_grad_buf.begin(), m_grad_buf.end(), 
		     member(i)->gradient(x).begin(), 
		     m_grad_buf.begin(),
		     std::plus<double>());

    return m_grad_buf;    
  }

  /*  
  bool SumOfModelFunction::hasGradient() const
  {
    //return false;
    return membersHaveGradient();
  }
  */
  
  const std::vector<double>& SumOfModelFunction::parameterGradient(const std::vector<double>& x) const
  {
    if(!providesParameterGradient()) return m_param_grad_buf; // empty vector

    std::vector<double>::iterator dest = m_param_grad_buf.begin();

    // no need to zero the buffer - current values will be overwritten

    // concatenate all param grad vectors and copy them into buffer
    for(int i=0; i<numberOfMembers(); ++i)
      {
	const std::vector<double>& v = member(i)->parameterGradient(x);

	dest = std::copy(v.begin(), v.end(), dest);
      }

    return m_param_grad_buf;
  }

  bool SumOfModelFunction::append(IFMLFunction *member_fun)
  {
    if(CompoundModelFunction::append(member_fun))
      {
	if(functionName()=="")
	  setFunctionName(member_fun->functionName());
	else
	  setFunctionName(functionName() + "+" + member_fun->functionName());

	// check if member provides normalization then also sum provides
	if (member_fun->providesNormalization())
	  setNormalization(true,true);
	// if one member does not provide and sum provides -switch off  
	else { 
	  if (providesNormalization())
	    setNormalization(false,false); 
	}  

	return true;
      }
    else
      return false;
  }

  // overwrite normalize to append parameters 
  
  void SumOfModelFunction::normalize(bool on) { 
    Function::normalize(on); 
    
    if (m_first) 
      appendAmpParameters(); 

  }

  // append parameters (in case members functions are normalized) 
  // I need to have  numberOfMemebers - 1 additional parameters
 
  
  void SumOfModelFunction::appendAmpParameters()
  {
    // be careful to call it only one time !!!
    if (!m_first) return; 
    if (!isNormalized() ) return; 

    m_first = false; 
    int n_old = numberOfParameters(); 
    int n_new = n_old + numberOfMembers() - 1;
    // add members - 1 parameters 
    Function::setNumberOfParameters(n_new ); 
    for (int i = n_old; i < n_new; ++i) { 
      std::string newName = "A_" + Util::to_string(m_iAmpl);
      Function::setParName(i,newName);
      double value = 1./double(numberOfMembers());
      Function::setParameter(newName,value); 
      // count number of parameters added ..
      m_iAmpl++;
    }

    // if normalized do not provide gradients (to be done..)
    setProvidesGradient(false); 
    setProvidesParameterGradient(false); 

  }

  /*  
  bool SumOfModelFunction::hasParameterGradient() const
  {
    //return false;
    return membersHaveParamGradient();
  }
  */

// }
}
