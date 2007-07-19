// Emacs -*- C++ -*-
#ifndef AIDANATIVE_STOREXML
#define AIDANATIVE_STOREXML 1

#include <vector>
#include <string>
#include <map>
#include <set>
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IBackingStore.h"

// type ahead declarations 
namespace AIDA {
  class IManagedObject;
  class ITuple;
  namespace Dev {
    class IDevManagedObject;
  }
}

namespace DataXML { 
  class DataObject; 
}

namespace iAIDA {
  namespace AIDA_MemoryStore { 
    class MemoryBackingStore; 
  }

  namespace AIDA_XMLStore {

    class StoreTranslator; 

/// Implementation of AIDA::Dev::IStore interface for a XML store

class AIDA_StoreXML : virtual public AIDA::Dev::IStore,
		       virtual public AIDA::Dev::IBackingStore
{
public:
  /// Constructor
  AIDA_StoreXML( const std::string& name,
		 bool readOnly,
		 bool createNew,
		 const std::string& options );
  /// Destructor
  ~AIDA_StoreXML();

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

  // helper methods 

  /// write all objects to XML
  bool writeAll();

  /// read all objects from the XML file
  bool readAllObjects( ); 
  /// create a dev AIDA Managed Object from an XML element 
  AIDA::Dev::IDevManagedObject * createFromXML(const DataXML::DataObject * xmlObj, std::string & path); 
  /// append Managed Object to XML stream
  bool writeToXML(AIDA::Dev::IDevManagedObject * object, const std::string & path); 

  //// create backing store 
  void createBackingStore();

  std::string m_name;
  iAIDA::AIDA_XMLStore::StoreTranslator * m_outputTranslator; 

  std::map< std::string, std::string > m_objectTypes;
  std::map< std::string, AIDA::Dev::IDevManagedObject* > m_objectRefs;
  std::set< std::string > m_objectsToDelete;
  std::set< std::string > m_objectsToAdd;
  std::set< std::string > m_objectsToUpdate;

  // backing store 
  iAIDA::AIDA_MemoryStore::MemoryBackingStore*     m_backingStore;

  //bool m_compress; 
  int m_compressLevel;

  bool m_readOnly; 

  std::string histoPluginType; 
  std::string tuplePluginType; 
  std::string funcPluginType; 
  std::string dpsPluginType; 

  enum { UNCOMPRESSION, GZIPCOMPRESSION, BZIPCOMPRESSION };


};

  }
}

#endif /* ifndef AIDANATIVE_StoreXML */
