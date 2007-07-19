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
