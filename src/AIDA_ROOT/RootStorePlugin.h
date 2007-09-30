// Project   : LCG
// Package   : AIDA_XMLStore
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Tue Sep 30 00:22:40 2003

#ifndef AIDA_ROOTSTORE_ROOTSTOREPLUGIN_H
#define AIDA_ROOTSTORE_ROOTSTOREPLUGIN_H 1

#include "AIDA_RootStoreFactory.h"
#include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_RootStore {


/**
 * PUT YOUR COMMENT HERE.
 * This class does not support copying.
 */
class RootStorePlugin : virtual public AIDA_Plugin::AIDA_PluginType  
{

public: 
  RootStorePlugin() {}
  virtual ~RootStorePlugin() {}


 public: 

  AIDA::Dev::IStoreFactory * createStoreFactory()  
    {
      return new iAIDA::AIDA_RootStore::AIDA_RootStoreFactory(); 
    }


}; 

} // end namespace AIDA_RootStore
} // end namespace iAIDA
#endif 
