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

# include "FML/MinimizerControlParams.h"

namespace fml
{

// namespace FML {

  MinimizerControlParams::MinimizerControlParams() : m_maxit(),  m_strategy() , m_tol(), m_prec(), m_up()
{

  useImplicitMaxIter();
  useImplicitTolerance();
  useImplicitPrecision();
  useImplicitErrorUP();
  useImplicitStrategy();
}


  /*
const int MinimizerControlParams::MAXITER_DEFAULT = 100;

// from Minuit (experimental) default
const double MinimizerControlParams::PRECISION_DEFAULT = 1.0e-15; 

// from NAG specification (prec^0.8)
const double MinimizerControlParams::TOLERANCE_DEFAULT = 6.3e-12;  

// PENDING: or maybe should be 0.5 ?
const double MinimizerControlParams::ERRORUP_DEFAULT = 1.0;
  */
// }
}
