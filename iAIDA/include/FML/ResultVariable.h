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

# ifndef MINFIT_ResultVariable_h__included__
# define MINFIT_ResultVariable_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Variable.h"

# include "FML/patches.h"

# include <iostream>

namespace fml
{

// namespace FML {

/**
 * Implementation of ResultVariable. 
 * ResultVariable is used to pass back minimizer solution 
 * and associated errors.
 *
 * \ingroup FML_LIB
 */

class ResultVariable : public Variable
{
 public:

  /// Construct default result variable (everything zero or false).
  
  ResultVariable() : m_stderror(), m_minos_minus(), m_minos_plus(), m_minos_available(false) {}

  /// Construct result variable without minos errors.
  ResultVariable(double val, double err) : Variable(val), m_stderror(err), m_minos_minus(), m_minos_plus(), m_minos_available(false) {}

  /// Construct result variable with minos errors available.
  ResultVariable(double val, double err, double mminus, double mplus) : Variable(val), m_stderror(err), m_minos_minus(mminus), m_minos_plus(mplus), m_minos_available(true) {}

  /// Standard error.
  double error() const { return m_stderror; }

  /// Return true if minos errors are available.
  bool minosAvailable() const { return m_minos_available; }
  
  /// Minos error plus.
  double minosErrorPlus() const { return m_minos_minus; }

  /// Minos error minus.
  double minosErrorMinus() const { return m_minos_plus; }


  void tabPrint(std::ostream& os) const;
  static void tabPrintNames(std::ostream& os);

 private:

  double m_stderror, m_minos_minus, m_minos_plus;

  bool m_minos_available;
};

std::ostream& operator << (std::ostream& os, const ResultVariable& v);

// }
}

# endif
