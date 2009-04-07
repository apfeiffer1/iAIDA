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

# include "FML/MinimizerResult.h"


namespace fml
{

// namespace FML {
 
MinimizerResult::MinimizerResult(int ndim, std::vector<ResultVariable> res, double up, double value, const FMLMatrix& cov, bool minos/*, GEMinimizer *gm*/) : m_dim(ndim), m_variables(res), m_errorup(up), m_covmatrix(cov), m_minvalue(value), m_isvalid(true), m_minos(minos)//, m_geminimizer(gm)
{}

MinimizerResult::MinimizerResult() : m_dim(0), m_variables(), m_errorup(0.0), m_covmatrix(), m_minvalue(0.0), m_isvalid(false), m_minos(false)
{}

MinimizerResult::~MinimizerResult()
{
}

int MinimizerResult::dimension() const 
{
  return m_dim;
}

const std::vector<ResultVariable>& MinimizerResult::variables() const
{
  return m_variables;
}


double MinimizerResult::errorUP() const
{
  return m_errorup;
}


const FMLMatrix& MinimizerResult::covMatrix() const
{
  return m_covmatrix;
}
  
 
/*const IVector**/ Contour MinimizerResult::ellipticalContour(int , int ) const
{
  
  return Contour();
}

/*const IVector**/ Contour MinimizerResult::minosContour(int , int ) const
{

  return Contour();
}

// }
}
