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
#ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLEDESCRIPTION
#define IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLEDESCRIPTION 1

#include "AIDA_Dev/ITupleVariableDescription.h"
#include <map>
#include <vector>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_TupleVariableDescription : virtual public AIDA::Dev::ITupleVariableDescription {

public:
  /// Constructor
  AIDA_TupleVariableDescription();
  /// Destructor
  ~AIDA_TupleVariableDescription();

  /// Methods inherited from the AIDA::Dev::ITupleVariableDescription class
  const std::string& variableName() const;
  bool setVariableName( const std::string& name );
  const std::string& variableType() const;
  bool setVariableType( const std::string& type );
  const AIDA::Dev::ITupleVariableStatistics& statistics() const;
  AIDA::Dev::ITupleVariableStatistics& statistics();
  bool resetStatistics();

  int numberOfVariables() const;
  bool setVariableDescription( AIDA::Dev::ITupleVariableDescription* description,
			       bool passOwnership );
  AIDA::Dev::ITupleVariableDescription* variableDescription( int subVariableIndex );
  const AIDA::Dev::ITupleVariableDescription* variableDescription( int subVariableIndex ) const;
  int variableIndex( const std::string& variableName ) const;

private:
  std::string                                                           m_name;
  std::string                                                           m_type;
  std::vector< std::pair<AIDA::Dev::ITupleVariableDescription*, bool> > m_variableDescriptions;
  std::map< std::string, int >                                          m_variableIndices;
  AIDA::Dev::ITupleVariableStatistics*                                  m_statistics;

  // no copy constructor or assignment operator
  AIDA_TupleVariableDescription( const AIDA_TupleVariableDescription& );
  AIDA_TupleVariableDescription& operator=( const AIDA_TupleVariableDescription& );
};

  }
}

#include "AIDA_TupleVariableDescription.inl"

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLEDESCRIPTION */
