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

# include "FML/UnbinnedMLFitMethod.h"

# include "FML/FitPlugin.h"

# include "FML/DataVectorIterator.h"

# include <cmath>
# include <float.h>

namespace fml {
// namespace FML {

static Util::Plugin<FitMethodFunction, UnbinnedMLFitMethod> plugin("UnbinnedML");

UnbinnedMLFitMethod::UnbinnedMLFitMethod() : FitMethodFunction(false,"unbinnedML")
{
  //  INFO_MSG("UnbinnedML created!");
}

UnbinnedMLFitMethod::~UnbinnedMLFitMethod()
{
  //  INFO_MSG("UnbinnedML destructed!");
}

const double pml_A = 1e-3;

inline double comp_ml_val(double fun_val)
{
  
  // safety measure against negative argument to the log,
  // not yet impl
  double tmp = fun_val; 
  if(fun_val<= FLT_MIN*5 )
    tmp = FLT_MIN*5; 
 
  return -std::log(tmp);

}

double UnbinnedMLFitMethod::value(const std::vector<double>&p) const { 
  IFMLFunction * function = modelFunction(); 
  IFMLDataIterator * data = dataIterator();  

  assert( function );
  assert( data );
  
  function->setParameters( p );
  int nParams = function->numberOfParameters(); 

  double val = 0.0;

  bool provGrad = providesGradient();
  if (provGrad)
    std::fill_n(m_grad_buf.begin(), m_grad_buf.size(), 0.0);


  // if used DataVectorIterator then is in memory and use stl iterator for best performances
  DataVectorIterator * dataVI = dynamic_cast<DataVectorIterator *>(data); 

  if (dataVI == 0) 
    { 

      for(data->start(); data->current(); data->next())
	{
	  const std::vector<double>& x = data->coords();	  
	  double fun_val = function->value(x);
	  val += comp_ml_val( fun_val );

	  if (provGrad ) 
	    { 	    
	    const std::vector<double>& pgrd = function->parameterGradient(x);
	    for(int j=0; j< nParams; j++)
	      if(fun_val<=FLT_MIN*5)	    
		m_grad_buf[j] += - FLT_MAX/5 * pgrd[j]; 
	      else
		m_grad_buf[j] += -1./fun_val * pgrd[j];
	    
	    }

	}    
    }
  
  else 
    { 
      // use STL iterator 
      const DataVector * vec = dataVI->dataVec();
      typedef std::vector<DataPoint>::const_iterator DataVectorIter;
      const std::vector<DataPoint> & points = vec->dataPoints();
      
      for ( DataVectorIter itr = points.begin(); itr != points.end(); ++itr)
	{      
	  const std::vector<double>& x = itr->coords();	  
	  double fun_val = function->value(x);
	  val += comp_ml_val( fun_val );


	  if (provGrad ) 
	    { 	    
	    const std::vector<double>& pgrd = function->parameterGradient(x);
	    for(int j=0; j<nParams; j++)
	      if(fun_val<=FLT_MIN*5)	    
		m_grad_buf[j] += - FLT_MAX/5 * pgrd[j]; 
	      else
		m_grad_buf[j] += -1./fun_val * pgrd[j];
	    
	    }

	}    

    }

  return val;
}


// }
}
