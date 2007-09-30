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
#ifndef IAIDA_AIDA_TUPLE_DEVFILTERFACTORY
#define IAIDA_AIDA_TUPLE_DEVFILTERFACTORY 1

#include "AIDA_Dev/IDevFilterFactory.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class CompiledExpressionManager;

/// Implementation of the AIDA::Dev::IDevFilterFactory

class AIDA_DevFilterFactory : virtual public AIDA::Dev::IDevFilterFactory
{
public:
  /// Constructor
  AIDA_DevFilterFactory();

  /// Destructor
  ~AIDA_DevFilterFactory();

  /// Creates a filter given a C++ expression
  AIDA::IFilter* createScripted( const std::string & expression );

  /** Creates a filter given a C++ expression,
      the number of tuple rows to process and the starting row. */
  AIDA::IFilter* createScripted( const std::string & expression,
				 int rowsToProcess,
				 int startingRow = 0 );

private:
  CompiledExpressionManager*   m_mgr;

};

  }
}

#endif
