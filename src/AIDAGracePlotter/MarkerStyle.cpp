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
 * \file Implementation of class MarkerStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:34:15 2002
// 


#include <assert.h>




#include "MarkerStyle.h"



namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
MarkerStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
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
  OptionList optList;
  //optList.push_back("line");
  //optList.push_back("tee");
  //optList.push_back("rectangle");
  //optList.push_back("ellipse");
  //t.addParameter("errorStyle", optList, "line");
  return t;
}




// Available marker types - if you change these update the tests :-)
ParameterTable
MarkerStyle::newDefaultShapeTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList shapeList;
  shapeList.push_back("solidCircle");
  shapeList.push_back("openCircle");
  shapeList.push_back("solidSquare");
  shapeList.push_back("openSquare");
  shapeList.push_back("solidDiamond");
  shapeList.push_back("openDiamond");
  t.addParameter("shape", shapeList, "openCircle");
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

MarkerStyle::MarkerStyle(const std::string& nameOfDefaultTable)
  : m_defaultShapeTable(newDefaultShapeTable(nameOfDefaultTable)),
    parameterHandler   (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    brushStyle         (BrushStyle(nameOfDefaultTable)),
    m_shape            (defaultShape()),
    shapeHasBeenSet    (false)
{
  // no-op
}




MarkerStyle::~MarkerStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void MarkerStyle::reset()
{
  m_shape         = defaultShape();
  shapeHasBeenSet = false;
  parameterHandler.reset();
  brushStyle.reset();
}




bool MarkerStyle::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return brushStyle.setParameter(par, opt);
}




std::string MarkerStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return brushStyle.parameterValue(parameterName);
}




std::vector<std::string> MarkerStyle::availableParameters() const
{
  // get parameters from BrushStyle and append our own
  std::vector<std::string> vec = brushStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::vector<std::string> MarkerStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = brushStyle.availableOptions(param);
  return vec; 
}




std::vector<std::string> MarkerStyle::availableColors() const
{
  return brushStyle.availableColors();
}




std::string MarkerStyle::color() const
{
  return brushStyle.color();
}




double MarkerStyle::opacity() const
{
  return brushStyle.opacity();
}




bool MarkerStyle::setColor(const std::string& s)
{
  return brushStyle.setColor(s);
}




bool MarkerStyle::setOpacity(double d)
{
  return brushStyle.setOpacity(d);
}




std::vector<std::string> MarkerStyle::availableShapes() const
{
  std::vector<std::string> vec;
  const bool OK = defaultShapeTable().availableOptions("shape", vec);
  assert (OK);
  return vec;
}




std::string MarkerStyle::shape() const
{
  return m_shape;
}




bool MarkerStyle::setShape(const std::string& newShape)
{
  if (!shapeIsOK(newShape)) return false;
  else {
    m_shape = newShape;
    shapeHasBeenSet = true;
    return true;
  }  
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& MarkerStyle::defaultShapeTable() const
{
  return m_defaultShapeTable;
}




bool MarkerStyle::shapeIsOK(const std::string& shapeName) const
{
  std::vector<std::string> vec;
  const bool OK = defaultShapeTable().availableOptions("shape", vec);
  assert (OK);
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (shapeName == vec[i]) return true;
  }
  return false;
}




std::string MarkerStyle::defaultShape() const
{
  std::string result;
  const bool OK = defaultShapeTable().getDefaultValue("shape", result);
  assert (OK);
  return result;
}




} // end of namespace AidaNative

