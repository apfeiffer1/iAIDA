# ifndef FMLLIB_PythonFunction_h__included__
# define FMLLIB_PythonFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

#include "Python.h"
#include "FML/Function.h"

namespace fml {

/** 
 * Scalar function implemented with a global function pointer.
 *
 * This is a wrapper for global functions.
 *
 * \ingroup FML_LIB
 */

class PythonFunction : public Function
{
public:

  /// Signature of global function that may be captured.
  //typedef double (*Fun_ptr)(const std::vector<double>& x, const std::vector<double> & p);
  
  // needed for plugin manager
  PythonFunction();

  // destructur needed for deacrising PythonObject reference count
  ~PythonFunction();

  /// Capture global function and specify dimensionality.
  PythonFunction(PyObject * func, int dim, int npar);

  /// Global function call to compute function value.
  double value(const std::vector<double>& d) const;  

  protected:

  double value1D(double x) const;
  double value2D(double x, double y) const;
  double value3D(double x, double y, double z) const;

private:

  PyObject *  m_func;

  mutable std::string m_argDescr;
};


}
# endif
