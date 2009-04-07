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

#include "CompiledExpressionManager.h"
#include "SymbolManager.h"
#include "ExpressionParser.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

//#include <iostream>

#include <fstream>
#include <set>

#include <cstdlib>

#ifdef USE_SEAL
#include "SealBase/StringOps.h"
#include "SealBase/ShellEnvironment.h"
#endif

//#include <iostream>

static const std::string filterSymbolPrefix = "pi_tuple_filter_";
static const std::string evaluatorSymbolPrefix = "pi_tuple_evaluator_";


iAIDA::AIDA_Tuple_native::CompiledExpressionManager::CompiledExpressionManager():
  m_cacheSizeForFilterObjects( iAIDA::AIDA_Tuple_native::CompiledExpressionManager::defaultCacheSizeForFilterObjects ),
  m_cacheSizeForEvaluatorObjects( iAIDA::AIDA_Tuple_native::CompiledExpressionManager::defaultCacheSizeForEvaluatorObjects ),
  m_symbolManager( new iAIDA::AIDA_Tuple_native::SymbolManager ),
  m_expressionParser( new iAIDA::AIDA_Tuple_native::ExpressionParser )
{

  std::set<std::string> includePaths;
  char* aidadir = std::getenv( "AIDA_DIR" );
  if ( aidadir != 0 ) {
    includePaths.insert( std::string( aidadir ) );
  }

  // look for include path from LD_LIBRARY_PATH
  std::vector<std::string> incPaths = findIncludePaths("PI"); 
  if (incPaths.size() != 0) { 
    for (unsigned int i = 0; i < incPaths.size(); ++i)  
      includePaths.insert( incPaths[i]); 
    
  }
  else { 
    // look at env variables 
    char* reldir = std::getenv( "PI" );
    if ( reldir != 0 ) {
      includePaths.insert( std::string( reldir ) + "/include" );
      } 
  }

  char* tupledir = std::getenv( "AIDA_TUPLE_DIR" );
  if ( tupledir != 0 ) {
    includePaths.insert( std::string( tupledir ) );
  }
  char* aida_devdir = std::getenv( "AIDA_DEV_DIR" );
  if ( aida_devdir != 0 ) {
    includePaths.insert( std::string( aida_devdir ) );
  }


  std::set<std::string> headerFiles;
  headerFiles.insert("<cmath>");
  headerFiles.insert("\"AIDA_Dev/IDevTuple.h\"");

  std::set<std::string> namespaces;
  namespaces.insert("std");

  m_expressionParser->useHeaderFiles( headerFiles );
  m_expressionParser->useNamespaces( namespaces );
  m_symbolManager->setIncludePaths( includePaths );
}


iAIDA::AIDA_Tuple_native::CompiledExpressionManager::~CompiledExpressionManager()
{
  if ( m_expressionParser ) delete m_expressionParser;
  if ( m_symbolManager ) {
    for ( std::map<std::string, iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory*>::iterator i = m_filterFactories.begin();
	  i != m_filterFactories.end(); ++i ) {
      m_symbolManager->destroyAndUnload( i->second );
    }
    for ( std::map<std::string, iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory*>::iterator i = m_evaluatorFactories.begin();
	  i != m_evaluatorFactories.end(); ++i ) {
      m_symbolManager->destroyAndUnload( i->second );
    }
  }
}


std::auto_ptr<iAIDA::AIDA_Tuple_native::IFilterExpressionBase>
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::createFilterExpression( const std::string& expression,
									      const std::map<std::string, std::string>& variableTypes,
									      std::set<std::string>& variablesUsed )
{
  // Determine the most used expression
  int nMax = 0;
  for ( std::map<std::string, int>::const_iterator i = m_filterFactoryUsage.begin(); i != m_filterFactoryUsage.end(); ++i ) {
    const int n = i->second;
    if ( n > nMax ) nMax = n;
  }

  // Check whether the expression already exists
  std::map<std::string, iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory*>::iterator iFilterFactory = m_filterFactories.find( expression );
  if ( iFilterFactory != m_filterFactories.end() ) {
    if ( m_filterFactoryUsage[expression] < nMax ) {
      m_filterFactoryUsage[expression] = nMax + 1;
      for ( std::map<std::string, int>::iterator i = m_filterFactoryUsage.begin(); i != m_filterFactoryUsage.end(); ++i ) {
	i->second -= 1;
      }
    }
    variablesUsed = m_filterFactoryVariables.find( expression )->second;
    return std::auto_ptr<iAIDA::AIDA_Tuple_native::IFilterExpressionBase>( iFilterFactory->second->create() );
  }

  std::string symbolName = "";
  if ( m_filterFactoryUsage.size() > 0 ) { // check if the maximum number of expressions is already reached.
    if ( static_cast<int>(m_filterFactoryUsage.size()) < m_cacheSizeForFilterObjects ) {
      nMax = m_filterFactoryUsage.size();
    }
    else { // find the one with the lowest usage
      for ( std::map<std::string, int>::const_iterator i = m_filterFactoryUsage.begin(); i != m_filterFactoryUsage.end(); ++i ) {
	if ( i->second == 0 ) {
	  symbolName = i->first;
	  break;
	}
      }
    }
  }

  // remove an old symbol if needed
  if ( symbolName.size() > 0 ) {
    m_symbolManager->destroyAndUnload( m_filterFactories[ symbolName ] );
    m_filterFactories.erase( symbolName );
    m_filterFactoryUsage.erase( symbolName );
    m_filterFactoryVariables.erase( symbolName );
  }
  else { // Form the symbol name
    std::ostringstream osSymbol;
    int nFilter = std::rand(); 
    osSymbol << filterSymbolPrefix << nFilter << "_" << nMax;
#ifndef BADENDS
    osSymbol << std::ends;
#endif
    symbolName = osSymbol.str();
  }

  const std::string symbolFile = symbolName + ".cpp";

  // create a new symbol
  std::ofstream fout( symbolFile.c_str() );
  variablesUsed = m_expressionParser->logicalEvaluation( expression, variableTypes, symbolName, fout );


  iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory* factoryPointer = m_symbolManager->makeAndLoadFilter( symbolFile, symbolName );

  // return the actual object
  iAIDA::AIDA_Tuple_native::IFilterExpressionBase* p = 0;
  if ( factoryPointer ) {
    p = factoryPointer->create();
    m_filterFactories.insert( std::make_pair( expression, factoryPointer ) );
    m_filterFactoryUsage.insert( std::make_pair( expression, static_cast<int>(m_filterFactories.size()) ) );
    m_filterFactoryVariables.insert( std::make_pair( expression, variablesUsed ) );
  }
  return std::auto_ptr<iAIDA::AIDA_Tuple_native::IFilterExpressionBase>( p );
}


std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase>
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::createEvaluatorExpression( const std::string& expression,
										 const std::map<std::string, std::string>& variableTypes,
										 std::set<std::string>& variablesUsed )
{
  // Determine the most used expression
  int nMax = 0;
  for ( std::map<std::string, int>::const_iterator i = m_evaluatorFactoryUsage.begin(); i != m_evaluatorFactoryUsage.end(); ++i ) {
    const int n = i->second;
    if ( n > nMax ) nMax = n;
  }

  // Check whether the expression already exists
  std::map<std::string, iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory*>::iterator iEvaluatorFactory = m_evaluatorFactories.find( expression );
  if ( iEvaluatorFactory != m_evaluatorFactories.end() ) {
    if ( m_evaluatorFactoryUsage[expression] < nMax ) {
      m_evaluatorFactoryUsage[expression] = nMax + 1;
      for ( std::map<std::string, int>::iterator i = m_evaluatorFactoryUsage.begin(); i != m_evaluatorFactoryUsage.end(); ++i ) {
	i->second -= 1;
      }
    }
    variablesUsed = m_evaluatorFactoryVariables.find( expression )->second;
    return std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase>( iEvaluatorFactory->second->create() );
  }

  std::string symbolName = "";
  if ( m_evaluatorFactoryUsage.size() > 0 ) { // check if the maximum number of expressions is already reached.
    if ( static_cast<int>(m_evaluatorFactoryUsage.size()) < m_cacheSizeForEvaluatorObjects ) {
      nMax = m_evaluatorFactoryUsage.size();
    }
    else { // find the one with the lowest usage
      for ( std::map<std::string, int>::const_iterator i = m_evaluatorFactoryUsage.begin(); i != m_evaluatorFactoryUsage.end(); ++i ) {
	if ( i->second == 0 ) {
	  symbolName = i->first;
	  break;
	}
      }
    }
  }

  // remove an old symbol if needed
  if ( symbolName.size() > 0 ) {
    m_symbolManager->destroyAndUnload( m_evaluatorFactories[ symbolName ] );
    m_evaluatorFactories.erase( symbolName );
    m_evaluatorFactoryUsage.erase( symbolName );
    m_evaluatorFactoryVariables.erase( symbolName );
  }
  else { // Form the symbol name
    std::ostringstream osSymbol;
    // LM : count all filters not just for same expressions
    int nEval = std::rand();
    osSymbol << evaluatorSymbolPrefix << nEval << "_" << nMax;
#ifndef BADENDS
    osSymbol << std::ends;
#endif
    symbolName = osSymbol.str();
  }

  const std::string symbolFile = symbolName + ".cpp";

  // create a new symbol
  std::ofstream fout( symbolFile.c_str() );
  variablesUsed = m_expressionParser->mathematicalEvaluation( expression, variableTypes, symbolName, fout );
  iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory* factoryPointer = m_symbolManager->makeAndLoadEvaluator( symbolFile, symbolName );

  // return the actual object
  iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase* p = 0;
  if ( factoryPointer ) {
    p = factoryPointer->create();
    m_evaluatorFactories.insert( std::make_pair( expression, factoryPointer ) );
    m_evaluatorFactoryUsage.insert( std::make_pair( expression, static_cast<int>(m_evaluatorFactories.size()) ) );
    m_evaluatorFactoryVariables.insert( std::make_pair( expression, variablesUsed ) );
  }
  return std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase>( p );
}


int
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::cacheSizeForFilterObjects()
{
  return m_cacheSizeForFilterObjects;
}


int
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::cacheSizeForEvaluatorObjects()
{
  return m_cacheSizeForEvaluatorObjects;
}


void
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::setCacheSizeForFilterObjects( int cacheSize )
{
  m_cacheSizeForFilterObjects = cacheSize;
}


void
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::setCacheSizeForEvaluatorObjects( int cacheSize )
{
  m_cacheSizeForEvaluatorObjects = cacheSize;
}


std::vector<std::string> 
iAIDA::AIDA_Tuple_native::CompiledExpressionManager::findIncludePaths(const std::string & relName) { 

  std::vector<std::string> incPaths; 
  // get environment variable
  std::string envVar = "LD_LIBRARY_PATH";

  /*-ap-begin
  if (! seal::ShellEnvironment().has(envVar) ) { 
    //std::cout << "Error - " << envVar << " is not defined ! \n"; 
    return incPaths; 
  }
  std::string value = seal::ShellEnvironment().get(envVar);

  seal::StringList items = seal::StringOps::split (value, ":"); 

  // find top variable 
  std::string searchString = relName + "_";  
  int expectedPos = 2; 
  std::string selectedItem; 
  //std::cout << "Look for first occurence of string " <<  searchString << std::endl;     
  for (unsigned int i = 0; i < items.size() ; ++i) { 
    // tokenize with "/"
    seal::StringList subItems = seal::StringOps::split (items[i], "/"); 
    // look for searchString IAIDA_   (expect to have IAIDA_XXX/platf/lib (items must be at least 3) 
    if (subItems.size() > 2) { 
     int j = subItems.size() -1 - expectedPos; 
     if (subItems[j].find(searchString) != std::string::npos ) { 
       selectedItem = items[i]; 
	  break; 
     }
    }
  }


  //std::cout << "Select : " << selectedItem << std::endl;
  seal::StringList subItems = seal::StringOps::split (selectedItem, "/"); 
  // delete last items after releases to get arch 
  
  subItems.pop_back(); 

  // for architecture dep includes 
  subItems.push_back("include");
 
  std::string archIncludeDir = seal::StringOps::join( subItems, "/");
  incPaths.push_back( archIncludeDir); 

  // remove the last two
  subItems.pop_back();
  subItems.pop_back();
  subItems.push_back("include");
  std::string includeDir = seal::StringOps::join( subItems, "/");
  incPaths.push_back( includeDir); 
  //std::cout << "release Dir for  " << relName << " = " << includeDir << std::endl;
  
  end-ap-*/

  return incPaths; 

}
