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

#include "AIDA_Tuple_native/AIDA_DevEvaluatorFactory.h"
#include "CompiledExpressionManager.h"
#include "AIDA_Evaluator.h"

iAIDA::AIDA_Tuple_native::AIDA_DevEvaluatorFactory::AIDA_DevEvaluatorFactory():
  m_mgr( 0 )
{
  m_mgr = new iAIDA::AIDA_Tuple_native::CompiledExpressionManager;
}

iAIDA::AIDA_Tuple_native::AIDA_DevEvaluatorFactory::~AIDA_DevEvaluatorFactory()
{
  if ( m_mgr ) delete m_mgr;
}

AIDA::IEvaluator*
iAIDA::AIDA_Tuple_native::AIDA_DevEvaluatorFactory::createScripted( const std::string & expression )
{
  return new iAIDA::AIDA_Tuple_native::AIDA_Evaluator( *m_mgr, expression );
}
