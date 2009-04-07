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
#ifndef AIDA_IDATAPOINTSETFACTORYPROVIDER
#define AIDA_IDATAPOINTSETFACTORYPROVIDER 1

namespace AIDA {
  class IDataPointSetFactory;

  namespace Dev {
    class IDevDataPointSetFactory;
    class IDevTree;

/// Abstract interface class for a datapointset factory provider

class IDataPointSetFactoryProvider {

public:
  /// Destructor
  virtual ~IDataPointSetFactoryProvider() {/*nop*/;}
  /// Creates a new dataPointSet factory
  virtual IDataPointSetFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level dataPointSet factory which creates unmanaged objects
  virtual IDevDataPointSetFactory& devDataPointSetFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IDATAPOINTSETFACTORYPROVIDER */
