// Project   : LCG
// Package   : AIDA_Plugin
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Fri Sep 19 11:23:48 2003

#ifndef AIDA_PLUGIN_AIDA_PLUGINLOADER_H
#define AIDA_PLUGIN_AIDA_PLUGINLOADER_H 1

#include "boost/pool/detail/singleton.hpp"

namespace AIDA_Plugin
{ 

  class  AIDA_PluginType;

/**
 * interface to seal plugin manager. Used to load plug-ins
 */
class AIDA_PluginLoader 
{

public: 
  AIDA_PluginLoader(); 
  virtual ~AIDA_PluginLoader(); 

  /* 
   *  load plugin of a certain type
   */ 

  AIDA_PluginType * load (const std::string & type); 

private:
  /// copying unimplemented in this class.  
  AIDA_PluginLoader(const AIDA_PluginLoader &); 
  /// copying unimplemented in this class.  
  AIDA_PluginLoader & operator = (const AIDA_PluginLoader &); 

}; 

// make a class a singleton 

//typedef Loki::SingletonHolder<AIDA_PluginLoader> AIDA_PluginManager;
typedef boost::details::pool::singleton_default<AIDA_PluginLoader> AIDA_PluginManager;

} // end namespace AIDA_Plugin
#endif 
