#include "MemoryFunctionCopier.h"
#include "AIDA_Dev/IDevFunctionFactory.h"
#include "AIDA_Dev/IDevFunction.h"
#include <typeinfo>


iAIDA::AIDA_MemoryStore::MemoryFunctionCopier::MemoryFunctionCopier(  ){}


AIDA::Dev::IDevManagedObject*
iAIDA::AIDA_MemoryStore::MemoryFunctionCopier::createCopy( const AIDA::IManagedObject& dataObject, AIDA::Dev::IDevFunctionFactory& factory )
{

  try {
    const AIDA::IFunction& function = dynamic_cast< const AIDA::IFunction& >( dataObject );
    return factory.cloneFunction( &( const_cast< AIDA::IFunction& >( function ) ) );
  }
  catch( std::bad_cast ) {
  }		      
  return 0;
}
