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

# ifndef MINUITFCNWRAPPER_h__included__
# define MINUITFCNWRAPPER_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Lorenzo.Moneta@cern.ch
//
// CERN  Geneva, 2004
//////////////////////////////////////////

# include "FML/IFMLFunction.h"
# include "Minuit2/FCNBase.h"



namespace fml 
{

/**
 * Wrapper around FML Objective  to be usable with LCG Minuit
 * 
 * \ingroup FML_LIB
 */

class MinuitFCNWrapper : public ROOT::Minuit2::FCNBase
{
public:
  MinuitFCNWrapper() : m_objf(0), m_up(1) {}
  MinuitFCNWrapper(IFMLFunction* f) : m_up(1) { m_objf = f; } 

  // no operation in destructor. Function is not managed by wrapper 
  ~MinuitFCNWrapper()  {} 

  //
  // try to rely on compilers generated assignment
  // how to handle references in operator = ?
  // now I changed to pointer but the question remains open!
  // MinuitFCNWrapper& operator = ( const MinuitFCNWrapper& src );
  //

  double operator()(const std::vector<double> & v) const { return m_objf->value(v); }

  double  Up() const { return m_up; }

  void setError( double errorUp ) { m_up = errorUp; }

private:
  IFMLFunction *m_objf;
  // vector of fixed parameters (for Minuit to FML convention ) 
  //std::vector<bool> m_par_isFixed; 
  // vector of initial values (to be used when one parameter is fixed)
  //std::vector<double> m_par_initValues; 
  // number of free parameters 
  //int m_numbOfFreeParams;
  double m_up;

};

}

# endif
