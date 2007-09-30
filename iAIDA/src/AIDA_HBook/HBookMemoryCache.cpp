#include "HBookMemoryCache.h"
#include "HBookPathFormater.h"
#include "HBook.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

iAIDA::AIDA_HBookStore::HBookMemoryCache::HBookMemoryCache( const std::string& zebraName ):
  m_zebraName( zebraName )
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( "//PAWC" );
  iAIDA::AIDA_HBookStore::HBook::makeNewDirectory( m_zebraName );
}


iAIDA::AIDA_HBookStore::HBookMemoryCache::~HBookMemoryCache()
{
  clearDirectory( "//PAWC/" + m_zebraName );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( "//PAWC" );
  iAIDA::AIDA_HBookStore::HBook::deleteExistingDirectory( m_zebraName );
}


void
iAIDA::AIDA_HBookStore::HBookMemoryCache::flushToFile( const std::set< std::string >& tuplesToUpdate ) const
{
  flushToFile( "//PAWC/" + m_zebraName, tuplesToUpdate );
  clearHistogramsFromCache( "//PAWC/" + m_zebraName );
}


std::string
iAIDA::AIDA_HBookStore::HBookMemoryCache::directoryInFile( const std::string& directory ) const
{
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  return dir;
}


std::string
iAIDA::AIDA_HBookStore::HBookMemoryCache::directoryInMemory( const std::string& directory ) const
{
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  return dir;
}


void
iAIDA::AIDA_HBookStore::HBookMemoryCache::clearDirectory( const std::string& directory ) const
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( directory );
  iAIDA::AIDA_HBookStore::HBook::deleteFromMemory( 0 );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< std::string > subdirs;
  std::string foundTitle = "";
  int iDir = 0;
  int iDirPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								  iAIDA::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iDir == iDirPrev ) break;
    subdirs.insert( foundTitle );
    iDirPrev = iDir;
  }
  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    clearDirectory( directory + "/" + ( *iSubDir ) );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( directory );
    iAIDA::AIDA_HBookStore::HBook::deleteExistingDirectory( *iSubDir );
  }
}


void
iAIDA::AIDA_HBookStore::HBookMemoryCache::clearHistogramsFromCache( const std::string& directory ) const
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( directory );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< std::string > subdirs;
  std::set< int > objectsToDelete;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::UNKNOWN,
								  objectType, foundTitle );
    if ( iObj == iObjPrev ) break;
    if ( objectType == iAIDA::AIDA_HBookStore::HBook::DIRECTORY ) {
      subdirs.insert( foundTitle );
    }
    else {
      if ( iObj == 0 ) break;
      if ( objectType != iAIDA::AIDA_HBookStore::HBook::NTUPLE ) {
	objectsToDelete.insert( iObj );
      }
    }
    iObjPrev = iObj;
  }

  for ( std::set< int >::const_iterator iObj = objectsToDelete.begin();
	iObj != objectsToDelete.end(); ++iObj ) {
    iAIDA::AIDA_HBookStore::HBook::deleteFromMemory( *iObj );
  }

  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    clearHistogramsFromCache( directory + "/" + ( *iSubDir ) );
  }
}


void
iAIDA::AIDA_HBookStore::HBookMemoryCache::flushToFile( const std::string& directory,
							const std::set< std::string >& tuplesToUpdate ) const
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( directory );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< int > objects;
  std::set< std::string > subdirs;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::UNKNOWN,
								  objectType, foundTitle );
    if ( iObj == iObjPrev) break;
    if ( objectType == iAIDA::AIDA_HBookStore::HBook::DIRECTORY ) {
      subdirs.insert( foundTitle );
    }
    else {
      if ( iObj == 0 ) break;
      else {
	if ( objectType == iAIDA::AIDA_HBookStore::HBook::NTUPLE ) {
	  std::ostringstream os;
	  os << iObj;
#ifndef BADENDS
	  os << std::ends;
#endif
	  const std::string name = os.str();
	  const std::string dir = directory.substr( 7 + m_zebraName.size() );
	  const std::string tuplePath = dir + "/" + name;
	  if ( tuplesToUpdate.find( tuplePath ) != tuplesToUpdate.end() ) {
	    objects.insert( iObj );
	  }
	}
	else {
	  objects.insert( iObj );
	}
      }
    }
    iObjPrev = iObj;
  }

  const std::string dirInFile = "//" + directory.substr( 7 );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dirInFile );
  for ( std::set< int >::const_iterator iObjectToFlush = objects.begin();
	iObjectToFlush != objects.end(); ++iObjectToFlush ) {
    iAIDA::AIDA_HBookStore::HBook::flushObjectToFile( *iObjectToFlush );
  }

  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    flushToFile( directory + "/" + ( *iSubDir ), tuplesToUpdate );
  }
}
