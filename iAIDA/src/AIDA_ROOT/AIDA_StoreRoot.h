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
#ifndef IAIDA_AIDA_STOREROOT_STOREROOT
#define IAIDA_AIDA_STOREROOT_STOREROOT 1

#include <vector>
#include <string>
#include <map>
#include <set>
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IBackingStore.h"

// type ahead declarations 

class TFile; 

namespace AIDA {
  class IManagedObject;
  class ITuple;
  namespace Dev {
    class IDevManagedObject;
  }
}

namespace DataRoot { 
  class DataObject; 
}

namespace iAIDA {

  namespace AIDA_RootStore {

    class IPersistentTuple;

/// Implementation of AIDA::Dev::IStore interface for a Root store

class AIDA_StoreRoot : virtual public AIDA::Dev::IStore,
		       virtual public AIDA::Dev::IBackingStore
{
public:
  /// Constructor
  AIDA_StoreRoot( const std::string& name,
		 bool readOnly,
		 bool createNew,
		 const std::string& options );
  /// Destructor
  ~AIDA_StoreRoot();

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
  void setVariableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex , void * );
  bool clearBindings( const AIDA::Dev::ITupleHeader& header );
  bool writeTupleRow( AIDA::Dev::ITupleHeader& header );
  bool readTupleRow( AIDA::Dev::ITupleHeader& header );  
  bool resetTuple( AIDA::Dev::ITupleHeader& header );

private:

  // helper methods 



  /// read object types from the Root file (empy is root ) 
  void collectObjectTypes(const std::string & directory = ""); 

  /// append Managed Object to Root stream
  bool writeToRoot(AIDA::Dev::IDevManagedObject * object, const std::string & path); 
  /// retrieve directory and name from full path
  std::pair<std::string, std::string> getDirectoryAndName(const std::string & path);
  /// cd to directory specified by the path and create if not exists
  bool setCurrentDirectory( const std::string & path); 


  std::string m_name;
  bool m_readMode;  // if in readonly mode 

  std::map< std::string, std::string > m_objectTypes;
  std::map< std::string, AIDA::Dev::IDevManagedObject* > m_objectRefs;
  std::set< std::string > m_objectsToDelete;
  std::set< std::string > m_objectsToAdd;
  std::set< std::string > m_objectsToUpdate;

  std::map< std::string, IPersistentTuple* > m_tuples;  
  
  // root store (file) 
  TFile * m_store;


  std::string histoPluginType; 
  std::string tuplePluginType; 

};

  }
}

#endif 
