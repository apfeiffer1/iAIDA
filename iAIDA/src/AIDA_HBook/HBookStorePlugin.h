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
// Package   : PluginHBookStore
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Tue Sep 30 00:22:40 2003

#ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREPLUGIN_H
#define IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREPLUGIN_H 1

#include "AIDA_HBookStore/AIDA_HBookStoreFactory.h"
// #include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
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
      return new iAIDA::AIDA_HBookStore::AIDA_HBookStoreFactory(); 
    }


}; 

} // end namespace AIDA_HBookStore
} // end namespace iAIDA
#endif 
