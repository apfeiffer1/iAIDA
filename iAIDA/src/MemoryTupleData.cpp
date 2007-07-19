#include "AIDA_MemoryStore/MemoryTupleData.h"
#include "AIDA_Dev/ITupleVariableDescription.h"

iAIDA::AIDA_MemoryStore::MemoryTupleData::MemoryTupleData():
  m_title( "" ),
  m_variables()
{
  m_variables.reserve( 1000 );
}


iAIDA::AIDA_MemoryStore::MemoryTupleData::~MemoryTupleData()
{
  for ( unsigned int i = 0; i < m_variables.size(); ++i ) {
    delete m_variables[i];
  }

  for( std::map< int, std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* > >::iterator i = m_tupleVariableData.begin();
       i != m_tupleVariableData.end(); ++i ) {
    std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >& vec = i->second;
    for ( std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator j = vec.begin(); j != vec.end(); ++j ) {
      if ( *j ) delete *j;
    }
  }
}
