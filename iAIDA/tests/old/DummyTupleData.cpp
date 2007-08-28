#include "DummyTupleData.h"
#include "AIDA_Dev/ITupleVariableDescription.h"

DummyTupleData::DummyTupleData():
  m_title( "" ),
  m_variables()
{
  m_variables.reserve( 1000 );
}


DummyTupleData::~DummyTupleData()
{
  for ( unsigned int i = 0; i < m_variables.size(); ++i ) {
    delete m_variables[i];
  }

  for( std::map< int, std::vector< DummyTupleData* > >::iterator i = m_tupleVariableData.begin();
       i != m_tupleVariableData.end(); ++i ) {
    std::vector< DummyTupleData* >& vec = i->second;
    for ( std::vector< DummyTupleData* >::iterator j = vec.begin(); j != vec.end(); ++j ) {
      if ( *j ) delete *j;
    }
  }
}
