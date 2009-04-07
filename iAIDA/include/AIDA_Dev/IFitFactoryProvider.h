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

#ifndef AIDA_IFITFACTORYPROVIDER
#define AIDA_IFITFACTORYPROVIDER 1

namespace AIDA {
  class IFitFactory;

  namespace Dev {

/// Abstract interface class for a fit factory provider

class IFitFactoryProvider {

public:
  /// Destructor
  virtual ~IFitFactoryProvider() {/*nop*/;}
  /// Creates a new fit factory
  virtual IFitFactory* createFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IFITFACTORYPROVIDER */
