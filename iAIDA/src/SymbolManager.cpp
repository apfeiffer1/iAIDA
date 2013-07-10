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

#include "SymbolManager.h"
#include "AIDA_Tuple_native/IEvaluatorExpressionBase.h"
#include "AIDA_Tuple_native/IFilterExpressionBase.h"

#include <cstdlib>
#ifndef _WIN32
#include <dlfcn.h>
#endif

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

#ifdef BUILDVERBOSE
#include <iostream>
#endif

iAIDA::AIDA_Tuple_native::SymbolManager::SymbolManager()
{}

iAIDA::AIDA_Tuple_native::SymbolManager::~SymbolManager()
{
  for ( std::map< IEvaluatorExpressionBaseFactory*, void* >::const_iterator iHandle = m_evaluatorHandles.begin();
	iHandle != m_evaluatorHandles.end(); ++iHandle ) {
#ifndef _WIN32
    dlclose( iHandle->second );
#endif
  }
  for ( std::map< IFilterExpressionBaseFactory*, void* >::const_iterator iHandle = m_filterHandles.begin();
	iHandle != m_filterHandles.end(); ++iHandle ) {
#ifndef _WIN32
    dlclose( iHandle->second );
#endif
  }
}

bool
iAIDA::AIDA_Tuple_native::SymbolManager::setIncludePaths( const std::set<std::string>& includes )
{
  m_includePaths = includes;
  return true;
}

const std::set<std::string>&
iAIDA::AIDA_Tuple_native::SymbolManager::includePaths() const
{
  return m_includePaths;
}

iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory*
iAIDA::AIDA_Tuple_native::SymbolManager::makeAndLoadEvaluator( const std::string& fileName,
								const std::string& symbolName )
{
#ifndef _WIN32
  const std::string libraryName = fileName + ".so";
  std::string systemCall;
#if defined LINUX
  systemCall = "g++ -shared ";
#elif defined SOLARIS 
  systemCall = "CC -G -lCrun ";
#endif
  systemCall += fileName + " -o " + libraryName;
  for ( std::set<std::string>::const_iterator iPath = m_includePaths.begin();
	iPath != m_includePaths.end(); ++iPath ) {
    systemCall += " -I" + (*iPath);
  }

#ifdef BUILDVERBOSE
  std::cout << systemCall << std::endl;
#endif


  const std::string rmSystemCall = "rm -f " + fileName;
  const std::string rmLibSystemCall = "rm -f " + libraryName;

#ifdef BUILDVERBOSE
  std::cout << rmSystemCall << std::endl;
  std::cout << rmLibSystemCall << std::endl;
#endif

  if ( std::system( systemCall.c_str() ) != 0 ) {
    std::system( rmSystemCall.c_str() );
    return 0;
  }


  // load the symbol
  void* ldHandle = dlopen( ( "./" + libraryName ).c_str(), RTLD_LAZY );
  char* error = dlerror();
  if ( error != NULL ) return 0;
  void* p = dlsym( ldHandle, symbolName.c_str() );
  if ( dlerror() != NULL ) {
    dlclose( ldHandle );
    std::system( rmSystemCall.c_str() );
    std::system( rmLibSystemCall.c_str() );
    return 0;
  }

  iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory* factory = *(reinterpret_cast<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory**>( p ) );
  std::system( rmSystemCall.c_str() );
  std::system( rmLibSystemCall.c_str() );

  // Register the symbol
  m_evaluatorHandles[ factory ] = ldHandle;
  return factory;
#else 
  return 0;
#endif
}

bool
iAIDA::AIDA_Tuple_native::SymbolManager::destroyAndUnload( IEvaluatorExpressionBaseFactory* factory )
{
#ifndef _WIN32
  dlclose( m_evaluatorHandles[ factory ] );
  m_evaluatorHandles.erase( factory );
  return true;
#else
  return false;
#endif
}

iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory*
iAIDA::AIDA_Tuple_native::SymbolManager::makeAndLoadFilter( const std::string& fileName,
							     const std::string& symbolName )
{
#ifndef _WIN32
  const std::string libraryName = fileName + ".so";
  std::string systemCall;
#if defined LINUX
  systemCall = "g++ -shared ";
#elif defined SOLARIS 
  systemCall = "CC -G -lCrun ";
#endif
  systemCall += fileName + " -o " + libraryName;
  for ( std::set<std::string>::const_iterator iPath = m_includePaths.begin();
	iPath != m_includePaths.end(); ++iPath ) {
    systemCall += " -I" + (*iPath);
  }


  const std::string rmSystemCall = "rm -f " + fileName;
  const std::string rmLibSystemCall = "rm -f " + libraryName;

  if ( std::system( systemCall.c_str() ) != 0 ) {
    std::system( rmSystemCall.c_str() );
    return 0;
  }

  // load the symbol
  void* ldHandle = dlopen( ( "./" + libraryName ).c_str(), RTLD_LAZY );
  char* error = dlerror();
  if ( error != NULL ) return 0;
  void* p = dlsym( ldHandle, symbolName.c_str() );
  if ( dlerror() != NULL ) {
    dlclose( ldHandle );
    std::system( rmSystemCall.c_str() );
    std::system( rmLibSystemCall.c_str() );
    return 0;
  }

  iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory* factory = *(reinterpret_cast<iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory**>( p ) );
  std::system( rmSystemCall.c_str() );
  std::system( rmLibSystemCall.c_str() );

  // Register the symbol
  m_filterHandles[ factory ] = ldHandle;
  return factory;
#else
  return 0;
#endif
}

bool
iAIDA::AIDA_Tuple_native::SymbolManager::destroyAndUnload( IFilterExpressionBaseFactory* factory )
{
#ifndef _WIN32
  dlclose( m_filterHandles[ factory ] );
  m_filterHandles.erase( factory );
  return true;
#else
  return false;
#endif
}
