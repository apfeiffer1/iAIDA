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
 * \file Implementation of class FillStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:29:21 2002
// 


#include <assert.h>


#include "FillStyle.h"



namespace AidaNative {



////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters
ParameterTable
FillStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  // (Example) Parameter1: 
  //
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "off");
  //
  // Parameter 2: Something else...
  // ...
  OptionList anyValue;
  //t.addParameter("angle1",  anyValue,  "45");
  //t.addParameter("angle2",  anyValue, "315");
  //t.addParameter("angle3",  anyValue,  "30");
  OptionList meshSizes;
  //meshSizes.push_back("fine");
  //meshSizes.push_back("medium");
  //meshSizes.push_back("coarse");
  //t.addParameter("meshSize", meshSizes, "medium");
  return t;
}




// Available fill patterns. If you change these update the tests :-)
ParameterTable
FillStyle::newDefaultPatternTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList availablePatterns;
  availablePatterns.push_back("empty");
  availablePatterns.push_back("solid");
  availablePatterns.push_back("crossHatch");
  availablePatterns.push_back("horizontalLines");
  availablePatterns.push_back("verticalLines");
  t.addParameter("pattern", availablePatterns, "empty");
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

FillStyle::FillStyle(const std::string& nameOfDefaultTable)
  : m_defaultPatternTable(newDefaultPatternTable(nameOfDefaultTable)),
    parameterHandler     (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    brushStyle           (BrushStyle(nameOfDefaultTable)), 
    m_pattern            (defaultPattern()),
    patternHasBeenSet    (false)
{
  // no-op
}




FillStyle::~FillStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void FillStyle::reset()
{
  m_pattern         = defaultPattern();
  patternHasBeenSet = false;
  parameterHandler.reset();
  brushStyle.reset();
}




bool FillStyle::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return brushStyle.setParameter(par, opt);
}




std::vector<std::string> FillStyle::availableParameters() const
{
  // get parameters from BrushStyle and append our own
  std::vector<std::string> vec = brushStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::string FillStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return brushStyle.parameterValue(parameterName);
}




std::vector<std::string> FillStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = brushStyle.availableOptions(param);
  return vec;
}




std::vector<std::string> FillStyle::availableColors() const
{
  return brushStyle.availableColors();
}




std::string FillStyle::color() const
{
  return brushStyle.color();
}




double FillStyle::opacity() const
{
  return brushStyle.opacity();
}




bool FillStyle::setColor(const std::string& s)
{
  return brushStyle.setColor(s);
}




bool FillStyle::setOpacity(double d)
{
  return brushStyle.setOpacity(d);
}




std::vector<std::string> FillStyle::availablePatterns() const
{
  std::vector<std::string> vec;
  const bool OK = defaultPatternTable().availableOptions("pattern", vec);
  assert (OK);
  return vec;
}




std::string FillStyle::pattern() const
{
  return m_pattern;
}




bool FillStyle::setPattern(const std::string& newPattern)
{
  if (!patternIsOK(newPattern)) return false;
  else {
    m_pattern = newPattern;
    patternHasBeenSet = true;
    return true;
  }  
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& FillStyle::defaultPatternTable() const
{
  return m_defaultPatternTable;
}




bool FillStyle::patternIsOK(const std::string& patternName) const
{
  std::vector<std::string> vec;
  const bool OK = defaultPatternTable().availableOptions("pattern", vec);
  assert (OK);
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (patternName == vec[i]) return true;
  }
  return false;
}




std::string FillStyle::defaultPattern() const
{
  std::string result;
  const bool OK = defaultPatternTable().getDefaultValue("pattern", result);
  assert (OK);
  return result;
}




} // end of namespace AidaNative

