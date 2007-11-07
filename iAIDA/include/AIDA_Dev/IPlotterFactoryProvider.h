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

#ifndef AIDA_IPLOTTERFACTORYPROVIDER
#define AIDA_IPLOTTERFACTORYPROVIDER 1

namespace AIDA {
  class IPlotterFactory;

  namespace Dev {

/// Abstract interface class for a plotter factory provider

class IPlotterFactoryProvider {

public:
  /// Destructor
  virtual ~IPlotterFactoryProvider() {/*nop*/;}
  /// Creates a new plotter factory
  virtual IPlotterFactory* createFactory( int argc = 0, char** args = 0 ) = 0;
};

  }
}

#endif /* ifndef AIDA_IPLOTTERFACTORYPROVIDER */
