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
