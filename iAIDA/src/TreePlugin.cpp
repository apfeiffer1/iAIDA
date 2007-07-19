// Project   : PI
// Package   : AIDA_Native
// Author    : Lorenzo.Moneta@cern.ch 
// Created by: moneta  at Fri Apr 11 16:11:18 2003


#include "TreePlugin.h"
#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Tree/AIDA_TreeFactory.h"



namespace AIDA_Native
{ 

TreePlugin::TreePlugin() 
{
}

TreePlugin::~TreePlugin() 
{
}


TreePlugin::TreePlugin(const TreePlugin &) 
{
}

TreePlugin & TreePlugin::operator = (const TreePlugin &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

// create using AIDA_native Analysis factory 

AIDA::ITreeFactory *  TreePlugin::createTreeFactory( ) 
{ 

  return new iAIDA::aida_tree::AIDA_TreeFactory( );

}





} // end namespace Aida_Native
