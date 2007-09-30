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
 * \file Implementation of class TitleStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 22:45:52 2002
// 



#include "TitleStyle.h"



namespace AidaNative {



/////////////
//         //
// Statics //
//         //
/////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
TitleStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  // (Example) Parameter1: 
  //
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "on");
  //
  // Parameter 2: Something else...
  // ...
  OptionList opts;
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

TitleStyle::TitleStyle(const std::string& nameOfDefaultTable)
  : parameterHandler    (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    m_textStyle         (TextStyle       (nameOfDefaultTable))
{
  // no-op
}


TitleStyle::~TitleStyle()
{
  // no-op
}



////////////////////
//                //
// Public methods //
//                //
////////////////////

void TitleStyle::reset()
{
  parameterHandler.reset();
  m_textStyle.reset();
}




bool TitleStyle::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  return OK;
}




std::vector<std::string> TitleStyle::availableParameters() const
{
  return parameterHandler.availableParameters();
}




std::vector<std::string> TitleStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (OK) return vec;
  else return std::vector<std::string>();
}




std::string TitleStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return "";
}



TextStyle& TitleStyle::textStyle()
{
  return m_textStyle;
}




bool TitleStyle::setTextStyle(const TextStyle& textStyle)
{
  m_textStyle = textStyle;
  return true;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

