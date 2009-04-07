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

// Project   : PI
// Package   : AIDA_Native
// Author    : Lorenzo.Moneta@cern.ch 
// Created by: moneta  at Fri Apr 11 16:11:18 2003

#ifndef AIDA_NATIVE_TREEPLUGIN_H
#define AIDA_NATIVE_TREEPLUGIN_H 1


#include "AIDA/ITreeFactory.h"
#include "AIDA/ITree.h"
#include "AIDA_Plugin/AIDA_PluginType.h"



namespace AIDA_Native 
{ 

/**
 * Implementation of AIDA_Plugin for AIDA_Native Tree
 * This class does not support copying.
 */
  class TreePlugin : virtual public AIDA_Plugin::AIDA_PluginType 
{

public: 
  TreePlugin( ); 
  virtual ~TreePlugin(); 

  /**  
   *  create AIDA Tree factory 
   **/
  

  AIDA::ITreeFactory * createTreeFactory( ); 

  

private:
  /// copying unimplemented in this class.  
  TreePlugin(const TreePlugin &); 
  /// copying unimplemented in this class.  
  TreePlugin & operator = (const TreePlugin &); 
  

}; 

} // end namespace AIDA_native
#endif 
