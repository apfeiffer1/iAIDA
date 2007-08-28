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
