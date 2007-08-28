# include "FML/PoissonMLFitMethod.h"

# include "FML/FitPlugin.h"
# include "FML/DataVectorIterator.h"


# include <cmath>


namespace fml {
// namespace FML {

static Util::Plugin<FitMethodFunction, PoissonMLFitMethod> plugin("PoissonML");


PoissonMLFitMethod::PoissonMLFitMethod() : FitMethodFunction(false,"PoissonML")
{
  //  INFO_MSG("PoissonML created!");
}

PoissonMLFitMethod::~PoissonMLFitMethod()
{
  //  INFO_MSG("PoissonML destructed!");
}

inline double my_abs(double x) { return x<0 ? -x : x; }

const double pml_A = 1e-3;

inline double comp_pml_val(double fun_val, double data_val, double binVol)
{
  double logtmp;
  
  // safety measure against negative argument to the log,
  // smooth linear extrapolation below pml_A
  if(fun_val<=pml_A) logtmp = fun_val/pml_A + std::log(pml_A) - 1; 
  else       logtmp = std::log(fun_val);

  return fun_val*binVol - data_val*logtmp;  
}

double PoissonMLFitMethod::value(const std::vector<double>& p) const
{
  IFMLFunction * function = modelFunction(); 
  IFMLDataIterator * data = dataIterator();  

  assert( function );
  assert( data );

  function->setParameters( p );
  int nParams = function->numberOfParameters(); 

  double val = 0.0;
  double binVol = 1.0;

  bool provGrad = providesGradient(); 
  if (provGrad)
    std::fill_n(m_grad_buf.begin(), m_grad_buf.size(), 0.0);


  // if used DataVectorIterator then is in memory and use stl iterator for best performances
  DataVectorIterator * dataVI = dynamic_cast<DataVectorIterator *>(data); 


  // use generic FML iterator 
  if (dataVI == 0) 
    { 

      for(data->start(); data->current(); data->next())
	{
	  const std::vector<double>& dx = data->coords();	  
	  double data_val =  data->value(); 
	  double fun_val = function->value(dx); 
	  val += comp_pml_val(fun_val, data_val, binVol);

	  if ( provGrad ) 
	    { 	    
	      const std::vector<double>& pgrd = function->parameterGradient(dx);
	      for(int j=0; j< nParams; j++)
		if(fun_val<=pml_A)	    
		  m_grad_buf[j] += (binVol-data_val/pml_A) * pgrd[j]; 
		else
		  m_grad_buf[j] += (binVol-data_val/fun_val) * pgrd[j];
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
	  const std::vector<double>& dx = itr->coords();	  
	  double data_val =  itr->value(); 
	  double fun_val = function->value(dx); 
	  val += comp_pml_val(fun_val, data_val, binVol);

	  if (provGrad ) 
	    { 	    
	      const std::vector<double>& pgrd = function->parameterGradient(dx);
	      for(int j=0; j< nParams; j++)
		if(fun_val<=pml_A)	    
		  m_grad_buf[j] += (binVol-data_val/pml_A) * pgrd[j]; 
		else
		  m_grad_buf[j] += (binVol-data_val/fun_val) * pgrd[j];
	    }
	}

    }

  return val;
}

// }
}
