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

// Project   : LCG
// Package   : AIDA_XMLStore
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Tue Sep 30 00:22:40 2003

#ifndef AIDA_XMLSTORE_XMLSTOREPLUGIN_H
#define AIDA_XMLSTORE_XMLSTOREPLUGIN_H 1

#include "AIDA_XMLStoreFactory.h"
#include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_XMLStore {


/**
 * PUT YOUR COMMENT HERE.
 * This class does not support copying.
 */
class XMLStorePlugin : virtual public AIDA_Plugin::AIDA_PluginType  
{

public: 
  XMLStorePlugin() {}
  virtual ~XMLStorePlugin() {}


 public: 

  AIDA::Dev::IStoreFactory * createStoreFactory()  
    {
      return new iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory(); 
    }


}; 

} // end namespace AIDA_XMLStore
} // end namespace iAIDA
#endif 
