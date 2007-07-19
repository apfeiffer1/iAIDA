// Emacs -*- C++ -*-
#ifndef PI_AIDA_HBOOKSTORE_HBOOKSTORE
#define PI_AIDA_HBOOKSTORE_HBOOKSTORE 1

#include <vector>
#include <string>
#include <map>
#include <set>
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IBackingStore.h"

namespace AIDA {
  namespace Dev {
    class IDevManagedObject;
  }
}

namespace pi {
  namespace AIDA_HBookStore {
    class HBookFileHandler;
    class HBookMemoryCache;
    class HBookDirectoryManager;
    class HBookOptionParser;
    class IPersistentTuple;

/// Implementation of AIDA::Dev::IStore interface for a HBook store

class AIDA_StoreHBook : virtual public AIDA::Dev::IStore,
			virtual public AIDA::Dev::IBackingStore
{
public:
  /// Constructor
  AIDA_StoreHBook( const std::string& name,
		   bool readOnly,
		   bool createNew,
		   const std::string& options );
  /// Destructor
  ~AIDA_StoreHBook();

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
  std::string             m_name;
  bool                    m_isClosed;
  HBookOptionParser*      m_optionParser;
  HBookFileHandler*       m_fileHandler;
  HBookMemoryCache*       m_cache;
  HBookDirectoryManager*  m_dirManager;

  std::map< std::string, std::string > m_objectTypes;
  std::map< std::string, AIDA::Dev::IDevManagedObject* > m_objectRefs;
  std::set< std::string > m_objectsToDelete;
  std::set< std::string > m_objectsToAdd;
  std::set< std::string > m_objectsToUpdate;
  std::map< std::string, IPersistentTuple* > m_tuples;

  std::string histoPluginType; 
  std::string tuplePluginType; 

private:
  void collectObjectTypes( const std::string& directory );
  void commitDelete();
  void commitAdd();
  std::pair<std::string, int> directoryAndIdentifier( const std::string& path ) const;



};

  }
}

#endif /* ifndef PI_AIDA_HBOOKSTORE_HBOOKSTORE */
