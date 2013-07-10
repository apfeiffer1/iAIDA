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
#ifndef AIDA_IDEVTREE
#define AIDA_IDEVTREE 1

#include <string>
#include "AIDA/ITree.h"

namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IStore;

/// Developer-level interface for a tree

class IDevTree : virtual public ITree {

public:
  /// Destructor
  virtual ~IDevTree() {/*nop*/;}
  /// Adds an object in the logical structure
  virtual bool add( IManagedObject * object,
                    const std::string& directory = "." ) = 0;
  /// Adds an object in the logical structure performing a copy first
  virtual bool copyAndAdd( IManagedObject * object,
			   const std::string& newPath ) = 0;
  /// Returns the address to the underlying store given a path
  virtual IStore* store( const std::string& path = "." ) = 0;
  /// Returns the native store
  virtual IStore* nativeStore() = 0;
  /// Returns the path in the store
  virtual std::string pathInStore( const IManagedObject& object ) const = 0;
  /// Checks if a directory exists
  virtual bool existsDirectory( const std::string& directory ) const = 0;
  /**
   * Internal operations between trees during mounting/unmounting.
   * A method should check whether a tree is mounted. If not, the
   * mounting can proceed. Then the parent tree sends its address to
   * the daughter tree. When the parent tree is deleted or unmounts the
   * daughter tree, it calls the setParentTree() of the daughter passing
   * 0 as argument. If the daughter tree is deleted while being mounted
   * by another, it uses the registered address to call its parent to
   * unmount it.
   */
  virtual bool isMounted() const = 0;
  virtual bool setParentTree( IDevTree * parentTree ) = 0;
  virtual bool unmountTree( IDevTree * daughterTree ) = 0;
};

  }
}

#endif /* ifndef AIDA_ISTORE */
