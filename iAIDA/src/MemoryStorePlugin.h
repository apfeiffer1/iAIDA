// Project   : LCG
// Package   : AIDA_MemoryStore
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Tue Sep 30 00:22:40 2003

#ifndef AIDA_MEMORYSTORE_MEMORYSTOREPLUGIN_H
#define AIDA_MEMORYSTORE_MEMORYSTOREPLUGIN_H 1

#include "AIDA_MemoryStoreFactory.h"
#include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_MemoryStore {


/**
 * PUT YOUR COMMENT HERE.
 * This class does not support copying.
 */
class MemoryStorePlugin : virtual public AIDA_Plugin::AIDA_PluginType  
{

public: 
  MemoryStorePlugin() {}
  virtual ~MemoryStorePlugin() {}


 public: 

  AIDA::Dev::IStoreFactory * createStoreFactory()  
    {
      return new iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory(); 
    }


}; 

} // end namespace AIDA_MemoryStore
} // end namespace iAIDA
#endif 
