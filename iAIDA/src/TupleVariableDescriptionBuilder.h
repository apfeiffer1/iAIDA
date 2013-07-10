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
#ifndef IAIDA_AIDA_TUPLE_TUPLEVARIABLEDECSRIPTIONBUILDER_H
#define IAIDA_AIDA_TUPLE_TUPLEVARIABLEDECSRIPTIONBUILDER_H 1

#include <vector>
#include <list>
#include <string>

#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/IDevTupleFactory.h"

namespace AIDA {
  class ITuple; 
  namespace Dev {
    class ITupleHeader;
  }
}

namespace iAIDA {
  namespace AIDA_Tuple_native {

class TupleVariableDescriptionBuilder
{
public:
  /// Constructor
  TupleVariableDescriptionBuilder( AIDA::Dev::IDevTupleFactory& factory );
  /// Destructor
  ~TupleVariableDescriptionBuilder() {}

  /// Builds a description from a string
  bool buildDescription( AIDA::Dev::ITupleHeader& header,
			 const std::string& descriptionString );

  /// Builds a description from two vectors (name, type)
  bool buildDescription( AIDA::Dev::ITupleHeader& header,
			 const std::vector<std::string>& variableNames,
			 const std::vector<std::string>& variableTypes );

  /***** HELPER METHODS *****/
  /// Builds a description from a string
  bool buildDescription( AIDA::Dev::ITupleVariableDescription& description,
			 const std::string& descriptionString );

  /// Builds a description from two vectors (name, type)
  bool buildDescription( AIDA::Dev::ITupleVariableDescription& description,
			 const std::vector<std::string>& variableNames,
			 const std::vector<std::string>& variableTypes );

  /// Decomposes a string into vectors of types and names
  bool decompose( const std::string& input,
		  std::vector<std::string>& variableNames,
		  std::vector<std::string>& variableTypes ) const;

  /// method to compose a description string from a tuple 
  /// inverse operation 

  static bool compose (const AIDA::ITuple & tp,  std::string& descriptionString ); 

private:
  /// The factory used to create description objects for sub-tuples
  AIDA::Dev::IDevTupleFactory&   m_factory;

  /// Template method to build the description,
  template< class T > bool buildDescriptionT( T& t,
					      const std::vector<std::string>& variableNames,
					      const std::vector<std::string>& variableTypes );
};

  }
}

#include "TupleVariableDescriptionBuilder.templ"

#endif /* ifndef IAIDA_AIDA_TUPLE_TUPLEVARIABLEDECSRIPTIONBUILDER_H */
