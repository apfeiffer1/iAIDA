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

#include "AIDA_XMLStoreFactory.h"
#include "AIDA_StoreXML.h"


#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define XML_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif


iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::AIDA_XMLStoreFactory()
{}

iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::~AIDA_XMLStoreFactory()
{}


AIDA::Dev::IStore*
iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::createStore( const std::string & storeName,
							  bool readOnly,
							  bool createNew,
							  const std::string & options )
{
#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    return new iAIDA::AIDA_XMLStore::AIDA_StoreXML( storeName, readOnly, createNew, options );
#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
  }
  catch ( std::exception& e ) {
    XML_STORE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


bool
iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

