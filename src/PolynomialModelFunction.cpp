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

# include <assert.h>

# include "FML/PolynomialModelFunction.h"

# include "FML/FunctionNameMatcher.h"
//# include "FML/HepUtilities/string_conversion.h"
# include "FML/Util/util.h"
# include "FML/Util/Plugin.h"
# include <math.h>

namespace fml {
// namespace FML {

// "@typeid@$iDegree$" is a pattern that allows to map strings like P5 into P
// to correctly fetch the implementation of polynomial
std::string PolynomialModelFunction::poly_pattern("@typeid@$iDegree$");

std::string PolynomialModelFunction::poly_typeid("P");


PolynomialModelFunction::PolynomialModelFunction()
  : Function(1,1,true,true,false,poly_typeid)
{
  constructor_init(1);
}

PolynomialModelFunction::PolynomialModelFunction(int n) 
  : Function(1,n+1,true,true,false,poly_typeid)
{
  constructor_init(n+1);
}

PolynomialModelFunction:: PolynomialModelFunction(const std::vector<double>& params)
  : Function(1,params.size(),true,true, false, poly_typeid)
    {
      // at least one parameter
      assert(params.size()>0);

      constructor_init(params.size());

      // virtual not overriden so safe in constructor 
      // but explicit name scope anyway
      Function::setParameters(params);
    }

  double PolynomialModelFunction::value(const std::vector<double>& x_vec) const
  {
    assert(x_vec.size()>0);
    double x = x_vec[0];
    int n = numberOfParameters() -1;
    
    const std::vector<double>& p = parameters();
    

    // LM: use as in NR  for evaluation
    double val = p[n]; 
    for(int i = n-1; i >= 0; i--)
      {
	val *= x + p[i]; 
      }

    return val;
  }

  const std::vector<double>& PolynomialModelFunction::gradient(const std::vector<double>& x_vec) const
  {
    assert(x_vec.size()>0);
    double x = x_vec[0];

    int n = numberOfParameters() - 1;
    
    const std::vector<double>& p = parameters();

    // LM: use as in NR  for evaluation
    double val = n*p[n]; 
    for(int i=n-1; i>0; i--)
      {
	val *= x + i* p[i]; 
      }
    
    m_grad_buf[0] = val;
    return m_grad_buf;
}

const std::vector<double>& PolynomialModelFunction::parameterGradient(const std::vector<double>& x_vec) const 
{
  assert(x_vec.size()>0);
  double x = x_vec[0];
  int n = numberOfParameters();

  double tmp = 1.0;

  for(int i=0;i<n;i++)
    {
      m_param_grad_buf[i] = tmp;
      tmp *= x;
    }

  return m_param_grad_buf;
}

void PolynomialModelFunction::constructor_init(int npar)
{
  std::vector<double> def_par(npar,1.0);

  Function::setParameters(def_par);  

  for(int i=0; i<numberOfParameters(); ++i)
    setParName(i, std::string("a")+Util::to_string(i));

  setFunctionName(poly_typeid+Util::to_string(npar-1));
}


bool PolynomialModelFunction::_constructThis(std::string functionName, const FunctionCatalogue*)
{
    if(!Function::_constructThis(functionName)) return false;

    FunctionNameMatcher matcher(poly_pattern);
    matcher.setParameter("typeid", poly_typeid);

    if(!matcher.match(functionName)) return false;

    int degree = matcher.intVariable("iDegree");
    if(degree < 0) return false;

    setNumberOfParameters(degree+1);
    constructor_init(degree+1);

    return true;
}

static Util::Plugin<IFMLFunction,PolynomialModelFunction> plugin(PolynomialModelFunction::poly_typeid,PolynomialModelFunction::poly_pattern); 


//}
}  // end namespace fml

///////////////////////////////////////////////////////////////////

# if 0 

/*
std::string PolyNameMatcher::match(std::string name)
{
  int deg = 0;

  if(readDegree(name,deg)) 
    return PolynomialModelFunction::functionTypename();
  else
    return "";
}

bool PolyNameMatcher::readDegree(std::string name, int & degree)
{
  // remove whitespaces in front
  std::string::size_type idx = name.find_first_not_of(" \t\n");
  
  if(idx != std::string::npos)
    name = name.substr(idx);

  if(name.empty()) return false;

  std::string bn = name.substr(0,PolynomialModelFunction::functionTypename().size());
  if(bn == PolynomialModelFunction::functionTypename())
    {
      std::string nn = name.substr(PolynomialModelFunction::functionTypename().size());
      int deg = 0;

      if(Util::to_value(nn,deg))
	{
	  degree = deg;
	  return true;
	}
    }

  return false;
}

*/

//static Util::Plugin<FunctionNameMatcher,PolyNameMatcher> matcher_plugin(PolynomialModelFunction::functionTypename());
// }
//}

# endif
