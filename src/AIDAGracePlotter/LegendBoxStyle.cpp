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
 * \file Implementation of class LegendBoxStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Thu Sep 26 21:12:50 2002
// 



#include "LegendBoxStyle.h"



namespace AidaNative {



////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
LegendBoxStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  //
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "off");
  //
  // Parameter 2: Something else...
  // ...
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList op;
  //op.push_back("left");
  //op.push_back("right");
  //t.addParameter("textPosition", op, "right");
  return t;
}




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

LegendBoxStyle::LegendBoxStyle(const std::string& nameOfDefaultTable)
  : parameterHandler(ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    m_borderStyle(LineStyle("legendBoxBorder")),
    m_fillStyle(FillStyle("legendBox")),
    m_textStyle(TextStyle(nameOfDefaultTable))
		
{
  // no-op
}




LegendBoxStyle::~LegendBoxStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void LegendBoxStyle::reset()
{
  parameterHandler.reset();
  m_borderStyle.reset();
  m_fillStyle.reset();
  m_textStyle.reset();
}




bool LegendBoxStyle::setParameter(const std::string& par, 
				  const std::string& opt)
{
  return parameterHandler.setParameter(par, opt);
}




std::string LegendBoxStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return "";
}




std::vector<std::string> LegendBoxStyle::availableParameters() const
{
  return parameterHandler.availableParameters();
}




std::vector<std::string> 
LegendBoxStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (OK) return vec;
  else return std::vector<std::string>();
}




LineStyle& LegendBoxStyle::borderStyle()
{
  return m_borderStyle;
}




FillStyle& LegendBoxStyle::fillStyle()
{
  return m_fillStyle;
}




TextStyle& LegendBoxStyle::textStyle()
{
  return m_textStyle;
}




void LegendBoxStyle::setBorderStyle(const LineStyle& x)
{
  m_borderStyle = x;
}




void LegendBoxStyle::setFillStyle(const FillStyle& x)
{
  m_fillStyle = x;
}




void LegendBoxStyle::setTextStyle(const TextStyle& x)
{
  m_textStyle = x;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

