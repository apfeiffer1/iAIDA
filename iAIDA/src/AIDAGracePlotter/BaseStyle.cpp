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
 * \file Implementation of class BaseStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 15:54:34 2002
// 




#include "BaseStyle.h"



namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable 
BaseStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  // typedef std::vector<std::string> OptionList;
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "off");
  //
  // Parameter 2: Something else...
  // ...
  ParameterTable t;
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

BaseStyle::BaseStyle(const std::string& nameOfDefaultTable)
  : parameterHandler(ParameterHandler(newDefaultPTable(nameOfDefaultTable)))
{
  // no-op
}




BaseStyle::~BaseStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void BaseStyle::reset() 
{
  parameterHandler.reset();
}




bool BaseStyle::setParameter(const std::string& par, const std::string& opt) 
{
  return parameterHandler.setParameter(par, opt);
}




std::string BaseStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (!OK) s = "";
  return s;
}




std::vector<std::string> BaseStyle::availableParameters() const 
{
  return parameterHandler.availableParameters();
}




std::vector<std::string> 
BaseStyle::availableOptions(const std::string& param) const 
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec.clear();
  return vec;
}




} // end of namespace AidaNative

