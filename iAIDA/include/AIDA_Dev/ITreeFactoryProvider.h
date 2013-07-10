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
#ifndef AIDA_ITREEFACTORYPROVIDER
#define AIDA_ITREEFACTORYPROVIDER 1

namespace AIDA {
  class ITreeFactory;

  namespace Dev {

/// Abstract interface class for a tree factory provider

class ITreeFactoryProvider {

public:
  /// Destructor
  virtual ~ITreeFactoryProvider() {/*nop*/;}
  /// Creates a new tree factory
  virtual ITreeFactory* createFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_ITREEFACTORYPROVIDER */
