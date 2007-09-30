// Emacs -*- C++ -*-
#ifndef IAIDA_SUBTREE
#define IAIDA_SUBTREE 1

#include <string>
#include <map>
#include <list>
#include <vector>

#include "AIDA/IManagedObject.h"

// type ahead declarations 
namespace AIDA {
  class ITree;

  namespace Dev {
    class IDevTree;
    class IStore;
  }
}


namespace iAIDA {
  namespace aida_tree {
    class MountInfo;

/// Class which describes a node in a tree

class SubTree {

public:
  /// Constructor
  SubTree( const std::string& name,
	   AIDA::Dev::IDevTree* );
  /// Destructor
  ~SubTree();
  /// Add method
  bool add( AIDA::IManagedObject * object,
	    std::list< std::string >& path,
	    bool overWrite,
            std::map< const AIDA::IManagedObject*, std::string >& objectMap,
            bool copy,
            bool writeInStore = true );
  /// find method
  AIDA::IManagedObject* find( std::list< std::string >& path,
			      std::map< const AIDA::IManagedObject*, std::string >& objectMap );
  /// find mount Tree (from AIDA 3.2) 
  AIDA::ITree* findTree( std::list< std::string >& path,
		std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints );
  AIDA::IManagedObject* findAndErase( std::list< std::string >& path );
  /// finds the store for a given directory or object
  AIDA::Dev::IStore* store( std::list< std::string >& path );
  /// Checks the existence of a directory
  bool existsDirectory( std::list< std::string >& path );
  /// makes a new directory
  bool mkdir( std::list< std::string >& path );
  bool mkdirs( std::list< std::string >& path );
  /// removes a directory
  bool rmdir( std::list< std::string >& path );
  /// removes an object
  bool rm( std::list< std::string >& path,
	   std::map< const AIDA::IManagedObject*, std::string >& objectMap );
  /// creates a symbolic link
  bool symlink( const std::string& path,
		std::list< std::string >& alias );
  /// mounts a tree
  bool mount( const std::string & path,
	      AIDA::Dev::IDevTree* tree,
	      std::list< std::string >& treePath,
              std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints );
  /// unmounts a tree
  bool unmount( std::list< std::string >& path,
		std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints );
  /// lists the object names under this node
  std::vector< std::string > listObjectNames( std::list< std::string >& path,
                                              bool recursive ) const;
  /// lists the type names of the objects under this node
  std::vector< std::string > listObjectTypes( std::list< std::string >& path,
                                              bool recursive ) const;

private:
  /// Full path of the node
  std::string                                                    m_name;
  /// Pointer to the master tree
  AIDA::Dev::IDevTree*                                           m_masterTree;
  /// Pointer to the mount summary information (if the node is a mount point)
  iAIDA::aida_tree::MountInfo*                                             m_mountInfo;
  /// Dependent nodes (subdirectories)
  std::map< std::string, SubTree* >                              m_subTrees;
  /// Dependend objects
  std::map< std::string, AIDA::IManagedObject* >                 m_objects;
  /// Dependent links
  std::map< std::string, std::string >                           m_links;

  // Forbidden : copy constructor and assignment operator
  SubTree( const SubTree& );
  SubTree& operator=( const SubTree& );
};

  }
}

#endif /* ifndef IAIDA_SUBTREE */
