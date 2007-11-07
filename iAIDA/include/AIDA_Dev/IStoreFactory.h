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
#ifndef AIDA_ISTOREFACTORY
#define AIDA_ISTOREFACTORY 1

#include <string>

namespace AIDA {
  namespace Dev {
    class IStore;

/// Abstract interface class for a store factory

class IStoreFactory {

public:
  /// Destructor
  virtual ~IStoreFactory() {/*nop*/;}
  /// Creates a new store
  virtual IStore* createStore( const std::string & storeName,
                               bool readOnly,
			       bool createNew,
                               const std::string & options ) = 0;
  /// Deletes a store object
  virtual bool destroyStore( IStore * store ) = 0;
};

  }
}

#endif /* ifndef AIDA_ISTOREFACTORY */
