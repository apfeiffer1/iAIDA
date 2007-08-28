// Emacs -*- C++ -*-
#ifndef AIDANATIVE_MEMORYSTORE
#define AIDANATIVE_MEMORYSTORE 1

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

#endif /* ifndef AIDANATIVE_MEMORYSTORE */
