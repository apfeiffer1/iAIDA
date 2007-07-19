// Project   : LCG
// Package   : PluginHBookStore
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Tue Sep 30 00:22:40 2003

#ifndef PI_AIDA_HBOOKSTORE_HBOOKSTOREPLUGIN_H
#define PI_AIDA_HBOOKSTORE_HBOOKSTOREPLUGIN_H 1

#include "AIDA_HBookStore/AIDA_HBookStoreFactory.h"
// #include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Dev/IStoreFactory.h"

namespace pi {
  namespace AIDA_HBookStore {


/**
 * PUT YOUR COMMENT HERE.
 * This class does not support copying.
 */
class HBookStorePlugin : virtual public AIDA_Plugin::AIDA_PluginType  
{

public: 
  HBookStorePlugin() {}
  virtual ~HBookStorePlugin() {}


 public: 

  AIDA::Dev::IStoreFactory * createStoreFactory()  
    {
      return new pi::AIDA_HBookStore::AIDA_HBookStoreFactory(); 
    }


}; 

} // end namespace AIDA_HBookStore
} // end namespace pi
#endif 
