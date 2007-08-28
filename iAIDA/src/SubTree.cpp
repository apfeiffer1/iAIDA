#include "SubTree.h"
#include "PathParser.h"
#include "MountInfo.h"

#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IStore.h"

void showObjs( const std::map< std::string, AIDA::IManagedObject* > &m_objects, std::string where ) {
  std::cerr << "showing m_objects " << where
	    << " size : " << m_objects.size() << std::endl;
  std::map< std::string, AIDA::IManagedObject* >::const_iterator iObj;
  for ( iObj = m_objects.begin(); iObj != m_objects.end(); ++iObj) {
    std::cerr << " '" << iObj->first << "'"
	      << " size = " << iObj->first.size() << " "
	      << " : 0x" << std::ios::hex << iObj->second 
	      << " (" << std::ios::dec << iObj->second << ")" 
	      << std::endl;
  }
}

std::map< std::string, AIDA::IManagedObject* >::iterator 
findObj( std::map< std::string, AIDA::IManagedObject* > &m_objects, std::string what ) {
  std::map< std::string, AIDA::IManagedObject* >::iterator iObj;
  for ( iObj = m_objects.begin(); iObj != m_objects.end(); ++iObj) {
    std::cerr << "what = '" << what << "' "
	      << "size = " << what.size() << " "
	      << "iObj->first = '" << iObj->first << "' "
	      << "size = " << iObj->first.size() << " "
	      << "comparison is " << (what == iObj->first )
	      << std::endl;

    if (what == iObj->first ) return iObj;
  }
  return m_objects.end();
}

iAIDA::aida_tree::SubTree::SubTree( const std::string& name,
					    AIDA::Dev::IDevTree* masterTree ):
  m_name( name ),
  m_masterTree( masterTree ),
  m_mountInfo( 0 ),
  m_subTrees(),
  m_objects(),
  m_links()
{}


iAIDA::aida_tree::SubTree::~SubTree()
{
  if ( m_mountInfo ) delete m_mountInfo;
  for ( std::map< std::string, AIDA::IManagedObject* >::iterator iObj = m_objects.begin();
	iObj != m_objects.end(); ++iObj ) {
    delete iObj->second;
  }
  for ( std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.begin();
	iDir != m_subTrees.end(); ++iDir ) {
    delete iDir->second;
  }
}


bool
iAIDA::aida_tree::SubTree::add( AIDA::IManagedObject * object,
					std::list< std::string >& path,
					bool overWrite,
					std::map< const AIDA::IManagedObject*, std::string >& objectMap,
					bool copy,
					bool addInStore )
{
  if ( m_mountInfo ) {
    if ( ! object ) return false;
    std::list< std::string > targetNames = tree_path_parser.formNames( m_mountInfo->targetPath() );
    for ( std::list< std::string >::const_iterator iName = path.begin(); iName != path.end(); ++iName ) {
      targetNames.push_back( *iName );
    }
    if ( copy ) {
      return m_mountInfo->targetTree()->copyAndAdd( object, "/" + tree_path_parser.formPath( targetNames ) );
    }
    else {
      targetNames.pop_back(); // remove the "filename" from the directory part
      return m_mountInfo->targetTree()->add( object, "/" + tree_path_parser.formPath( targetNames ) );
    }
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  const std::string& name = path.front();
  // First of all check whether it still has to propagate the call even further.
  if ( path.size() > 1 ) {
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iSubDir = m_subTrees.find( name );
    if ( iSubDir == m_subTrees.end() ) {
      return false;
    }
    else { // propagate call to the underlying sub-tree
      path.pop_front();
      return iSubDir->second->add( object, path, overWrite, objectMap, copy, addInStore );
    }
  }

  // We are in the target directory
  // make sure that there is no directory or link named after the same name
  std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iSubDir = m_subTrees.find( name );
  if ( iSubDir != m_subTrees.end() ) {
    if ( copy &&
	 iSubDir->second->m_mountInfo == 0 &&
	 iSubDir->second->m_subTrees.find( name ) != iSubDir->second->m_subTrees.end() ) {
      return iSubDir->second->add( object, path, overWrite, objectMap, copy, addInStore );
    }
    else return false;
  }

  // Check if an object with this name already exists
  if ( m_objects.find( name ) != m_objects.end() ) {
    if ( overWrite ) {
      // Remove first the old one
      m_masterTree->rm( m_name + name );
      m_objects.insert( std::make_pair( name, object ) );
      if ( object ) {
	if ( copy ) {
	  AIDA::IManagedObject* newObject = m_masterTree->nativeStore()->copyAndWrite( *object, m_name + name );
	  m_objects[ name ] = newObject;
	  objectMap[ newObject ] = m_name + name;
	}
	else {
	  if (addInStore) m_masterTree->nativeStore()->writeObject( *object, m_name + name );
	  objectMap[ object ] = m_name + name;
	}
      }
      return true;
    }
    else return false;
  }
  else {
    // Check if a symbolic link exists with the same name.
    // Note : if in a copy method it is the directory that it is specified and the last node is a link,
    // then the link is overwritten and not followed. (We cannot look at it since it may point to another tree).
    if ( m_links.find( name ) != m_links.end() ){
      if ( overWrite ) {
	m_links.erase( name );
      }
      else {
	return false;
      }
    }
    m_objects.insert( std::make_pair( name, object ) );
    if ( object ) {
      if ( copy ) {
	AIDA::IManagedObject* newObject = m_masterTree->nativeStore()->copyAndWrite( *object, m_name + name );
	m_objects[ name ] = newObject;
	objectMap[ newObject ] = m_name + name;
      }
      else {
	if (addInStore) m_masterTree->nativeStore()->writeObject( *object, m_name + name );
	objectMap[ object ] = m_name + name;
      }
    }
    return true;
  }
}


AIDA::Dev::IStore*
iAIDA::aida_tree::SubTree::store( std::list< std::string >& path )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->store( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }
  // LM : check path first 
  if ( path.empty() ) { 
    return 0; 
  }

  const std::string& dirName = path.front();
  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return 0;
    }
    path.pop_front();
    return iDir->second->store( path );
  }

  // This is the mother directory. Check if the target exists
  std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
  if ( iDir != m_subTrees.end() ) {
    if ( iDir->second->m_mountInfo ) {
      return iDir->second->m_mountInfo->targetTree()->store( iDir->second->m_mountInfo->targetPath() );
    }
    else return m_masterTree->nativeStore();
  }
  if ( m_objects.find( dirName ) != m_objects.end() ) return m_masterTree->nativeStore();
  std::map< std::string, std::string >::const_iterator iLink = m_links.find( dirName );
  if ( iLink == m_links.end() ) return 0; // apparently, bad directory path was given
  else return m_masterTree->store( iLink->second );
}


bool
iAIDA::aida_tree::SubTree::existsDirectory( std::list< std::string >& path )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->existsDirectory( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  const std::string& dirName = path.front();
  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    path.pop_front();
    return iDir->second->existsDirectory( path );
  }

  // This is the mother directory. Check if the directory exists
  if ( m_subTrees.find( dirName ) != m_subTrees.end() ) return true;
  else return false;
}


bool
iAIDA::aida_tree::SubTree::mkdir( std::list< std::string >& path )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->mkdir( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  const std::string& dirName = path.front();
  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    path.pop_front();
    return iDir->second->mkdir( path );
  }

  // This is the mother directory. Check the validity of the name and add in the list of directories
  if ( m_objects.find( dirName ) != m_objects.end() ) return false;
  if ( m_links.find( dirName ) != m_links.end() ) return false;
  if ( m_subTrees.find( dirName ) != m_subTrees.end() ) return false;
  if ( dirName == "" || dirName == "." || dirName == ".." ) return false; // Check for invalid names ("", ".", "..")

  m_subTrees.insert( std::make_pair( dirName, new iAIDA::aida_tree::SubTree( m_name + dirName + "/", m_masterTree ) ) );
  return true;
}


bool
iAIDA::aida_tree::SubTree::mkdirs( std::list< std::string >& path )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->mkdirs( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  if ( path.size() == 1 ) return mkdir( path );

  // Propagate the call to the next directory
  std::string dirName = path.front();
  path.pop_front();
  // Check if the directory already exists
  std::map< std::string, iAIDA::aida_tree::SubTree* >::const_iterator iDir = m_subTrees.find( dirName );
  if ( iDir == m_subTrees.end() ) {
    if ( m_objects.find( dirName ) != m_objects.end() ) return false;
    else if ( m_links.find( dirName ) != m_links.end() ) return false;
    else { // Not in the links, therefore create the intermediate directory
      m_subTrees[ dirName ] = new iAIDA::aida_tree::SubTree( m_name + dirName + "/", m_masterTree );
      return m_subTrees[ dirName ]->mkdirs( path );
    }
  }
  else {
    return iDir->second->mkdirs( path );
  }
}


bool
iAIDA::aida_tree::SubTree::rmdir( std::list< std::string >& path )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->rmdir( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  const std::string& dirName = path.front();
  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    path.pop_front();
    return iDir->second->rmdir( path );
  }

  // This is the parent directory.
  if ( m_objects.find( dirName ) != m_objects.end() ) return false;

  // Check if the target directory is empty.
  iAIDA::aida_tree::SubTree& subTree =  * ( m_subTrees[ dirName ] );
  if ( subTree.m_objects.size() > 0 ) return false;
  if ( subTree.m_subTrees.size() > 0 ) return false;
  // Destroy the directory.
  delete m_subTrees[ dirName ];
  m_subTrees.erase( dirName );
  return true;
}


bool
iAIDA::aida_tree::SubTree::symlink( const std::string& path,
					    std::list< std::string >& alias )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->symlink( m_mountInfo->targetPath() + tree_path_parser.formPath( alias ), path );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }

  const std::string& dirName = alias.front();
  if ( alias.size() > 1 ) {
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    alias.pop_front();
    return iDir->second->symlink( path, alias );
  }

  // This is the mother directory. Check if there is a directory or an object with the same name
  if ( m_subTrees.find( dirName ) != m_subTrees.end() ) return false;
  if ( m_objects.find( dirName ) != m_objects.end() ) return false;

  // Check if there is a link with the same name
  return m_links.insert( std::make_pair( dirName, path ) ).second;
}



bool
iAIDA::aida_tree::SubTree::mount( const std::string & path,
					  AIDA::Dev::IDevTree* tree,
					  std::list< std::string >& treePath,
					  std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints )
{
  if ( m_mountInfo ) {
    return false; // Directory is already a mount point for a tree
  }

  if ( treePath.size() > 0 ) {
    const std::string& dirName = treePath.front();
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    treePath.pop_front();
    return iDir->second->mount( path, tree, treePath, mountPoints );
  }

  // This is the mount directory. Make sure that we are not mounting the same tree !!!
  if ( tree == m_masterTree ) return false;

  m_mountInfo = new MountInfo( tree, path, m_name );
  tree->setParentTree( m_masterTree );
  return mountPoints.insert( std::make_pair( tree, m_mountInfo ) ).second;
}


bool
iAIDA::aida_tree::SubTree::unmount( std::list< std::string >& path,
					    std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints )
{
  AIDA::Dev::IDevTree* p = 0;
  if ( m_mountInfo ) {
    if ( path.size() == 0 ) {
      p = m_mountInfo->targetTree();
      delete m_mountInfo;
      m_mountInfo = 0;
    }
  }
  else if ( path.size() > 0 ) {
    const std::string& dirName = path.front();
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    path.pop_front();
    return iDir->second->unmount( path, mountPoints );
  }

  if ( p ) {
    p->setParentTree( 0 );
    mountPoints.erase( p );
    return true;
  }
  else return false;
}


bool
iAIDA::aida_tree::SubTree::rm( std::list< std::string >& path,
				       std::map< const AIDA::IManagedObject*, std::string >& objectMap )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->rm( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return false; 
  }
  const std::string& dirName = path.front();

  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return false;
    }
    path.pop_front();
    return iDir->second->rm( path, objectMap );
  }

  // This is the mother directory.
  // Check if the name is one of the links
  if ( m_links.find( dirName ) != m_links.end() ) {
    m_links.erase( dirName );
    return true;
  }
  else { // Check in the registered objects
    std::map< std::string, AIDA::IManagedObject* >::iterator iObj = m_objects.find( dirName );
    if ( iObj == m_objects.end() ) return false; // Invalid path
    AIDA::IManagedObject* objectPointer = iObj->second;
    if ( objectPointer ) delete objectPointer;
    m_objects.erase( iObj );
    m_masterTree->nativeStore()->removeObject( m_name + dirName ); // remove object from the store.
    objectMap.erase( objectPointer );
    return true;
  }
}


AIDA::IManagedObject*
iAIDA::aida_tree::SubTree::find( std::list< std::string >& path,
					 std::map< const AIDA::IManagedObject*, std::string >& objectMap )
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->find( m_mountInfo->targetPath() + tree_path_parser.formPath( path ) );
  }

  // LM : check path first 
  if ( path.empty() ) { 
    return 0; 
  }

  const std::string& dirName = path.front();
  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return 0;
    }
    path.pop_front();
    return iDir->second->find( path, objectMap );
  }

  // This is the mother directory.
  // Check if the name is one of the links
  std::map< std::string, std::string >::const_iterator iLink = m_links.find( dirName );
  if ( iLink != m_links.end() ) {
    return m_masterTree->find( iLink->second );
  }
  else { // Check in the registered objects

    std::map< std::string, AIDA::IManagedObject* >::iterator iObj = m_objects.find( dirName );
    if ( iObj == m_objects.end() ) {
      return 0; // Invalid path
    } else {
      // Check if the object has been fetched from the store.
      AIDA::IManagedObject* objectPointer = iObj->second;
      if ( objectPointer == 0 ) {
	objectPointer = m_masterTree->nativeStore()->retrieveObject( m_name + dirName );
        if ( objectPointer ) objectMap[ objectPointer ] = m_name + dirName;
      }
      return objectPointer;
    }
  }
}


AIDA::ITree*
iAIDA::aida_tree::SubTree::findTree( std::list< std::string >& path,
				  std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >& mountPoints )
{
  if ( m_mountInfo ) {
    if ( path.size() == 0 ) { 
      return m_mountInfo->targetTree();
    }
  } else if ( path.size() > 0 ) {
    const std::string& dirName = path.front();
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return 0;
    }
    path.pop_front();
    return iDir->second->findTree( path, mountPoints );
  }

  return 0;
  
}


AIDA::IManagedObject*
iAIDA::aida_tree::SubTree::findAndErase( std::list< std::string >& path )
{

  // LM : check path first 
  if ( path.empty() ) { 
    return 0; 
  }

  std::cout <<" Find And Erase" << path.size() << std::endl;

  if ( m_mountInfo ) return 0;

  const std::string& dirName = path.front();

  std::cout << "dirname " << dirName << std::endl; 

  if ( path.size() > 1 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return 0;
    }
    path.pop_front();
    return iDir->second->findAndErase( path );
  }

  // This is the mother directory.
  // Check if the name is one of the links
  if ( m_links.find( dirName ) != m_links.end() ) return 0;
  else { // Check in the registered objects
    std::map< std::string, AIDA::IManagedObject* >::const_iterator iObj = m_objects.find( dirName );
    if ( iObj == m_objects.end() ) return 0; // Invalid path
    else {
      AIDA::IManagedObject* p = iObj->second;
      m_objects.erase( iObj->first );
      return p;
    }
  }
}


std::vector< std::string >
iAIDA::aida_tree::SubTree::listObjectNames( std::list< std::string >& path,
						    bool recursive ) const
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->listObjectNames( m_mountInfo->targetPath() + tree_path_parser.formPath( path ), recursive );
  }

  const std::string& dirName = path.front();
  if ( path.size() > 0 ) {   // Check if the directory already exists
    std::map< std::string, iAIDA::aida_tree::SubTree* >::const_iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return std::vector< std::string >();
    }
    path.pop_front();
    return iDir->second->listObjectNames( path, recursive );
  }

  // This is the mother directory.
  std::vector< std::string > output;
  // List first the links:
  for ( std::map< std::string, std::string >::const_iterator iLink = m_links.begin();
	iLink != m_links.end(); ++iLink ) {
    output.push_back( iLink->first );
  }

  // Actual objects then. Ask the store to provide them.
  std::vector< std::string > objectNamesFromStore = m_masterTree->nativeStore()->listObjectPaths( m_name, false );
  std::vector< std::string > objectTypesFromStore = m_masterTree->nativeStore()->listObjectTypes( m_name, false );
  for ( unsigned int i = 0; i < objectNamesFromStore.size(); ++i ) {
    if ( objectTypesFromStore[i] != "dir" ) {
      output.push_back( objectNamesFromStore[i].substr( m_name.size() ) );
    }
  }

  // Finally list the directories:
  for ( std::map< std::string, iAIDA::aida_tree::SubTree* >::const_iterator iDir = m_subTrees.begin();
	iDir != m_subTrees.end(); ++iDir ) {
    output.push_back( iDir->first );
    if ( recursive ) {
      std::list< std::string > emptyList;
      std::vector< std::string > dirOutput = iDir->second->listObjectNames( emptyList, recursive );
      for ( unsigned int i = 0; i < dirOutput.size(); ++i ) output.push_back( iDir->first + "/" + dirOutput[i] );
    }
  }
  return output;
}


std::vector< std::string >
iAIDA::aida_tree::SubTree::listObjectTypes( std::list< std::string >& path,
						    bool recursive ) const
{
  if ( m_mountInfo ) {
    return m_mountInfo->targetTree()->listObjectTypes( m_mountInfo->targetPath() + tree_path_parser.formPath( path ), recursive );
  }

  if ( path.size() > 0 ) {   // Check if the directory already exists
    const std::string& dirName = path.front();
    std::map< std::string, iAIDA::aida_tree::SubTree* >::const_iterator iDir = m_subTrees.find( dirName );
    if ( iDir == m_subTrees.end() ) {
      return std::vector< std::string >();
    }
    path.pop_front();
    return iDir->second->listObjectTypes( path, recursive );
  }

  // This is the mother directory.
  std::vector< std::string > output;
  // List first the links:
  for ( std::map< std::string, std::string >::const_iterator iLink = m_links.begin();
	iLink != m_links.end(); ++iLink ) {
    output.push_back( "-> " + iLink->second );
  }

  // Actual objects then. Ask the store to provide them.
  std::vector< std::string > objectTypesFromStore = m_masterTree->nativeStore()->listObjectTypes( m_name, false );
  for ( unsigned int i = 0; i < objectTypesFromStore.size(); ++i ) {
    if ( objectTypesFromStore[i] != "dir" ) output.push_back( objectTypesFromStore[i] );
  }

  // Finally list the directories:
  for ( std::map< std::string, iAIDA::aida_tree::SubTree* >::const_iterator iDir = m_subTrees.begin();
	iDir != m_subTrees.end(); ++iDir ) {
    output.push_back( "dir" );
    if ( recursive ) {
      std::list< std::string > emptyList;
      std::vector< std::string > dirOutput = iDir->second->listObjectTypes( emptyList, recursive );
      for ( unsigned int i = 0; i < dirOutput.size(); ++i ) output.push_back( dirOutput[i] );
    }
  }
  return output;
}
