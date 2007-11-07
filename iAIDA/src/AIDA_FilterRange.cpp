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

#include "AIDA_FilterRange.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include <typeinfo>
#include <cmath>

iAIDA::AIDA_Tuple_native::AIDA_FilterRange::AIDA_FilterRange( CompiledExpressionManager& manager,
							       const std::string& expression,
							       int numberOfRows,
							       int startingRow ):
  iAIDA::AIDA_Tuple_native::AIDA_Filter( manager, expression ),
  m_startingRow( std::abs( startingRow ) ),
  m_endingRow( m_startingRow + std::abs( numberOfRows ) -1 ),
  m_header( 0 )
{}


bool
iAIDA::AIDA_Tuple_native::AIDA_FilterRange::initialize( AIDA::ITuple & tuple )
{
  if ( ! this->iAIDA::AIDA_Tuple_native::AIDA_Filter::initialize( tuple ) ) return false;
  try{
    AIDA::Dev::IDevTuple& tp = dynamic_cast<AIDA::Dev::IDevTuple&>( tuple );
    m_header = &( tp.header() );
    return true;
  }
  catch( std::bad_cast ) {
    return false;
  }
}


bool
iAIDA::AIDA_Tuple_native::AIDA_FilterRange::accept() const
{
  if ( ! m_header ) return false;
  const int currentRow = m_header->currentRowNumber();
  if ( currentRow < m_startingRow || currentRow > m_endingRow ) return false;
  return this->iAIDA::AIDA_Tuple_native::AIDA_Filter::accept();
}
