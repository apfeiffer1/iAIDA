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
