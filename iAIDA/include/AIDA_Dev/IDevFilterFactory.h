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
#ifndef AIDA_IDEVFILTERFACTORY
#define AIDA_IDEVFILTERFACTORY 1

#include <string>

namespace AIDA {
  class IFilter;

  namespace Dev {

/// Abstract interface class for a factory which creates filter objects

class IDevFilterFactory
{
public:
  /// Destructor
  virtual ~IDevFilterFactory() {/*nop*/;}

  /// Creates a filter given a C++ expression
  virtual IFilter* createScripted( const std::string & expression ) = 0;

  /** Creates a filter given a C++ expression,
      the number of tuple rows to process and the starting row. */
  virtual IFilter* createScripted( const std::string & expression,
				   int rowsToProcess,
				   int startingRow = 0 ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVFILTERFACTORY */
