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
#ifndef IAIDA_MEMORYSTORE
#define IAIDA_MEMORYSTORE 1

#include <vector>
#include <string>
#include <map>
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IBackingStore.h"

// type ahead declarations 
namespace AIDA {
  class IManagedObject;
  class ITuple;
}

namespace iAIDA {
  namespace AIDA_MemoryStore {
    class MemoryBackingStore;

/// Implementation of AIDA::Dev::IStore interface for a memory store

class AIDA_StoreMemory : virtual public AIDA::Dev::IStore,
			 virtual public AIDA::Dev::IBackingStore
{
public:
  /// Constructor
  AIDA_StoreMemory( const std::string& name );
  /// Destructor
  ~AIDA_StoreMemory();

  /// Methods inherited from the IStore class
  const std::string& name() const;
  bool writeObject( const AIDA::IManagedObject& dataObject,
		    const std::string& path );
  AIDA::IManagedObject* copyAndWrite( const AIDA::IManagedObject& dataObject,
				      const std::string& path );
  AIDA::IManagedObject* retrieveObject( const std::string & path );
  bool removeObject( const std::string& path );
  bool moveObject(const std::string& from, const std::string& to );
  bool commit();
  bool close();
  std::vector< std::string > listObjectPaths( const std::string directory,
					      bool recursive = false ) const;
  std::vector< std::string > listObjectTypes( const std::string directory,
					      bool recursive = false ) const;
  bool canMoveTuples() const;
  bool canCopyTuples() const;

  /// Methods inherited from the IBackingStore class
  bool writeTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool readTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex );
  void* variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex );
  const void* variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const;
  void setVariableAddress( const AIDA::Dev::ITupleHeader& , int , void *  ) { /* not impl.*/ }
  bool clearBindings( const AIDA::Dev::ITupleHeader& header );
  bool writeTupleRow( AIDA::Dev::ITupleHeader& header );
  bool readTupleRow( AIDA::Dev::ITupleHeader& header );  
  bool resetTuple( AIDA::Dev::ITupleHeader& header );

private:
  std::string m_name;
  std::map< std::string, std::string > m_objectTypes;
  MemoryBackingStore*                  m_backingStore;

  std::string histoPluginType; 
  std::string tuplePluginType; 
  std::string funcPluginType; 
  std::string dpsPluginType; 
};


  }
}

#endif /* ifndef IAIDA_MEMORYSTORE */
