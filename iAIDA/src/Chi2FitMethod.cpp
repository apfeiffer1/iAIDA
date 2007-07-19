# include "FML/Chi2FitMethod.h"

# include "FML/FitPlugin.h"
# include "FML/Util/debug.h"

# include "FML/DataVectorIterator.h"

namespace fml
{
// namespace FML {

static Util::Plugin<FitMethodFunction, Chi2FitMethod> plugin("chi2");

inline double my_abs(double x) { return x<0 ? -x : x; }


Chi2FitMethod::Chi2FitMethod() : FitMethodFunction(false,"chi2")
{
  m_ncall = 0;
  //  INFO_MSG("Chi2 created!");
}

Chi2FitMethod::~Chi2FitMethod()
{
  std::cout << "Total number of calls for chi2 value is " << m_ncall << std::endl; 
  //  INFO_MSG("Chi2 destructed!");
}

double Chi2FitMethod::value(const std::vector<double>& p ) const
{
  // count number of calls
  m_ncall++;

  IFMLFunction * function = modelFunction(); 
  IFMLDataIterator * data = dataIterator();  

  assert( function );
  assert( data );
  /*
  // even though model function is a (private) mutable data member
  // compiler cannot properly resolve constness of the associated
  // (protected) getter method, is this standard behaviour or only
  // a gcc-2.95.2 feature?
  const_cast<Chi2FitMethod*>(this)->modelFunction()->setParameters(x);
  */

  function->setParameters( p );

  int nParams = function->numberOfParameters(); 

  //  DBG_VAL(m_grad_buf.size());
  //  DBG_VAL(modelFunction()->numberOfParameters());

  assert(m_grad_buf.size() == static_cast<unsigned int> (function->numberOfParameters() ));

  double val = 0.0;
  double tmp = 0.0;

  /// zero the buffer
  bool provGrad = providesGradient();
  if (provGrad) { 
    std::fill_n(m_grad_buf.begin(), m_grad_buf.size(), 0.0);
  }


  // if used DataVectorIterator then is in memory and use stl iterator for best performances
  DataVectorIterator * dataVI = dynamic_cast<DataVectorIterator *>(data); 

  if (dataVI == 0) 
    { 

      for(data->start(); data->current(); data->next())
	{      
	  
	  
	  //      const DataPoint *pt = &dataVector()->point(i);
	  //      const std::vector<double>& dx = pt->coords(); //coords(pt);
	  
	  const std::vector<double>& dx = data->coords();
	  
	  
	  double error_bar = data->vErrorPlus();
	  //	my_abs(data->vErrorPlus()) +
	  //my_abs(data->vErrorMinus());

	  if(error_bar==0.0) error_bar = 1.0;
	  //      else
	  //	error_bar /= 2.0;
	  
	  tmp = (data->value()-function->value(dx))/error_bar;
	  
	  val += tmp*tmp;
	  
	  if(provGrad)
	    {
	      const std::vector<double>& pgrd = function->parameterGradient(dx);
	      for(int j=0; j< nParams; ++j)
		m_grad_buf[j] += -2*tmp*pgrd[j]/error_bar;
	    }
	  
	}
    }
  
  // use STL iterator 
  else 
    { 
      const DataVector * vec = dataVI->dataVec();
      
      typedef std::vector<DataPoint>::const_iterator DataVectorIter;
      const std::vector<DataPoint> & points = vec->dataPoints();
      
      for ( DataVectorIter itr = points.begin(); itr != points.end(); ++itr)
	{      
      

	  //      const DataPoint *pt = &dataVector()->point(i);
	  //      const std::vector<double>& dx = pt->coords(); //coords(pt);
	  
	  const std::vector<double>& dx = itr->coords();
	  
	  
	  double inv_error = itr->invError();
	  
	  //if(error_bar==0.0) error_bar = 1.0;
      
	  tmp = (itr->value()-function->value(dx))* inv_error;
	  	  
	  val += tmp*tmp;
	  
	  if(provGrad)
	    {
	      const std::vector<double>& pgrd = function->parameterGradient(dx);
	      for(int j=0; j< nParams; ++j)
		m_grad_buf[j] += -2*tmp*pgrd[j] * inv_error;
	    }

	}
    }
  
  return val;
}

// }
}
