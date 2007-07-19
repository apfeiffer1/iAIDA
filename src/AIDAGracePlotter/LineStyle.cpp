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
 * \file Implementation of class LineStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:19:47 2002
// 


#include <assert.h>


#include "LineStyle.h"



namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
LineStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  //
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions);
  //
  // Parameter 2: Something else...
  // ...
  ParameterTable t;
  return t;
}




// Available line types - if you change these update the tests :-)
ParameterTable
LineStyle::newDefaultLineTypeTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList lineTypes;
  lineTypes.push_back("solid");
  lineTypes.push_back("dashed");
  lineTypes.push_back("dotted");
  lineTypes.push_back("dashdot");
  lineTypes.push_back("dashdotdot");
  t.addParameter("type", lineTypes, "solid");
  // modify defaults if required
  if (nameOfDefaultTable == "axisGrid") {
    t.setDefaultValue("type", "dotted");
  }
  return t;
}




int LineStyle::newDefaultThickness(const std::string& nameOfDefaultTable)
{
  int val = 1;
  if (nameOfDefaultTable == "axisValue") val = 0; // for the future
  return val;
}




const int LineStyle::MAX_THICKNESS = 9;




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

LineStyle::LineStyle(const std::string& nameOfDefaultTable)
  : m_defaultLineTypeTable(newDefaultLineTypeTable(nameOfDefaultTable)),
    m_defaultThickness    (newDefaultThickness(nameOfDefaultTable)),
    parameterHandler      (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    brushStyle            (BrushStyle(nameOfDefaultTable)),
    m_thickness           (defaultThickness()),
    m_lineType            (defaultLineType()),
    thicknessHasBeenSet   (false),
    lineTypeHasBeenSet    (false)
{
  // no-op
}




LineStyle::~LineStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void LineStyle::reset()
{
  m_thickness         = defaultThickness();
  m_lineType          = defaultLineType();
  thicknessHasBeenSet = false;
  lineTypeHasBeenSet  = false;
  parameterHandler.reset();
  brushStyle.reset();
}




bool LineStyle::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return brushStyle.setParameter(par, opt);
}




std::vector<std::string> LineStyle::availableParameters() const
{
  // get parameters from BrushStyle and append our own
  std::vector<std::string> vec = brushStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::string LineStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return brushStyle.parameterValue(parameterName);
}




std::vector<std::string> LineStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = brushStyle.availableOptions(param);
  return vec; 
}




std::vector<std::string> LineStyle::availableColors() const
{
  return brushStyle.availableColors();
}




std::string LineStyle::color() const
{
  return brushStyle.color();
}




double LineStyle::opacity() const
{
  return brushStyle.opacity();
}




bool LineStyle::setColor(const std::string& s)
{
  return brushStyle.setColor(s);
}




bool LineStyle::setOpacity(double d)
{
  return brushStyle.setOpacity(d);
}




std::vector<std::string> LineStyle::availableLineTypes() const
{
  std::vector<std::string> vec;
  const bool OK = defaultLineTypeTable().availableOptions("type", vec);
  assert (OK);
  return vec;
}




std::string LineStyle::lineType() const
{
  return m_lineType;
}




int LineStyle::thickness() const
{
  return m_thickness;
}




bool LineStyle::setLineType(const std::string& newLineType)
{
  if (!lineTypeIsOK(newLineType)) return false;
  else {
    m_lineType = newLineType;
    lineTypeHasBeenSet = true;
    return true;
  }
}




bool LineStyle::setThickness(int newThickness)
{
  if (newThickness < 0 || newThickness > MAX_THICKNESS) return false;
  else {
    m_thickness = newThickness;
    thicknessHasBeenSet = true;
    return true;
  }
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& LineStyle::defaultLineTypeTable() const
{
  return m_defaultLineTypeTable;
}




const int LineStyle::defaultThickness() const
{
  return m_defaultThickness;
}




bool LineStyle::lineTypeIsOK(const std::string& lineTypeName) const
{
  std::vector<std::string> vec;
  const bool OK = defaultLineTypeTable().availableOptions("type", vec);
  assert (OK);
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (lineTypeName == vec[i]) return true;
  }
  return false;
}




const std::string LineStyle::defaultLineType() const
{
  std::string result;
  const bool OK = defaultLineTypeTable().getDefaultValue("type", result);
  assert (OK);
  return result;  
}




} // end of namespace AidaNative

