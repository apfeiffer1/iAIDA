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
#ifndef AIDA_ISTORE
#define AIDA_ISTORE 1

#include <vector>
#include <string>

namespace AIDA {
  class IManagedObject;

  namespace Dev {

/// Abstract interface class for a store

class IStore {

public:
  /// Destructor
  virtual ~IStore() {/*nop*/;}
  /// Returns the name of the store
  virtual const std::string& name() const = 0;
  /// Writes an object into the persistent store
  virtual bool writeObject( const IManagedObject& dataObject,
                            const std::string& path ) = 0;
  /// Copies and writes and object into the persistent store
  virtual IManagedObject* copyAndWrite( const IManagedObject& dataObject,
					const std::string& path ) = 0;
  /// Retrieves an object from the persistent store
  virtual IManagedObject* retrieveObject( const std::string & path ) = 0;
  /// Removes an object in a given path
  virtual bool removeObject( const std::string& path ) = 0;
  /// Moves (renames) an object from one path to another
  virtual bool moveObject( const std::string& sourcePath, const std::string& targetPath ) = 0;
  /// Commits the transaction in the underlying store
  virtual bool commit() = 0;
  /// Closes the underlying persistent store
  virtual bool close() = 0;
  /// Lists the full paths of all the managed objects
  virtual std::vector< std::string > listObjectPaths( const std::string directory,
                                                      bool recursive = false ) const = 0;
  /// Lists the types of all the managed objects
  virtual std::vector< std::string > listObjectTypes( const std::string directory,
                                                      bool recursive = false ) const = 0;
  /// Checks whether tuples can be moved
  virtual bool canMoveTuples() const = 0;
  /// Checks whether tuples can be copied to
  virtual bool canCopyTuples() const = 0;
};

  }
}

#endif /* ifndef AIDA_ISTORE */
