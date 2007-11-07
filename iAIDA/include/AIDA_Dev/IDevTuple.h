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
#ifndef AIDA_IDEVTUPLE
#define AIDA_IDEVTUPLE 1

#include "AIDA/ITuple.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {
    class IBackingStore;
    class ITupleHeader;

/// Developer-level abstact interface for a tuple

class IDevTuple : virtual public ITuple,
		  virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevTuple() {/*nop*/;}

  /// Connects the tuple to a backing store
  virtual bool connectToStore( IBackingStore* store ) = 0;

  /// Returns the pointer to the supporting backing store
  virtual IBackingStore* store() = 0;
  virtual const IBackingStore* store() const = 0;

  /// Returns the header object
  virtual ITupleHeader& header() = 0;
  virtual const ITupleHeader& header() const = 0;

  /// Returns the address of a given variable
  virtual void* variableAddress( const std::string& variableName ) = 0;
  virtual const void* variableAddress( const std::string& variableName ) const = 0;

  /// set address for generic objects
  virtual void setVariableAddress(int column, void * p) = 0;  

};

  }
}

#endif /* ifndef AIDA_IDEVTUPLE */
