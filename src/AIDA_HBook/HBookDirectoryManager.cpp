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

#include "HBookDirectoryManager.h"
#include "HBook.h"
#include "HBookPathFormater.h"
#include <set>

iAIDA::AIDA_HBookStore::HBookDirectoryManager::HBookDirectoryManager( const std::string& zebraName ):
  m_zebraName( zebraName )
{
  copyDirectoryStructureInMemory( "/" );
}


bool
iAIDA::AIDA_HBookStore::HBookDirectoryManager::existsDirectory( const std::string& directory ) const
{
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  if ( names.empty() ) return true;
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( "//PAWC/" + m_zebraName );
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
    std::string foundTitle = "";
    int iDir = 0;
    int iDirPrev = 0;
    bool foundDir = false;
    while ( true ) {
      iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								    iAIDA::AIDA_HBookStore::HBook::DIRECTORY,
								    objectType, foundTitle );
      if ( iDir == iDirPrev || iDir == 0 ) break;
      if ( foundTitle == names[iName] ) {
	foundDir = true;
	break;
      }
      iDirPrev = iDir;
    }
    if ( ! foundDir ) return false;
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( foundTitle );
  }
  return true;  
}


bool
iAIDA::AIDA_HBookStore::HBookDirectoryManager::createDirectory( const std::string& directory ) const
{
  if ( existsDirectory( directory ) ) return false;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( "//" + m_zebraName );
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
    std::string foundTitle = "";
    int iDir = 0;
    int iDirPrev = 0;
    bool foundDir = false;
    while ( true ) {
      iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								    iAIDA::AIDA_HBookStore::HBook::DIRECTORY,
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
      iAIDA::AIDA_HBookStore::HBook::makeNewDirectory( names[iName] );
      // Create the directory in memory
      std::string memDir = "//PAWC/" + m_zebraName;
      for ( unsigned int jName = 0; jName < iName; ++jName ) memDir += "/" + names[jName];
      iAIDA::AIDA_HBookStore::HBook::changeDirectory( memDir );
      iAIDA::AIDA_HBookStore::HBook::makeNewDirectory( names[iName] );
    }
    // Change to the directory in file
    std::string fileDir = "//" + m_zebraName;
    for ( unsigned int jName = 0; jName <= iName; ++jName ) fileDir += "/" + names[jName];
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( fileDir );
  }
  return true;
}


bool
iAIDA::AIDA_HBookStore::HBookDirectoryManager::removeDirectory( const std::string& directory ) const
{
  if ( ! existsDirectory( directory ) ) return false;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  if ( names.size() == 0 ) return false;
  std::string memDir = "//PAWC/" + m_zebraName;
  std::string fileDir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size() - 1; ++iName ) {
    memDir += "/" + names[iName];
    fileDir += "/" + names[iName];
  }
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( fileDir );
  iAIDA::AIDA_HBookStore::HBook::deleteExistingDirectory( names.back() );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( memDir );
  iAIDA::AIDA_HBookStore::HBook::deleteExistingDirectory( names.back() );
  return true;
}


std::vector< std::string >
iAIDA::AIDA_HBookStore::HBookDirectoryManager::subDirectories( const std::string& directory ) const
{
  std::vector< std::string > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iDir = 0;
  int iDirPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								  iAIDA::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iDir == iDirPrev || iDir == 0 ) break;
    result.push_back( foundTitle );
    iDirPrev = iDir;
  }
  return result;
}


std::vector< int >
iAIDA::AIDA_HBookStore::HBookDirectoryManager::histograms1D( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( memdir );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::HISTO1D,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    // check if it is a profile or not
    iAIDA::AIDA_HBookStore::HBook::loadObjectIntoMemory( iObj );
    if ( iAIDA::AIDA_HBookStore::HBook::typeOfObject( iObj ) == iAIDA::AIDA_HBookStore::HBook::HISTO1D ) {
      result.push_back( iObj );
    }
    iAIDA::AIDA_HBookStore::HBook::deleteFromMemory( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
iAIDA::AIDA_HBookStore::HBookDirectoryManager::histograms2D( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
  }
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::HISTO2D,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    result.push_back( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
iAIDA::AIDA_HBookStore::HBookDirectoryManager::profiles( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( memdir );
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::PROFILE,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0  ) break;
    // check if it is a profile or not
    iAIDA::AIDA_HBookStore::HBook::loadObjectIntoMemory( iObj );
    if ( iAIDA::AIDA_HBookStore::HBook::typeOfObject( iObj ) == iAIDA::AIDA_HBookStore::HBook::PROFILE ) {
      result.push_back( iObj );
    }
    iAIDA::AIDA_HBookStore::HBook::deleteFromMemory( iObj );
    iObjPrev = iObj;
  }
  return result;
}


std::vector< int >
iAIDA::AIDA_HBookStore::HBookDirectoryManager::tuples( const std::string& directory ) const
{
  std::vector< int > result;
  if ( ! existsDirectory( directory ) ) return result;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
  }
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::NTUPLE,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0 ) break;
    result.push_back( iObj );
    iObjPrev = iObj;
  }
  return result;
}


void
iAIDA::AIDA_HBookStore::HBookDirectoryManager::copyDirectoryStructureInMemory( const std::string& directory ) const
{
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  std::string memdir = "//PAWC/" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) {
    dir += "/" + names[iName];
    memdir += "/" + names[iName];
  }
  std::set<std::string> subDirs;
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iObj = 0;
  int iObjPrev = 0;
  while ( true ) {
    iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iObj,
								  iAIDA::AIDA_HBookStore::HBook::DIRECTORY,
								  objectType, foundTitle );
    if ( iObj == iObjPrev || iObj == 0 ) break;
    subDirs.insert( foundTitle );
    iObjPrev = iObj;
  }

  iAIDA::AIDA_HBookStore::HBook::changeDirectory( memdir );
  for ( std::set<std::string>::const_iterator iSubDir = subDirs.begin();
	iSubDir != subDirs.end(); ++iSubDir ) {
    iAIDA::AIDA_HBookStore::HBook::makeNewDirectory( *iSubDir );
  }

  for ( std::set<std::string>::const_iterator iSubDir = subDirs.begin();
	iSubDir != subDirs.end(); ++iSubDir ) {
    copyDirectoryStructureInMemory( directory + "/" + ( *iSubDir ) );
  }
}


bool
iAIDA::AIDA_HBookStore::HBookDirectoryManager::isDirectoryEmpty( const std::string& directory ) const
{
  if ( ! existsDirectory( directory ) ) return true;
  std::vector< std::string > names = iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater().formPathNames( directory );
  std::string dir = "//" + m_zebraName;
  for ( unsigned int iName = 0; iName < names.size(); ++iName ) dir += "/" + names[iName];
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( dir );
  iAIDA::AIDA_HBookStore::HBook::ObjectType objectType;
  std::string foundTitle = "";
  int iDir = 0;
  iAIDA::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory( iDir,
								iAIDA::AIDA_HBookStore::HBook::UNKNOWN,
								objectType, foundTitle );
  if ( iDir == 0 ) return true;
  else return false;
}
