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

# ifndef MINFIT_ScalarModelFunction_h__included__
# define MINFIT_ScalarModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/ModelFunction.h"

namespace FML {

/**
 * Convinience partial implementation of scalar function.
 *
 * Methods dealing directly with double instead of with vector<double>.
 *
 *
 * \ingroup FML_LIB
 */

class ScalarModelFunction : public ModelFunction
{
 public:

  /**
   * Compute the value.
   * Must be overriden. Convinience method.
   */
  virtual double scalarValue(double x) const  = 0;

  /**
   * Compute gradient (if hasGradient()==true).
   * Convinience method.
   */
  virtual double scalarGrad(double x) const { return 0; }

  /**
   * Compute parameter gradient (if hasParameterGradient()==true).
   * Convinience method.
   */
  virtual const vector<double>& scalarParameterGradient(double x) const 
    { 
      return empty_vector; 
    }

  //
  double value(const vector<double>& x) const { return scalarValue(x[0]); }

  //
  const vector<double>& grad(const vector<double>& x) const 
  {
    if(!hasGradient()) return empty_vector;
    m_grad_buf[0] = scalarGrad(x[0]); 
    return m_grad_buf; 
  }

  //
  const vector<double>& parameterGradient(const vector<double>& x) const 
  {
    return scalarParameterGradient(x[0]); 
  }

 protected:

  ScalarModelFunction(int dimx, int parnum, bool has_grad = false, 
		      bool has_param_grad = false) : 
    ModelFunction(dimx,parnum,has_grad, has_param_grad), m_grad_buf(1) {}

 private:

  mutable vector<double> m_grad_buf;
};

}

# endif
