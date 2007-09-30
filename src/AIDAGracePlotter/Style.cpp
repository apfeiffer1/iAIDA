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
 * \file Implementation of class Style
 *
 * (c) Copyright CERN, Geneva
 *
 */


//
// Created by user sang on Tue Sep 24 14:36:07 2002
//



#include "Style.h"




namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available representations - if you change these update the tests :-)

ParameterTable
Style::newDefaultPTable(const std::string& nameOfDefaultTable)
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
  return t;
}





////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////


Style::Style(const std::string& nameOfDefaultTable) 
  : parameterHandler     (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    m_plotStyle          (PlotStyle(nameOfDefaultTable)),
    m_xAxisStyle         (AxisStyle(nameOfDefaultTable)),
    m_yAxisStyle         (AxisStyle(nameOfDefaultTable)),
    m_zAxisStyle         (AxisStyle(nameOfDefaultTable))
{
  // no-op
}



Style::~Style() 
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void Style::reset()
{
  parameterHandler.reset();
  m_plotStyle.reset();
  m_xAxisStyle.reset();
  m_yAxisStyle.reset();
  m_zAxisStyle.reset();
}




bool Style::setParameter(const std::string& par, const std::string& opt)
{
  return parameterHandler.setParameter(par, opt);
}




std::vector<std::string> Style::availableParameters() const
{
  return parameterHandler.availableParameters();
}



std::string Style::parameterValue(const std::string& parameterName) const
{
  std::string s;
  parameterHandler.getParameterValue(parameterName, s);
  return s;
}



std::vector<std::string> 
Style::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  parameterHandler.getAvailableOptions(param, vec);
  return std::vector<std::string>(vec); 
}




PlotStyle& Style::plotStyle()
{
  return m_plotStyle;
}




void Style::setPlotStyle(const PlotStyle& x)
{
  m_plotStyle = x;
}




void Style::setXAxisStyle(const AxisStyle& x)
{
  m_xAxisStyle = x;
}




void Style::setYAxisStyle(const AxisStyle& x)
{
  m_yAxisStyle = x;
}




void Style::setZAxisStyle(const AxisStyle& x)
{
  m_zAxisStyle = x;
}




void Style::setTitleStyle(const TitleStyle& x)
{
  m_titleStyle = x;
}




void Style::setLegendBoxStyle(const LegendBoxStyle& x)
{
  m_legendBoxStyle = x;
}




AxisStyle& Style::xAxisStyle()
{
  return m_xAxisStyle;
}




AxisStyle& Style::yAxisStyle()
{
  return m_yAxisStyle;
}




AxisStyle& Style::zAxisStyle()
{
  return m_zAxisStyle;
}



TitleStyle& Style::titleStyle()
{
  return m_titleStyle;
}




LegendBoxStyle& Style::legendBoxStyle()
{
  return m_legendBoxStyle;
}




} // end of namespace AidaNative
