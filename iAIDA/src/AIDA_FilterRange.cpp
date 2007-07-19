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
