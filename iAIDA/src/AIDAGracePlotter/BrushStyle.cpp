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
 * \file Implementation of class BrushStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:09:57 2002
// 

#include <assert.h>

#include <algorithm>
#include "BrushStyle.h"




namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
BrushStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  // typedef std::vector<std::string> OptionList;
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "on");
  //
  // Parameter 2: Something else...
  // ...
  ParameterTable t;
  return t;
}




// Available colours - if you change these update the tests :-)
ParameterTable
BrushStyle::newDefaultColorTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList listOfColours;
  listOfColours.push_back("black");
  listOfColours.push_back("white");
  listOfColours.push_back("red");
  listOfColours.push_back("blue");
  listOfColours.push_back("green");
  listOfColours.push_back("cyan");
  listOfColours.push_back("magenta");
  listOfColours.push_back("yellow");
  listOfColours.push_back("orange");
  listOfColours.push_back("brown");
  t.addParameter("color", listOfColours, "black");
  return t;
}




double BrushStyle::newDefaultOpacity(const std::string& nameOfDefaultTable)
{
  double val = 1.0;
  // if (nameOfDefaultTable = "??") val = 0.5 // maybe for the future
  return val;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

BrushStyle::BrushStyle(const std::string& nameOfDefaultTable)
  : m_defaultColorTable(newDefaultColorTable(nameOfDefaultTable)),
    m_defaultOpacity   (newDefaultOpacity(nameOfDefaultTable)),
    parameterHandler   (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    baseStyle          (BaseStyle(nameOfDefaultTable)),
    m_opacity          (defaultOpacity()),
    m_color            (defaultColor()),
    opacityHasBeenSet  (false),
    colorHasBeenSet    (false)
{
  // no-op
}




BrushStyle::~BrushStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void BrushStyle::reset()
{
  m_opacity         = defaultOpacity();
  m_color           = defaultColor();
  opacityHasBeenSet = false;
  colorHasBeenSet   = false;
  parameterHandler.reset();
  baseStyle.reset();
}




// TODO: do we want case-sensitivity?
bool BrushStyle::setParameter(const std::string& par, 
			      const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return baseStyle.setParameter(par, opt);
}




std::string BrushStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return baseStyle.parameterValue(parameterName);
}




std::vector<std::string> BrushStyle::availableParameters() const
{
  // get parameters from BaseStyle and append our own
  std::vector<std::string> vec = baseStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::vector<std::string> BrushStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = baseStyle.availableOptions(param);
  return vec;  
}




std::vector<std::string> BrushStyle::availableColors() const
{
  std::vector<std::string> vec;
  const bool OK = defaultColorTable().availableOptions("color", vec);
  assert (OK);
  return vec;
}




std::string BrushStyle::color() const
{
  return m_color;
}




double BrushStyle::opacity() const
{
  return m_opacity;
}




bool BrushStyle::setColor(const std::string& newColor)
{
  if (!colorIsOK(newColor)) return false;
  else {
    m_color = newColor;
    colorHasBeenSet = true;
    return true;
  }
}




bool BrushStyle::setOpacity(double newOpacity)
{
  if (newOpacity > 1.0 || newOpacity < 0.0) return false;
  else {
    m_opacity = newOpacity;
    opacityHasBeenSet = true;
    return true;
  }
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& BrushStyle::defaultColorTable() const
{
  return m_defaultColorTable;
}




const double BrushStyle::defaultOpacity() const
{
  return m_defaultOpacity;
}




bool BrushStyle::colorIsOK(const std::string& colorName) const
{
  std::vector<std::string> vec;
  bool OK = defaultColorTable().availableOptions("color", vec);
  assert (OK);
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (colorName == vec[i]) return true;
  }
  return false;
}




const std::string BrushStyle::defaultColor() const
{
  std::string result;
  const bool OK = defaultColorTable().getDefaultValue("color", result);
  assert (OK);
  return result;
}




} // end of namespace AidaNative

