#include "MemoryDataPointSetCopier.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "AIDA_Dev/IDevDataPointSet.h"
#include <typeinfo>


iAIDA::AIDA_MemoryStore::MemoryDataPointSetCopier::MemoryDataPointSetCopier()
{}


AIDA::Dev::IDevManagedObject*
iAIDA::AIDA_MemoryStore::MemoryDataPointSetCopier::createCopy( const AIDA::IManagedObject& dataObject,  AIDA::Dev::IDevDataPointSetFactory & factory )
{

  try {
    return factory.createCopy( dynamic_cast< const AIDA::IDataPointSet& >( dataObject ) );
  }
  catch( std::bad_cast ) {
  }		      
  return 0;
}
