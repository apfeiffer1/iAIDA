#include "AIDA_TupleVariableDescription.h"
#include "AIDA_TupleVariableStatistics.h"

iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::AIDA_TupleVariableDescription():
  m_name(""),
  m_type(""),
  m_variableDescriptions(),
  m_variableIndices(),
  m_statistics( 0 )
{
  m_statistics = new iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics;
}


iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::~AIDA_TupleVariableDescription()
{
  for ( unsigned int i = 0; i < m_variableDescriptions.size(); ++i ) {
    if ( m_variableDescriptions[i].second ) delete m_variableDescriptions[i].first;
  }

  if ( m_statistics ) delete m_statistics;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::setVariableDescription( AIDA::Dev::ITupleVariableDescription* description,
										  bool passOwnership )
{
  std::map< std::string, int >::iterator iVariable = m_variableIndices.find( description->variableName() );
  if ( iVariable != m_variableIndices.end() ) {
    if ( m_variableDescriptions[ iVariable->second ].second ) delete m_variableDescriptions[ iVariable->second ].first;
    m_variableDescriptions[ iVariable->second ].first = description;
    m_variableDescriptions[ iVariable->second ].second = passOwnership;
  }
  else {
    m_variableIndices.insert( std::make_pair( description->variableName(), static_cast<int>(m_variableDescriptions.size()) ) );
    m_variableDescriptions.push_back( std::make_pair( description, passOwnership ) );
  }
  return true;
}


AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::variableDescription( int subVariableIndex )
{
  if ( subVariableIndex < 0 || subVariableIndex >= static_cast<int>(m_variableDescriptions.size()) ) return 0;
  return m_variableDescriptions[subVariableIndex].first;
}


const AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::variableDescription( int subVariableIndex ) const
{
  if ( subVariableIndex < 0 || subVariableIndex >= static_cast<int>(m_variableDescriptions.size()) ) return 0;
  return m_variableDescriptions[subVariableIndex].first;
}


int
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::variableIndex( const std::string& variableName ) const
{
  std::map< std::string, int >::const_iterator i = m_variableIndices.find( variableName );
  if ( i == m_variableIndices.end() ) return -1;
  else return i->second;
}


const AIDA::Dev::ITupleVariableStatistics&
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::statistics() const
{
  return *m_statistics;
}


AIDA::Dev::ITupleVariableStatistics&
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::statistics()
{
  return *m_statistics;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::resetStatistics()
{
  return m_statistics->reset();
}
