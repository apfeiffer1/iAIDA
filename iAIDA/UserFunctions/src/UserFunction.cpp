#include "UserFunctions/UserFunction.h"
#include "FML/UserModelFunction.h"
#include "AIDA_Function/AIDA_FunctionAdapter.h"
#include "PythonFunction.h"

pi_aida::UserFunction::UserFunction(FreeFun func, int dim, int npar, const std::string & name) 
{
  // this function are created directly from FML

  std::auto_ptr<fml::IFMLFunction>  fml_fun(new fml::UserModelFunction(func,dim,npar)); 

  // create AIDA function which is a wrapper to FML function 

  AIDA::IModelFunction * rep = new iAIDA::AIDA_Function::AIDA_FunctionAdapter(fml_fun); 

  setRepresentation(rep); 
  setName(name); 
  setTitle("Free Function" + name); 
}


// destroyed representation - function is not managed  
pi_aida::UserFunction::~UserFunction()
{
   AIDA::IFunction * rep = representation(); 
   if (!rep) delete rep;
}

// create from Python 

pi_aida::UserFunction::UserFunction(PyObject * func, int dim, int npar, const std::string & name) 
{
  // this function are created directly from FML

  std::auto_ptr<fml::IFMLFunction>  fml_fun(new fml::PythonFunction(func,dim,npar)); 

  // create AIDA function which is a wraper to FML function 

  AIDA::IModelFunction * rep = new iAIDA::AIDA_Function::AIDA_FunctionAdapter(fml_fun); 

  setRepresentation(rep); 
  setName(name); 
  setTitle("Python Function " + name);
}

// need a clone method (to bedone !)  
  
