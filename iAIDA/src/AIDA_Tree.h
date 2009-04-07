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
#ifndef IAIDA_TREE
#define IAIDA_TREE 1

#include <vector>
#include <string>
#include <map>
#include <set>
#include "AIDA_Dev/IDevTree.h"

// type ahead declarations 
namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IStore;
  }
}


namespace iAIDA {
  namespace aida_tree {
    class SubTree;
    class MountInfo;

/// Implementation of the AIDA::ITree abstract interface

class AIDA_Tree : virtual public AIDA::Dev::IDevTree
{
public:
  /// Constructor
  AIDA_Tree( AIDA::Dev::IStore* store );
  /// Destructor
  ~AIDA_Tree();

  /// Methods inherited from the IDevTree class
  bool add( AIDA::IManagedObject * object,
	    const std::string& directory = "." );
  bool copyAndAdd( AIDA::IManagedObject * object,
		   const std::string& newPath );
  AIDA::Dev::IStore* store( const std::string& path = "." );
  AIDA::Dev::IStore* nativeStore();
  std::string pathInStore( const AIDA::IManagedObject& object ) const;
  bool existsDirectory( const std::string& directory ) const;
  bool isMounted() const;
  bool setParentTree( AIDA::Dev::IDevTree * parentTree );
  bool unmountTree( AIDA::Dev::IDevTree * daughterTree );

  /// Methods inherited from the ITree class
  std::string  storeName() const;
  AIDA::IManagedObject * find( const std::string & path );
  AIDA::ITree * findTree(const std::string & path);
  bool cd( const std::string & path );
  std::string pwd() const;
  bool ls( const std::string & path = ".",
	   bool recursive = false,
	   std::ostream & os = std::cout ) const;
  std::vector< std::string > listObjectNames( const std::string & path = ".",
					      bool recursive = false ) const;
  std::vector< std::string > listObjectTypes( const std::string & path = ".",
					      bool recursive = false ) const;
  bool mkdir( const std::string & path );
  bool mkdirs( const std::string & path );
  bool rmdir( const std::string & path );
  bool rm( const std::string & path );
  std::string findPath( const AIDA::IManagedObject & object ) const;
  bool mv( const std::string & oldPath,
	   const std::string & newPath );
  bool commit();
  void setOverwrite( bool overwrite = true );
  bool cp( const std::string & oldPath,
	   const std::string & newPath,
	   bool recursive = false );
  bool symlink( const std::string & path,
		const std::string & alias );
  bool mount( const std::string & path,
	      AIDA::ITree & tree,
	      const std::string & treePath );
  bool unmount( const std::string & path );
  bool close();
  // from AIDA 3.2
  void * cast(const  std::string & className) const; 

private:
  AIDA::Dev::IStore*          m_store;
  AIDA::Dev::IDevTree*        m_parentTree;
  bool                        m_overWrite;
  std::string                 m_cwd;
  std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >  m_mountPoints;
  std::map< const AIDA::IManagedObject*, std::string >  m_objectPaths;
  iAIDA::aida_tree::SubTree*                    m_rootDir;

  // Private method
  std::string absolutePath( const std::string& path ) const;
};

  }
}

#endif /* ifndef IAIDA_TREE */
