#ifndef USERSFUNCTIONS_USERFUNCTION_H
#define USERSFUNCTIONS_USERFUNCTION_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include "Python.h"


#include <string>
#include <vector>


#include "AIDA_Proxy/Function.h"



namespace iAIDA_aida {

  /// Signature of global function that may be captured.
  typedef double (* FreeFun)(const std::vector<double>& x, const std::vector<double> & p);


  /** 
   * Extension to pi_aida Function to use user defined function:
   *
   * The user defined function can be constructed from: 
   *     - a pointer to a free function of a vector of coordinate or parameter 
   *     - a boost function object build from a vector of coordinate and parameter
   *     - a Python callable object for an interpreted function 
   *
   *
   * NOTE: this function are created directly and are not managed for the moment
   */ 

  class UserFunction : virtual public Function 
{ 

  public: 

    /* 
     * create a function from a pointer to free function 
     */
  
    UserFunction(FreeFun func, int dim, int npar, const std::string&  name = ""); 

    /* 
     *  create a function from a Python function
     */

    UserFunction(PyObject * func, int dim, int npar, const std::string& name = ""); 

    // destructor 
    ~UserFunction(); 

  };

} // end namespace


#endif
