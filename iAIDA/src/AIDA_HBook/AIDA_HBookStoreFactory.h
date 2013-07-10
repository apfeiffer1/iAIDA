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
#ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY
#define IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY 1

#include <string>
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_HBookStore {

/// Implementation of AIDA::DEV::IStoreFactory interface for a factory of hbook stores

class AIDA_HBookStoreFactory : virtual public AIDA::Dev::IStoreFactory
{
public:
  /// Constructor
  AIDA_HBookStoreFactory();
  /// Destructor
  ~AIDA_HBookStoreFactory();

  /// Method inherited from the IStoreFactory class
  AIDA::Dev::IStore* createStore( const std::string & storeName,
				  bool readOnly,
				  bool createNew,
				  const std::string & options );
  bool destroyStore( AIDA::Dev::IStore* store );
};

  }
}

#endif /* ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY */
