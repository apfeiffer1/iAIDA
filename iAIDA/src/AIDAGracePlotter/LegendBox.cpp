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

//
// (c) CERN, Geneva
//

//
// Implementation file for class LegendBox
// 
// Created by user sang on Mon Aug  5 12:23:42 2002
// 



#include "LegendBox.h"



namespace AidaNative {


////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
LegendBox::newDefaultPTable(const std::string& nameOfDefaultTable)
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
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

LegendBox::LegendBox(const std::string& nameOfDefaultTable)
  : parameterHandler   (ParameterHandler(newDefaultPTable(nameOfDefaultTable)))
{
  // no-op
}




LegendBox::~LegendBox()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void LegendBox::resetParameters()
{
  parameterHandler.reset();
}




bool LegendBox::setParameter(const std::string& paramName, 
			     const std::string& options)
{
  return parameterHandler.setParameter(paramName, options);
}




std::string LegendBox::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return "";
}




std::vector<std::string> 
LegendBox::availableParameters() const
{
  return parameterHandler.availableParameters();
}




std::vector<std::string> 
LegendBox::availableOptions(const std::string& paramName) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(paramName, vec);
  if (OK) return vec;
  else return std::vector<std::string>();
}




bool LegendBox::addText(const std::string& s)
{
  textList.push_back(s);
  return true;
}



void LegendBox::clear()
{
  lineList.clear();
  markerList.clear();
  fillList.clear();
  textList.clear();
}



bool LegendBox::addLegend(const std::string& legendText, 
			  const MarkerStyle& markerStyle) 
{
  markerList.push_back(std::make_pair(markerStyle, legendText));
  return true;
}




bool LegendBox::addLegend(const std::string& legendText,
			  const LineStyle& lineStyle) 
{
  lineList.push_back(std::make_pair(lineStyle, legendText));
  return true;
}




bool LegendBox::addLegend(const std::string& legendText, 
			  const FillStyle& fillStyle) 
{
  fillList.push_back(std::make_pair(fillStyle, legendText));
  return true;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

