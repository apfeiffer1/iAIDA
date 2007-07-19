// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEHEADER
#define AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEHEADER 1

#include "AIDA_Dev/ITupleHeader.h"
#include <map>
#include <vector>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_TupleHeader : virtual public AIDA::Dev::ITupleHeader {
public:
  AIDA_TupleHeader();
  ~AIDA_TupleHeader();

  /// Methods inherited from AIDA::Dev::ITupleHeader
  AIDA::IAnnotation& annotation();
  const AIDA::IAnnotation& annotation() const;
  bool setVariableDescription( AIDA::Dev::ITupleVariableDescription* description,
			       bool passOwnership );
  const AIDA::Dev::ITupleVariableDescription* variableDescription( int index ) const;
  AIDA::Dev::ITupleVariableDescription* variableDescription( int index );
  int variableIndex( const std::string& variableName ) const;
  int numberOfVariables() const;
  bool setPathInStore( const std::string& path );
  const std::string& pathInStore() const;
  bool setNumberOfRows( int rows );
  int numberOfRows() const;
  bool setCurrentRowNumber( int rowNumber );
  int currentRowNumber() const;
  bool setOptions( const std::string& options );
  const std::string& options() const;

private:
  AIDA::IAnnotation*                                                    m_annotation;
  int                                                                   m_numberOfRows;
  int                                                                   m_currentRowNumber;
  std::string                                                           m_pathInStore;
  std::vector< std::pair<AIDA::Dev::ITupleVariableDescription*, bool> > m_variableDescriptions;
  std::map< std::string, int >                                          m_variableIndices;
  std::string                                                           m_options;

  // private methods
  AIDA::IAnnotation*  createAnnotation();

  // No copy constructor or assignment operator
  AIDA_TupleHeader( const AIDA_TupleHeader& );
  AIDA_TupleHeader& operator=( const AIDA_TupleHeader& );

};

  }
}

#include "AIDA_TupleHeader.inl"

#endif /* ifndef AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEHEADER */
