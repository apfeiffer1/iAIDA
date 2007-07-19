// -*- C++ -*-

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  This library is free software; you can redistribute it and/or      //
//  modify it under the terms of the GNU Lesser General Public         //
//  License as published by the Free Software Foundation; either       //
//  version 2.1 of the License, or (at your option) any later version. //
//                                                                     //
//  This library is distributed in the hope that it will be useful,    //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of     //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   //
//  Lesser General Public License for more details.                    //
//                                                                     //
//  You should have received a copy of the GNU Lesser General Public   //
//  License along with this library (see file COPYING); if not, write  //
//  to the Free Software Foundation, Inc., 59 Temple Place, Suite      //
//  330, Boston, MA 02111-1307 USA, or contact the author.             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/**
 *
 * \file Implementation of class ParameterTable
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Aug  7 17:41:14 2002
// 



#include "ParameterTable.h"




namespace AidaNative {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

ParameterTable::ParameterTable()
{
  // no-op
}




ParameterTable::~ParameterTable()
{
  theMap.clear();
}




ParameterTable::ParameterTable(const ParameterTable& rhs)
  : theMap(rhs.theMap)
{
  // no-op
}




ParameterTable& ParameterTable::operator=(const ParameterTable& rhs)
{
  if (this != &rhs) {
    theMap.clear();
    theMap = rhs.theMap;
  }
  return *this;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void ParameterTable::reset()
{
  theMap.clear();
}




bool ParameterTable::addParameter(const std::string& parameterName, 
				  const std::vector<std::string>& possibleOptions,
				  const std::string& defaultValue)
{
  if (parameterName == "") return false;
  std::string par = parameterName;
  StringList opts = possibleOptions;
  theMap[par] = std::make_pair(opts, defaultValue);
  return true;
}




std::vector<std::string> ParameterTable::availableParameters() const
{
  StringList v;
  for (SMapCIt i = theMap.begin(); i != theMap.end(); ++i) v.push_back(i->first);
  return v;
}




bool ParameterTable::availableOptions(const std::string& param,
				      std::vector<std::string>& theOptions) const
{
  SMapCIt i = theMap.find(param);
  if (theMap.end() == i) return false;
  else {
    theOptions.clear();
    theOptions = std::vector<std::string>(i->second.first);
    return true;
  }
}




bool ParameterTable::getDefaultValue(const std::string& parameterName,
				     std::string& theDefault) const
{
  SMapCIt i = theMap.find(parameterName);
  if (theMap.end() == i) return false;
  else {
    theDefault = i->second.second;
    return true;
  }
}




bool ParameterTable::setDefaultValue(const std::string& parameterName,
				     const std::string& newDefault)
{
  SMapIt i = theMap.find(parameterName);
  if (theMap.end() == i) return false;
  else {
    i->second.second = newDefault;
    return true;
  }
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

