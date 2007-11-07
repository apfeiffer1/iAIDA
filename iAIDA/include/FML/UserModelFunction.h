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

# ifndef FMLLIB_GenericFunction_h__included__
# define FMLLIB_GenericFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"

namespace fml {
// namespace FML {

/** 
 * Scalar function implemented with a global function pointer.
 *
 * This is a wrapper for global functions.
 *
 * \ingroup FML_LIB
 */

class UserModelFunction : public Function
{
public:

  /// Signature of global function that may be captured.
  typedef double (*Fun_ptr)(const std::vector<double>& x, const std::vector<double> & p);
  
  // needed for plugin manager
  UserModelFunction();

  /// Capture global function and specify dimensionality.
  UserModelFunction(Fun_ptr fptr, int dim, int npar);

  /// Global function call to compute function value.
  double value(const std::vector<double>& d) const 
  {
    return (*m_fptr)(d,parameters());
  }

 /// Global function call to compute function value.
  double value(const std::vector<double>& d, const std::vector<double>& p ) const 
  {
    return (*m_fptr)(d,p);
  }



  /// Check for equality of two functions.
  /// UserModelFunction objects are assumed equal if they captured the same
  /// function (i.e. if pointers to global function are equal).
  /// It is not possible nor feasible to check if two different
  /// global functions give the same results (are mathematicaly equal).
  bool isEqual(const IFMLFunction * f) const
  {

    /// PENDING: dynamic cast does not work on Solaris

# ifdef FML_SOLARIS_COMPILER
    return false;
# else
    const UserModelFunction *cf = dynamic_cast<const UserModelFunction*>(f);
    return cf ? (m_fptr == cf->m_fptr) : false;
# endif
  }



private:

  Fun_ptr m_fptr;
};

// }
}
# endif
