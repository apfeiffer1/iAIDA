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
 * \file Implementation of class AxisStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug 20 14:50:02 2002
// 



#include "AxisStyle.h"


namespace AidaNative {




///////////////////////////////
//                           //
// Initialisation of statics //
//                           //
///////////////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
AxisStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
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
  OptionList opts;
  //opts.push_back("on");
  //opts.push_back("off");
  //t.addParameter("log", opts, "off");
  //t.addParameter("grid", opts, "off");
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AxisStyle::AxisStyle(const std::string& nameOfDefaultTable)
  : parameterHandler(ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    baseStyle       (BaseStyle(nameOfDefaultTable)),
    m_axisLineStyle (LineStyle(nameOfDefaultTable)),
    m_valueTextStyle(TextStyle("axisValue"))
{
  // no-op
}




AxisStyle::~AxisStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AxisStyle::reset()
{
  parameterHandler.reset();
  baseStyle.reset();
  m_axisLineStyle.reset();
  m_valueTextStyle.reset();
}




bool AxisStyle::setParameter(const std::string & par, 
			     const std::string & opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return baseStyle.setParameter(par, opt);  
}




std::string AxisStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return baseStyle.parameterValue(parameterName);  
}




std::vector<std::string> AxisStyle::availableParameters() const
{
  // get parameters from BaseStyle and append our own
  std::vector<std::string> vec = baseStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::vector<std::string> 
AxisStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = baseStyle.availableOptions(param);
  return vec;    
}




LineStyle& AxisStyle::lineStyle()
{
  return m_axisLineStyle;
}




TextStyle& AxisStyle::tickLabelStyle()
{
  return m_valueTextStyle;
}




void AxisStyle::setTickLabelStyle(const TextStyle& x)
{
  m_valueTextStyle = x;
}



void AxisStyle::setLineStyle(const LineStyle& x)
{
  m_axisLineStyle = x;
}




std::string AxisStyle::label() const
{
  return m_label;
}




void AxisStyle::setLabel(const std::string& labelText)
{
  m_label = labelText;
}



TextStyle& AxisStyle::labelStyle()
{
  return m_labelTextStyle;
}



void AxisStyle::setLabelStyle(const TextStyle& x)
{
  m_labelTextStyle = x;
}



/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////




} // end of namespace AidaNative

