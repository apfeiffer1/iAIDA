# include "PythonFunction.h"


# include "FML/Util/Plugin.h"


static const std::string userFunctionName = "PythonFunction";

namespace fml
{

  // needed for plugin manager
  PythonFunction::PythonFunction() : 
    Function(1,0,false,false,false,userFunctionName) 
  {}

  /// Capture global function and specify dimensionality.
  PythonFunction::PythonFunction(PyObject *  func, int dim, int npar) : 
    Function(dim,npar,false,false,false,userFunctionName), 
    m_func(func) 
  {
    // increase reference counts 
    Py_XINCREF(m_func); 
    // build string to define python tuple
    m_argDescr = "("; 
    if (dim > 1 ) { 
      m_argDescr += "("; 
      for (int i = 0; i < dim; ++i) 
	m_argDescr += "f";

      m_argDescr += ")";
    } 
    else 
      m_argDescr += "f";

    // add parameter description
    if ( npar > 0) { 
      m_argDescr += " ("; 
      for (int i = 0; i < npar; ++i) 
	m_argDescr += "f"; 
      m_argDescr += ")";
    }
    m_argDescr += ")";

  }

    // dispose 
    PythonFunction::~PythonFunction() { 
      Py_XDECREF(m_func); 
    }

  double PythonFunction::value(const std::vector<double>& d) const 
  {
    unsigned int dim = dimension(); 
    assert ( d.size() >= dim );  
    if (dim == 1)
      return value1D(d[0]); 
    else if (dim ==2)
      return value2D(d[0],d[1]); 
    else if (dim == 3) 
      return value3D(d[0],d[1],d[2]);
    else 
      return 0; 
  }
      
 
  // 1d case
  double   PythonFunction::value1D(double x) const {  
        /* Time to call the callback */
    PyObject * result; 
    // assume only one dim (for the moment) and max 5  params
    PyObject * arglist = 0; 

    //    std::cout << numberOfParameters() << " argDescr = " << m_argDescr << std::endl;
    if ( numberOfParameters() == 0) 
      arglist = Py_BuildValue(const_cast<char *> (m_argDescr.c_str() ), x);
    else if ( numberOfParameters() == 1 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, parameters()[0] );
    else if ( numberOfParameters() == 2 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, parameters()[0], parameters()[1] );
    else if ( numberOfParameters() == 3 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, parameters()[0], parameters()[1], parameters()[2] );
    else if ( numberOfParameters() == 4 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, parameters()[0], parameters()[1], parameters()[2] , parameters()[3] );
    else if ( numberOfParameters() == 5 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, parameters()[0], parameters()[1], parameters()[2] , parameters()[3], parameters()[4] );

    result = PyEval_CallObject(m_func, arglist);
    Py_DECREF(arglist);
    if (result == NULL) 
        return 0; /* Pass error back */
    //...use result...
    double v = PyFloat_AsDouble(result); 
    Py_DECREF(result);
    return v; 
  }

  // 2d case
  double   PythonFunction::value2D(double x, double y) const {  
        /* Time to call the callback */
    PyObject * result; 
    // assume only one dim (for the moment) and max 5  params
    PyObject * arglist = 0; 

    //   std::cout << numberOfParameters() << " argDescr = " << m_argDescr << std::endl;
    if ( numberOfParameters() == 0) 
      arglist = Py_BuildValue(const_cast<char *> (m_argDescr.c_str() ), x, y);
    else if ( numberOfParameters() == 1 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, parameters()[0] );
    else if ( numberOfParameters() == 2 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, parameters()[0], parameters()[1] );
    else if ( numberOfParameters() == 3 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, parameters()[0], parameters()[1], parameters()[2] );
    else if ( numberOfParameters() == 4 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, parameters()[0], parameters()[1], parameters()[2] , parameters()[3] );
    else if ( numberOfParameters() == 5 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, parameters()[0], parameters()[1], parameters()[2] , parameters()[3], parameters()[4] );

    result = PyEval_CallObject(m_func, arglist);
    Py_DECREF(arglist);
    if (result == NULL) 
        return 0; /* Pass error back */
    //...use result...
    double v = PyFloat_AsDouble(result); 
    Py_DECREF(result);
    return v; 
  }

  // 3d case
  double   PythonFunction::value3D(double x, double y, double z) const {  
        /* Time to call the callback */
    PyObject * result; 
    // assume only one dim (for the moment) and max 5  params
    PyObject * arglist = 0; 

    //    std::cout << numberOfParameters() << " argDescr = " << m_argDescr << std::endl;
    if ( numberOfParameters() == 0) 
      arglist = Py_BuildValue(const_cast<char *> (m_argDescr.c_str() ), x, y, z);
    else if ( numberOfParameters() == 1 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, z, parameters()[0] );
    else if ( numberOfParameters() == 2 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, z, parameters()[0], parameters()[1] );
    else if ( numberOfParameters() == 3 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, z, parameters()[0], parameters()[1], parameters()[2] );
    else if ( numberOfParameters() == 4 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, z, parameters()[0], parameters()[1], parameters()[2] , parameters()[3] );
    else if ( numberOfParameters() == 5 ) 
      arglist = Py_BuildValue(const_cast<char *> ( m_argDescr.c_str() ), x, y, z, parameters()[0], parameters()[1], parameters()[2] , parameters()[3], parameters()[4] );

    result = PyEval_CallObject(m_func, arglist);
    Py_DECREF(arglist);
    if (result == NULL) 
        return 0; /* Pass error back */
    //...use result...
    double v = PyFloat_AsDouble(result); 
    Py_DECREF(result);
    return v; 
  }


      



static Util::Plugin<IFMLFunction,PythonFunction> plugin(userFunctionName); 

}
