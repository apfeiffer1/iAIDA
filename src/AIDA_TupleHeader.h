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

// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEHEADER
#define IAIDA_AIDA_TUPLE_AIDA_TUPLEHEADER 1

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

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEHEADER */
