#include "AIDA_Tree.h"
#include "PathParser.h"
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IDevManagedObject.h"
#include "SubTree.h"
#include "MountInfo.h"

static const std::string emptyString = "";
static const std::string thisClassName="AIDA::ITree";

iAIDA::aida_tree::AIDA_Tree::AIDA_Tree( AIDA::Dev::IStore* store ):
  m_store( store ),
  m_parentTree( 0 ),
  m_overWrite( true ),
  m_cwd( "/" ),
  m_mountPoints(),
  m_objectPaths(),
  m_rootDir( 0 )
{
  // Create the local root directory
  m_rootDir = new iAIDA::aida_tree::SubTree( "/", this );

  /// Fetch the directory structure from the store
  std::vector< std::string > objectPaths = m_store->listObjectPaths( "/", true );
  std::vector< std::string > objectTypes = m_store->listObjectTypes( "/", true );

  /// Construct the directory structure first.
  for ( unsigned int iEntry = 0; iEntry < objectTypes.size(); ++iEntry ) {
    if ( objectTypes[ iEntry ] == "dir" ) {
      std::list< std::string > dirPath = tree_path_parser.formNames( objectPaths[iEntry] );
      m_rootDir->mkdirs( dirPath );
    }
  }

  /// Add empty entries for the objects.
  for ( unsigned int iEntry = 0; iEntry < objectTypes.size(); ++iEntry ) {
    if ( objectTypes[ iEntry ] != "dir" ) {
      std::list< std::string > path = tree_path_parser.formNames( objectPaths[iEntry] );
      m_rootDir->add( 0, path, true, m_objectPaths, false );
    }
  }
}


iAIDA::aida_tree::AIDA_Tree::~AIDA_Tree()
{
  /// Ask parent tree to be unmount this tree
  if ( m_parentTree ) m_parentTree->unmountTree( this );

  /// Unmount the dependent trees
  for ( std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::const_iterator iMountPoint = m_mountPoints.begin();
	iMountPoint != m_mountPoints.end(); ++iMountPoint ) {
    iMountPoint->first->setParentTree( 0 );
  }

  /// Umount the local root file system
  if ( m_rootDir ) delete m_rootDir;

  /// Close and clear the local store
  if ( m_store ) {
    m_store->close();
    delete m_store;
  }
}


bool
iAIDA::aida_tree::AIDA_Tree::add( AIDA::IManagedObject * object,
					  const std::string& directory )
{
  // Check the validity of the pointer
  if ( ! object ) return false;

  std::string objectName = object->name();
  std::list< std::string > objectNameList = tree_path_parser.formNames( objectName );
  if ( objectNameList.size() != 1 ) return false;
  objectName = tree_path_parser.formPath( objectNameList );

  std::list<std::string> dirNames = tree_path_parser.formNames( absolutePath( directory ) + "/" + objectName );
  return m_rootDir->add( object, dirNames, m_overWrite, m_objectPaths, false );
}


bool
iAIDA::aida_tree::AIDA_Tree::copyAndAdd( AIDA::IManagedObject * object,
						 const std::string& path )
{
  // Check the validity of the pointer
  if ( ! object ) return false;

  // Check if it is a tuple and if the store can copy tuples
  AIDA::Dev::IDevManagedObject* devObject = dynamic_cast< AIDA::Dev::IDevManagedObject* >( object );
  if ( ! devObject ) return false;
  if ( devObject->userLevelClassType() == "ITuple" && ! ( m_store->canCopyTuples() ) ) return false;

  std::string dir = absolutePath( path ) + "/";
  // Check here if the target is an existing directory. If this is the case, append the object name.
  if ( existsDirectory( dir ) ) dir += "/" + object->name();
  std::list< std::string > dirNames = tree_path_parser.formNames( dir );
  return m_rootDir->add( object, dirNames, m_overWrite, m_objectPaths, true );
}


AIDA::Dev::IStore*
iAIDA::aida_tree::AIDA_Tree::nativeStore()
{
  return m_store;
}


AIDA::Dev::IStore*
iAIDA::aida_tree::AIDA_Tree::store( const std::string& path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  if ( names.size() == 0 ) return m_store;
  return m_rootDir->store( names );
}


bool
iAIDA::aida_tree::AIDA_Tree::isMounted() const
{
  return ( ( m_parentTree == 0 ) ? false : true );
}


bool
iAIDA::aida_tree::AIDA_Tree::setParentTree( AIDA::Dev::IDevTree * parentTree )
{
  m_parentTree = parentTree;
  return true;
}


bool
iAIDA::aida_tree::AIDA_Tree::unmountTree( AIDA::Dev::IDevTree * daughterTree )
{
  std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::const_iterator iMountPoint = m_mountPoints.find( daughterTree );
  if ( iMountPoint == m_mountPoints.end() ) return false;
  std::list< std::string > names = tree_path_parser.formNames( iMountPoint->second->mountPoint() );
  return m_rootDir->unmount( names, m_mountPoints );
}


std::string
iAIDA::aida_tree::AIDA_Tree::storeName() const
{
  return m_store->name();
}


AIDA::IManagedObject*
iAIDA::aida_tree::AIDA_Tree::find( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->find( names, m_objectPaths );
}


AIDA::ITree * 
iAIDA::aida_tree::AIDA_Tree::findTree(const std::string & path) 
{ 
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->findTree( names,  m_mountPoints );
}

bool
iAIDA::aida_tree::AIDA_Tree::cd( const std::string & path )
{
  if ( path.size() == 0 ) {
    m_cwd = "/";
    return true;
  }

  m_cwd = absolutePath( path ) + "/";
  return true;
}


std::string
iAIDA::aida_tree::AIDA_Tree::pwd() const
{
  return m_cwd;
}


bool
iAIDA::aida_tree::AIDA_Tree::existsDirectory( const std::string& path ) const
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  if ( names.size() == 0 ) return true;
  return m_rootDir->existsDirectory( names );
}


bool
iAIDA::aida_tree::AIDA_Tree::mkdir( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->mkdir( names );
}


bool
iAIDA::aida_tree::AIDA_Tree::mkdirs( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->mkdirs( names );
}


bool
iAIDA::aida_tree::AIDA_Tree::rmdir( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->rmdir( names );
}


std::string
iAIDA::aida_tree::AIDA_Tree::findPath( const AIDA::IManagedObject & object ) const
{
  // If the path points to an object inside the tree
  std::map< const AIDA::IManagedObject*, std::string >::const_iterator iObj = m_objectPaths.find( &object );
  if ( iObj != m_objectPaths.end() ) return iObj->second;

  // Check in the mounted trees
  for ( std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::const_iterator iTree = m_mountPoints.begin();
	iTree != m_mountPoints.end(); ++iTree ) {
    std::string path = iTree->first->findPath( object );
    if ( path.size() > 0 ) {
      // The object is in a mounted tree. Extract the path relative to this tree.
      std::list< std::string > names = tree_path_parser.formNames( path );
      const int dirSize = tree_path_parser.formNames( iTree->second->targetPath() ).size();
      for ( int i = 0; i < dirSize; ++i ) names.pop_front();
      const std::list< std::string > namesOfMountPoint = tree_path_parser.formNames( iTree->second->mountPoint() );
      for ( std::list< std::string >::const_reverse_iterator iName = namesOfMountPoint.rbegin();
	    iName != namesOfMountPoint.rend(); ++iName ) {
	names.push_front( *iName );
      }
      std::string objectPath = "/" + tree_path_parser.formPath( names );
      return objectPath;
    }
  }

  return emptyString;
}


std::string
iAIDA::aida_tree::AIDA_Tree::pathInStore( const AIDA::IManagedObject& object ) const
{
  // If the path points to an object inside the tree
  std::map< const AIDA::IManagedObject*, std::string >::const_iterator iObj = m_objectPaths.find( &object );
  if ( iObj != m_objectPaths.end() ) return iObj->second;

  // Check in the mounted trees
  for ( std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::const_iterator iTree = m_mountPoints.begin();
	iTree != m_mountPoints.end(); ++iTree ) {
    std::string path = iTree->first->pathInStore( object );
    if ( path.size() > 0 ) return path;
  }

  return emptyString;
}


bool
iAIDA::aida_tree::AIDA_Tree::mv( const std::string & oldPath,
					 const std::string & newPath )
{
  // Fetch the store from the oldPath
  AIDA::Dev::IStore* oldStore = store( oldPath );
  if ( ! oldStore ) return false;

  // Fetch the store from the new path
  AIDA::Dev::IStore* newStore = store( newPath );
  if ( ! newStore ) { // give a try with the directory
    newStore = store( tree_path_parser.parent( absolutePath( newPath ) ) );
    if ( ! newStore ) return false;
  }

  if ( oldStore != newStore ) {
    if ( cp( oldPath, newPath ) ) {
      rm( oldPath );
      return true;
    }
    else {
      return false;
    }
  }

  // If this is the local store, simply rename
  if ( oldStore == m_store ) {
    std::string oldAbsolutePath = absolutePath( oldPath );
    std::list< std::string > oldNames = tree_path_parser.formNames( oldAbsolutePath );
    AIDA::IManagedObject* object = m_rootDir->findAndErase( oldNames );
    if ( ! object ) return false;
    m_objectPaths.erase( object );
    std::string dir = absolutePath( newPath );
    // Check here if the target is an existing directory. If this is the case, append the object name.
    if ( existsDirectory( dir + "/" ) ) dir += "/" + object->name();
    std::list< std::string > dirNames = tree_path_parser.formNames( dir );
    bool bSuccess = m_rootDir->add( object, dirNames, m_overWrite, m_objectPaths, false, false );
    if ( ! bSuccess ) { // recover the operation in case of failure
      std::string odir = absolutePath( oldPath ) + "/";
      if ( existsDirectory( odir ) ) odir += "/" + object->name();
      std::list< std::string > odirNames = tree_path_parser.formNames( odir );
      m_rootDir->add( object, odirNames, m_overWrite, m_objectPaths, false, false );
      return false;
    }
    // OK, moved logically in the tree. We issue the command to the store
    return m_store->moveObject( oldAbsolutePath, absolutePath( dir ) );
  }
  else { // This is another tree. Let's propagate the call over there
    std::string oldAbsolutePath = absolutePath( oldPath );
    std::string newAbsolutePath = absolutePath( newPath );
    for ( std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::iterator iMountedTree = m_mountPoints.begin();
	  iMountedTree != m_mountPoints.end(); ++iMountedTree ) {
      AIDA::Dev::IDevTree* tree = iMountedTree->first;
      if ( tree->nativeStore() == oldStore ) { // OK, identified the tree. lets form the relative paths
	std::list<std::string> oldPathNames = tree_path_parser.formNames( oldAbsolutePath );
	std::list<std::string> newPathNames = tree_path_parser.formNames( newAbsolutePath );
	const unsigned int prefixSize = tree_path_parser.formNames( iMountedTree->second->mountPoint() ).size();
	for ( unsigned int i = 0; i < prefixSize; ++i ) {
	  oldPathNames.pop_front();
	  newPathNames.pop_front();
	}
	const std::list<std::string> baseNames = tree_path_parser.formNames( iMountedTree->second->targetPath() );
	for ( std::list<std::string>::const_reverse_iterator iBase = baseNames.rbegin();
	      iBase != baseNames.rend(); ++iBase ) {
	  oldPathNames.push_front( *iBase );
	  newPathNames.push_front( *iBase );
	}
	// OK, return the move command inside the tree
	tree->mv( "/" + tree_path_parser.formPath( oldPathNames ),
		  "/" + tree_path_parser.formPath( newPathNames ) );
      }
    }
  }

  return false;
}


bool
iAIDA::aida_tree::AIDA_Tree::commit()
{
  bool success = m_store->commit();
  // Propagate the commit to the dependent trees
  for ( std::map< AIDA::Dev::IDevTree*, iAIDA::aida_tree::MountInfo* >::const_iterator iTree = m_mountPoints.begin();
	iTree != m_mountPoints.end(); ++iTree ) {
    success = success && iTree->first->commit();
  }
  return success;
}


void
iAIDA::aida_tree::AIDA_Tree::setOverwrite( bool overwrite )
{
  m_overWrite = overwrite;
}


bool
iAIDA::aida_tree::AIDA_Tree::rm( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->rm( names, m_objectPaths );
}


bool
iAIDA::aida_tree::AIDA_Tree::cp( const std::string & oldPath,
					 const std::string & newPath,
					 bool recursive )
{
  if ( recursive ) {
    // Retrieve the directory structure under the old path.
    std::vector< std::string > objectNames = listObjectNames( oldPath, true );
    std::vector< std::string > objectTypes = listObjectTypes( oldPath, true );

    // Form the new directory path correctly.
    if ( newPath.size() == 0 ) return false;
    std::string dir = absolutePath( newPath ) + "/";

    // Form the old directory path correctly.
    if ( oldPath.size() == 0 ) return false;
    std::string oldDir = absolutePath( oldPath ) + "/";

    // Copy the directory structure first
    mkdirs( dir );
    for ( unsigned int i = 0; i < objectNames.size(); ++i ) {
      if ( objectTypes[i] == "dir" ) {
	mkdirs( dir + objectNames[i] );
      }
    }

    // Copy and add the new objects under the new structure.
    for ( unsigned int i = 0; i < objectNames.size(); ++i ) {
      if ( objectTypes[i] != "dir" ) {
	AIDA::IManagedObject* object = find( oldDir + objectNames[i] );
        if ( object ) copyAndAdd( object, dir + objectNames[i] );
      }
    }
    return true;
  }
  else { // Simple copy
    AIDA::IManagedObject* object = find( oldPath );
    if ( ! object ) return false;
    return copyAndAdd( object, newPath );
  }
}


bool
iAIDA::aida_tree::AIDA_Tree::symlink( const std::string & path,
					      const std::string & alias )
{
  if ( alias.size() == 0 ) return false;
  std::list< std::string > aliasNames = tree_path_parser.formNames( alias );
  if( aliasNames.size() == 0 || ( aliasNames.size() == 1 && aliasNames.front() == "." ) ) return false;
  std::list< std::string > dirNames = tree_path_parser.formNames( absolutePath( alias ) );
  return m_rootDir->symlink( absolutePath( path ), dirNames );
}


bool
iAIDA::aida_tree::AIDA_Tree::mount( const std::string & path,
					    AIDA::ITree & tree,
					    const std::string & treePath )
{
  AIDA::Dev::IDevTree * daughterTree = dynamic_cast< AIDA::Dev::IDevTree * >( &tree );
  if ( daughterTree->isMounted() ) return false;

  if ( path.size() == 0 ) return false;
  std::list< std::string > pathNames = tree_path_parser.formNames( path );
  if( pathNames.size() == 0 || ( pathNames.size() == 1 && pathNames.front() == "." ) ) return false;
  std::list< std::string > dirNames = tree_path_parser.formNames( absolutePath( path ) );

  // form correctly the target path
  std::list< std::string > targetNames = tree_path_parser.formNames( treePath );
  std::string targetTreePath = "/";
  if (targetNames.size() > 0 ) targetTreePath = tree_path_parser.formPath( targetNames ) + "/";
  return m_rootDir->mount( targetTreePath, daughterTree, dirNames, m_mountPoints );
}


bool
iAIDA::aida_tree::AIDA_Tree::unmount( const std::string & path )
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->unmount( names, m_mountPoints );
}


bool
iAIDA::aida_tree::AIDA_Tree::close()
{
  return m_store->close();
}


std::vector< std::string >
iAIDA::aida_tree::AIDA_Tree::listObjectNames( const std::string & path,
						      bool recursive ) const
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->listObjectNames( names, recursive );
}


std::vector< std::string >
iAIDA::aida_tree::AIDA_Tree::listObjectTypes( const std::string & path,
						      bool recursive ) const
{
  std::list< std::string > names = tree_path_parser.formNames( absolutePath( path ) );
  return m_rootDir->listObjectTypes( names, recursive );
}


bool
iAIDA::aida_tree::AIDA_Tree::ls( const std::string & path,
					 bool recursive,
					 std::ostream & os ) const
{
  if ( ! existsDirectory( path ) ) return false;
  std::vector< std::string > objectNames = listObjectNames( path, recursive );
  std::vector< std::string > objectTypes = listObjectTypes( path, recursive );

  for ( unsigned int i = 0; i < objectNames.size(); ++i ) {
    os << objectNames[i] << "\t : \t" << objectTypes[i] << std::endl;
  }
  return true;
}


std::string
iAIDA::aida_tree::AIDA_Tree::absolutePath( const std::string& path ) const
{
  if ( path.size() == 0 ) return emptyString;
  std::list< std::string > names = tree_path_parser.formNames( m_cwd );
  if ( path.size() > 0 && path[0] == '/' ) names.clear(); // Action for absolute path
  std::list< std::string > relativeNames = tree_path_parser.formNames( path );
  if ( relativeNames.size() > 0 ) {
    if ( relativeNames.front() == "." ) relativeNames.pop_front();
    else if ( relativeNames.front() == ".." ) {
      if ( names.size() == 0 ) return emptyString;
      else {
	relativeNames.pop_front();
	names.pop_back();
      }
    }
  }
  std::string dir = "/";
  if ( names.size() > 0 ) dir += tree_path_parser.formPath( names ) + "/";
  if ( relativeNames.size() > 0 ) dir += tree_path_parser.formPath( relativeNames );
  return dir;
}

void * 
iAIDA::aida_tree::AIDA_Tree::cast(const  std::string & className) const
{ 
  if (className == thisClassName) 
    return const_cast<AIDA_Tree *> (this);
  else 
    return 0;  
}
  
