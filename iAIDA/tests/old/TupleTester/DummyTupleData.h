#ifndef DUMMYTUPLEDATA_H
#define DUMMYTUPLEDATA_H

#include <map>
#include <string>
#include <vector>

namespace AIDA {
  namespace Dev {
    class ITupleVariableDescription;
  }
}


class DummyTupleData
{
public:
  DummyTupleData();
  ~DummyTupleData();

  const std::string& title() const { return m_title; }
  void title( const std::string t ) { m_title = t; }

  const std::vector< AIDA::Dev::ITupleVariableDescription * >& variables() const { return m_variables; }
  std::vector< AIDA::Dev::ITupleVariableDescription * >& variables() { return m_variables; }

  const std::map< int, std::vector< double > >& doubleVariableData() const { return m_doubleVariableData; }
  std::map< int, std::vector< double > >& doubleVariableData() { return m_doubleVariableData; }

  const std::map< int, std::vector< float > >& floatVariableData() const { return m_floatVariableData; }
  std::map< int, std::vector< float > >& floatVariableData() { return m_floatVariableData; }

  const std::map< int, std::vector< long > >& longVariableData() const { return m_longVariableData; }
  std::map< int, std::vector< long > >& longVariableData() { return m_longVariableData; }

  const std::map< int, std::vector< int > >& intVariableData() const { return m_intVariableData; }
  std::map< int, std::vector< int > >& intVariableData() { return m_intVariableData; }

  const std::map< int, std::vector< short > >& shortVariableData() const { return m_shortVariableData; }
  std::map< int, std::vector< short > >& shortVariableData() { return m_shortVariableData; }

  const std::map< int, std::vector< char > >& charVariableData() const { return m_charVariableData; }
  std::map< int, std::vector< char > >& charVariableData() { return m_charVariableData; }

  const std::map< int, std::vector< bool > >& boolVariableData() const { return m_boolVariableData; }
  std::map< int, std::vector< bool > >& boolVariableData() { return m_boolVariableData; }

  const std::map< int, std::vector< std::string > >& stringVariableData() const { return m_stringVariableData; }
  std::map< int, std::vector< std::string > >& stringVariableData() { return m_stringVariableData; }

  const std::map< int, std::vector< DummyTupleData* > >& tupleVariableData() const { return m_tupleVariableData; }
  std::map< int, std::vector< DummyTupleData* > >& tupleVariableData() { return m_tupleVariableData; }

private:
  std::string                                           m_title;
  std::vector< AIDA::Dev::ITupleVariableDescription * > m_variables;
  std::map< int, std::vector< double > >                m_doubleVariableData;
  std::map< int, std::vector< float > >                 m_floatVariableData;
  std::map< int, std::vector< long > >                  m_longVariableData;
  std::map< int, std::vector< int > >                   m_intVariableData;
  std::map< int, std::vector< short > >                 m_shortVariableData;
  std::map< int, std::vector< char > >                  m_charVariableData;
  std::map< int, std::vector< bool > >                  m_boolVariableData;
  std::map< int, std::vector< std::string > >           m_stringVariableData;
  std::map< int, std::vector< DummyTupleData* > >       m_tupleVariableData;
};


#endif
