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
#ifndef AIDA_IDEVMANAGEDOBJECT
#define AIDA_IDEVMANAGEDOBJECT 1

#include "AIDA/IManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a manageable/storeable object

class IDevManagedObject : virtual public IManagedObject {

public:
  /// Destructor
  virtual ~IDevManagedObject() {/*nop*/;}

  /// Checks if the object is up-to-date with respect to its stored image
  virtual bool isUpToDate() const = 0;

  /// The up-to-date flag
  virtual void setUpToDate( bool isUpToDate ) = 0;

  /// Sets the name of the object
  virtual bool setName( const std::string& newName ) = 0;

  /// The AIDA user-level unterface leaf class type
  virtual const std::string& userLevelClassType() const = 0;
};

  }
}
#endif /* ifndef AIDA_IDEVMANAGEDOBJECT */
