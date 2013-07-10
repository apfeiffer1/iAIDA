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
#ifndef AIDA_IHISTOGRAMFACTORYPROVIDER
#define AIDA_IHISTOGRAMFACTORYPROVIDER 1

namespace AIDA {
  class IHistogramFactory;

  namespace Dev {
    class IDevHistogramFactory;
    class IDevTree;

/// Abstract interface class for a histogram factory provider

class IHistogramFactoryProvider {

public:
  /// Destructor
  virtual ~IHistogramFactoryProvider() {/*nop*/;}
  /// Creates a new histogram factory
  virtual IHistogramFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level histogram factory which creates unmanaged objects
  virtual IDevHistogramFactory& devHistogramFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IHISTOGRAMFACTORYPROVIDER */
