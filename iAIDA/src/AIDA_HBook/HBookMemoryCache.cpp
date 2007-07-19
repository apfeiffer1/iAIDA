#include "HBookMemoryCache.h"
#include "HBookPathFormater.h"
#include "HBook.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

pi::AIDA_HBookStore::HBookMemoryCache::HBookMemoryCache( const std::string& zebraName ):
  m_zebraName( zebraName )
{
  pi::AIDA_HBookStore::HBook::changeDirectory( "//PAWC" );
  pi::AIDA_HBookStore::HBook::makeNewDirectory( m_zebraName );
}


pi::AIDA_HBookStore::HBookMemoryCache::~HBookMemoryCache()
{
  clearDirectory( "//PAWC/" + m_zebraName );
  pi::AIDA_HBookStore::HBook::changeDirectory( "//PAWC" );
  pi::AIDA_HBookStore::HBook::deleteExistingDirectory( m_zebraName );
}


void
pi::AIDA_HBookStore::HBookMemoryCache::flushToFile( const std::set< std::string >& tuplesToUpdate ) const
{
  flushToFile( "//PAWC/" + m_zebraName, tuplesToUpdate );
  clearHistogramsFromCache( "//PAWC/" + m_zebraName );
}


std::string
pi::AIDA_HBookStore::HBookMemoryCache::directoryInFile( const std::string& directory ) const
{
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  return dir;
}


std::string
pi::AIDA_HBookStore::HBookMemoryCache::directoryInMemory( const std::string& directory ) const
{
  std::vector< std::string > names = pi::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  return dir;
}


void
pi::AIDA_HBookStore::HBookMemoryCache::clearDirectory( const std::string& directory ) const
{
  pi::AIDA_HBookStore::HBook::changeDirectory( directory );
  pi::AIDA_HBookStore::HBook::deleteFromMemory( 0 );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< std::string > subdirs;
  std::string foundTitle = "";
  int iDir = 0;
  int iDirPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								  pi::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iDir == iDirPrev ) break;
    subdirs.insert( foundTitle );
    iDirPrev = iDir;
  }
  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    clearDirectory( directory + "/" + ( *iSubDir ) );
    pi::AIDA_HBookStore::HBook::changeDirectory( directory );
    pi::AIDA_HBookStore::HBook::deleteExistingDirectory( *iSubDir );
  }
}


void
pi::AIDA_HBookStore::HBookMemoryCache::clearHistogramsFromCache( const std::string& directory ) const
{
  pi::AIDA_HBookStore::HBook::changeDirectory( directory );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< std::string > subdirs;
  std::set< int > objectsToDelete;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::UNKNOWN,
								  objectType, foundTitle );
    if ( iObj == iObjPrev ) break;
    if ( objectType == pi::AIDA_HBookStore::HBook::DIRECTORY ) {
      subdirs.insert( foundTitle );
    }
    else {
      if ( iObj == 0 ) break;
      if ( objectType != pi::AIDA_HBookStore::HBook::NTUPLE ) {
	objectsToDelete.insert( iObj );
      }
    }
    iObjPrev = iObj;
  }

  for ( std::set< int >::const_iterator iObj = objectsToDelete.begin();
	iObj != objectsToDelete.end(); ++iObj ) {
    pi::AIDA_HBookStore::HBook::deleteFromMemory( *iObj );
  }

  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    clearHistogramsFromCache( directory + "/" + ( *iSubDir ) );
  }
}


void
pi::AIDA_HBookStore::HBookMemoryCache::flushToFile( const std::string& directory,
							const std::set< std::string >& tuplesToUpdate ) const
{
  pi::AIDA_HBookStore::HBook::changeDirectory( directory );
  pi::AIDA_HBookStore::HBook::ObjectType objectType;
  std::set< int > objects;
  std::set< std::string > subdirs;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  pi::AIDA_HBookStore::HBook::UNKNOWN,
								  objectType, foundTitle );
    if ( iObj == iObjPrev) break;
    if ( objectType == pi::AIDA_HBookStore::HBook::DIRECTORY ) {
      subdirs.insert( foundTitle );
    }
    else {
      if ( iObj == 0 ) break;
      else {
	if ( objectType == pi::AIDA_HBookStore::HBook::NTUPLE ) {
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
  pi::AIDA_HBookStore::HBook::changeDirectory( dirInFile );
  for ( std::set< int >::const_iterator iObjectToFlush = objects.begin();
	iObjectToFlush != objects.end(); ++iObjectToFlush ) {
    pi::AIDA_HBookStore::HBook::flushObjectToFile( *iObjectToFlush );
  }

  for ( std::set< std::string >::const_iterator iSubDir = subdirs.begin();
	iSubDir != subdirs.end(); ++iSubDir ) {
    flushToFile( directory + "/" + ( *iSubDir ), tuplesToUpdate );
  }
}
