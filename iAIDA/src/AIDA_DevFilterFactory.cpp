#include "AIDA_Tuple_native/AIDA_DevFilterFactory.h"
#include "CompiledExpressionManager.h"
#include "AIDA_Filter.h"
#include "AIDA_FilterRange.h"


iAIDA::AIDA_Tuple_native::AIDA_DevFilterFactory::AIDA_DevFilterFactory():
  m_mgr( 0 )
{
  m_mgr = new iAIDA::AIDA_Tuple_native::CompiledExpressionManager;
}

iAIDA::AIDA_Tuple_native::AIDA_DevFilterFactory::~AIDA_DevFilterFactory()
{
  if ( m_mgr ) delete m_mgr;
}


AIDA::IFilter*
iAIDA::AIDA_Tuple_native::AIDA_DevFilterFactory::createScripted( const std::string & expression )
{
  return new iAIDA::AIDA_Tuple_native::AIDA_Filter( *m_mgr, expression );
}

AIDA::IFilter*
iAIDA::AIDA_Tuple_native::AIDA_DevFilterFactory::createScripted( const std::string & expression,
								  int rowsToProcess,
								  int startingRow )
{
  return new iAIDA::AIDA_Tuple_native::AIDA_FilterRange( *m_mgr, expression, rowsToProcess, startingRow );
}
