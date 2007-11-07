// Emacs -*- C++ -*-
#ifndef DUMMYSTORE
#define DUMMYSTORE 1

#include <iosfwd>
#include <vector>
#include <string>
#include <set>
#include "AIDA_Dev/IStore.h"

// type ahead declarations 
namespace AIDA {
  class IManagedObject;
  class ITuple;
}

namespace iAIDA {
  namespace aida_tree {

class DummyStore : virtual public AIDA::Dev::IStore {

public:
  /// Constructor
  DummyStore( const std::string& name, std::ostream& os );
  /// Destructor
  ~DummyStore();

  /// Methods inherited from the IStore class
  const std::string& name() const;
  bool writeObject( const AIDA::IManagedObject& dataObject,
		    const std::string& path );
  AIDA::IManagedObject* copyAndWrite( const AIDA::IManagedObject& dataObject,
				      const std::string& path );
  AIDA::IManagedObject* retrieveObject( const std::string & path );
  bool removeObject( const std::string& path );
  bool moveObject(const std::string& from, const std::string& to );
  bool commit();
  bool close();
  std::vector< std::string > listObjectPaths( const std::string directory,
					      bool recursive = false ) const;
  std::vector< std::string > listObjectTypes( const std::string directory,
					      bool recursive = false ) const;
  bool canMoveTuples() const;
  bool canCopyTuples() const;

private:
  std::string             m_name;
  std::ostream&           m_os;
  std::set< std::string > m_objects;
};

  }
}

#endif /* ifndef DUMMYSTORE */
