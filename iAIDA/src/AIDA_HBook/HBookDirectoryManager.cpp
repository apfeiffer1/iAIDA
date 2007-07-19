#include "HBookDirectoryManager.h"
#include "HBook.h"
#include "HBookPathFormater.h"
#include <set>

pi::AIDA_HBookStore::HBookDirectoryManager::HBookDirectoryManager( const std::string& zebraName ):
  m_zebraName( zebraName )
{
  copyDirectoryStructureInMemory( "/" );
}


bool
pi::AIDA_HBookStore::HBookDirectoryManager::existsDirectory( const std::string& directory ) const
{
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  if ( names.empty() ) return true;
  pi::AIDA_HBookStore::HBook::changeDirectory( "//PAWC/" + m_zebraName );
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    pi::AIDA_HBookStore::HBook::ObjectType objectType;
    std::string foundTitle = "";
    int iDir = 0;
    int iDirPrev = 0;
    bool foundDir = false;
    while ( true ) {
      pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								    pi::AIDA_HBookStore::HBook::DIRECTORY,
								    objectType, foundTitle );
      if ( iDir == iDirPrev || iDir == 0 ) break;
      if ( foundTitle == names[iName] ) {
	foundDir = true;
	break;
      }
      iDirPrev = iDir;
    }
    if ( ! foundDir ) return false;
    pi::AIDA_HBookStore::HBook::changeDirectory( foundTitle );
  }
  return true;  
}


bool
pi::AIDA_HBookStore::HBookDirectoryManager::createDirectory( const std::string& directory ) const
{
  if ( existsDirectory( directory ) ) return false;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  pi::AIDA_HBookStore::HBook::changeDirectory( "//" + m_zebraName );
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    pi::AIDA_HBookStore::HBook::ObjectType objectType;
    std::string foundTitle = "";
    int iDir = 0;
    int iDirPrev = 0;
    bool foundDir = false;
    while ( true ) {
      pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								    pi::AIDA_HBookStore::HBook::DIRECTORY,
								    objectType, foundTitle );
      if ( iDir == iDirPrev ) break;
      if ( foundTitle == names[iName] ) {
	foundDir = true;
	break;
      }
      iDirPrev = iDir;
    }
    if ( ! foundDir ) {
      // Create the directory in file
      pi::AIDA_HBookStore::HBook::makeNewDirectory( names[iName] );
      // Create the directory in memory
      std::string memDir = "//PAWC/" + m_zebraName;
      for ( unsigned int jName = 0; jName < iName; ++jName ) memDir += "/" + names[jName];
      pi::AIDA_HBookStore::HBook::changeDirectory( memDir );
      pi::AIDA_HBookStore::HBook::makeNewDirectory( names[iName] );
    }
    // Change to the directory in file
    std::string fileDir = "//" + m_zebraName;
    for ( unsigned int jName = 0; jName <= iName; ++jName ) fileDir += "/" + names[jName];
    pi::AIDA_HBookStore::HBook::changeDirectory( fileDir );
  }
  return true;
}


bool
pi::AIDA_HBookStore::HBookDirectoryManager::removeDirectory( const std::string& directory ) const
{
  if ( ! existsDirectory( directory ) ) return false;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  if ( names.size() == 0 ) return false;
  std::string memDir = "//PAWC/" + m_zebraName;
  std::string fileDir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size() - 1; ++iName ) {
    memDir += "/" + names[iName];
    fileDir += "/" + names[iName];
  }
  pi::AIDA_HBookStore::HBook::changeDirectory( fileDir );
  pi::AIDA_HBookStore::HBook::deleteExistingDirectory( names.back() );
  pi::AIDA_HBookStore::HBook::changeDirectory( memDir );
  pi::AIDA_HBookStore::HBook::deleteExistingDirectory( names.back() );
  return true;
}


std::vector< std::string >
pi::AIDA_HBookStore::HBookDirectoryManager::subDirectories( const std::string& directory ) const
{
  std::vector< std::string > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iDir = 0;
  int iDirPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								  pi::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iDir == iDirPrev || iDir == 0 ) break;
    result.push_back( foundTitle );
    iDirPrev = iDir;
  }
  return result;
}


std::vector< int >
pi::AIDA_HBookStore::HBookDirectoryManager::histograms1D( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  pi::AIDA_HBookStore::HBook::changeDirectory( memdir );
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::HISTO1D,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    // check if it is a profile or not
    pi::AIDA_HBookStore::HBook::loadObjectIntoMemory( iObj );
    if ( pi::AIDA_HBookStore::HBook::typeOfObject( iObj ) == pi::AIDA_HBookStore::HBook::HISTO1D ) {
      result.push_back( iObj );
    }
    pi::AIDA_HBookStore::HBook::deleteFromMemory( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
pi::AIDA_HBookStore::HBookDirectoryManager::histograms2D( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
  }
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::HISTO2D,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    result.push_back( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
pi::AIDA_HBookStore::HBookDirectoryManager::profiles( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  pi::AIDA_HBookStore::HBook::changeDirectory( memdir );
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::PROFILE,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    // check if it is a profile or not
    pi::AIDA_HBookStore::HBook::loadObjectIntoMemory( iObj );
    if ( pi::AIDA_HBookStore::HBook::typeOfObject( iObj ) == pi::AIDA_HBookStore::HBook::PROFILE ) {
      result.push_back( iObj );
    }
    pi::AIDA_HBookStore::HBook::deleteFromMemory( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
pi::AIDA_HBookStore::HBookDirectoryManager::tuples( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
  }
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::NTUPLE,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0 ) break;
    result.push_back( iObj );
    iObjPrev = iObj;
  }
  return result;
}


void
pi::AIDA_HBookStore::HBookDirectoryManager::copyDirectoryStructureInMemory( const std::string& directory ) const
{
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  std::set<std::string> subDirs;
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0 ) break;
    subDirs.insert( foundTitle );
    iObjPrev = iObj;
  }

  pi::AIDA_HBookStore::HBook::changeDirectory( memdir );
  for ( std::set<std::string>::const_iterator iSubDir = subDirs.begin();
	iSubDir != subDirs.end(); ++iSubDir ) {
    pi::AIDA_HBookStore::HBook::makeNewDirectory( *iSubDir );
  }

  for ( std::set<std::string>::const_iterator iSubDir = subDirs.begin();
	iSubDir != subDirs.end(); ++iSubDir ) {
    copyDirectoryStructureInMemory( directory + "/" + ( *iSubDir ) );
  }
}


bool
pi::AIDA_HBookStore::HBookDirectoryManager::isDirectoryEmpty( const std::string& directory ) const
{
  if ( ! existsDirectory( directory ) ) return true;
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  pi::AIDA_HBookStore::HBook::changeDirectory( dir );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iDir = 0;
  pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								pi::AIDA_HBookStore::HBook::UNKNOWN,
								objectType, foundTitle );
  if ( iDir == 0 ) return true;
  else return false;
}
