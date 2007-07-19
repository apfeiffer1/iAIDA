#include "MemoryHistogramCopier.h"
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevCloud1D.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevProfile2D.h"
#include <typeinfo>


iAIDA::AIDA_MemoryStore::MemoryHistogramCopier::MemoryHistogramCopier( )
{}


AIDA::Dev::IDevManagedObject*
iAIDA::AIDA_MemoryStore::MemoryHistogramCopier::createCopy( const AIDA::IManagedObject& dataObject, AIDA::Dev::IDevHistogramFactory& factory, const std::string& type )
{

  try {
    if ( type == "IHistogram1D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::IHistogram1D& >( dataObject ) );
    }
    if ( type == "IHistogram2D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::IHistogram2D& >( dataObject ) );
    }
    if ( type == "IHistogram3D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::IHistogram3D& >( dataObject ) );
    }
    if ( type == "ICloud1D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::ICloud1D& >( dataObject ) );
    }
    if ( type == "ICloud2D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::ICloud2D& >( dataObject ) );
    }
    if ( type == "ICloud3D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::ICloud3D& >( dataObject ) );
    }
    if ( type == "IProfile1D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::IProfile1D& >( dataObject ) );
    }
    if ( type == "IProfile2D" ) {
      return factory.createCopy( dynamic_cast< const AIDA::IProfile2D& >( dataObject ) );
    }
  }
  catch( std::bad_cast ) {
  }		      
  return 0;
}
