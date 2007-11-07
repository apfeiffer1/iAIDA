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

#ifndef AIDA_DEV_IFUNCTIONFACTORYPROVIDER_H
#define AIDA_DEV_IFUNCTIONFACTORYPROVIDER_H

namespace AIDA {
  class IFunctionFactory;

  namespace Dev {
    class IDevFunctionFactory;
    class IDevTree;

/// Abstract interface class for a function factory provider

class IFunctionFactoryProvider {

public:
  /// Destructor
  virtual ~IFunctionFactoryProvider() {/*nop*/;}
  /// Creates a new function factory
  virtual IFunctionFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level function factory which creates unmanaged objects
  virtual IDevFunctionFactory& devFunctionFactory() = 0;
};

  }
}


#endif
