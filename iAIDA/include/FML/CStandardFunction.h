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

# ifndef FMLLIB_CStandardFunction_h__included__
# define FMLLIB_CStandardFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/ObjectiveFunction.h"

namespace FML
{

/** 
 * Function implemented with a global function pointer with a standard 
 * signature used in Minuit and Nag.
 *
 * This is a wrapper for global function.
 *
 * \ingroup FML_LIB
 */

class CStandardFunction : public ObjectiveFunction
{
public:

  /// Signature of global function that may be captured.
  typedef void (*cscalar_fptr)(int n, double g[], double *objf, const double parms[], int code);
  
  /// Capture global function and specify dimensionality.
  /// If function computes gradient then gradflag must be set true.
  CStandardFunction(cscalar_fptr fptr, int dim, bool gradflag=false) : ObjectiveFunction(dim, gradflag), m_fptr(fptr) {}

  /// Global function call to compute function value.
  double value(const vector<double>& d) const 
    {
      make_call(d);
      return m_value;
    }

  const vector<double>& grad(const vector<double>& d) const
    {
      make_call(d);
      return m_grad_buf;
    }

  /* not needed anymore
  /// Check for equality of two functions.
  /// Objects are assumed equal if they captured the same
  /// function (i.e. if pointers to global function are equal).
  bool isEqual(const IFunction * f) const
  {
    const CStandardFunction *cf = dynamic_cast<const CStandardFunction*>(f);

    return cf ? (m_fptr == cf->m_fptr) : false;
  }
  */

private:

  void make_call(const vector<double>& d) const
    {
      if(d == m_lastarg) return;

      m_lastarg = d;

      // WARNING: contignous storage assumption for vectors

      //      double * gr = const_cast<double*>(&m_grad[0]);

      (*m_fptr)(dimension(), &m_grad_buf[0], &m_value, &m_lastarg[0], hasGradient()?2:0);
    }

  const cscalar_fptr m_fptr;
  
  mutable double m_value;
  //  mutable vector<double> m_grad;
  mutable vector<double> m_lastarg;
};

}

# endif

