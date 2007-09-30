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
 * \file Implementation of class PlotStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Sep 25 14:05:13 2002
// 



#include "PlotStyle.h"



namespace AidaNative {



/////////////
//         //
// Statics //
//         //
/////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
PlotStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  // typedef std::vector<std::string> OptionList;
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "on");
  //
  // Parameter 2: Something else...
  // ..of course behaviour can be modified according to nameOfDefT...
  ParameterTable t;
  return t;
}



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

PlotStyle::PlotStyle(const std::string& defaultName)
  : parameterHandler(ParameterHandler(newDefaultPTable(defaultName))),
    m_markerStyle   (MarkerStyle(defaultName)),
    m_lineStyle     (LineStyle(defaultName)),
    m_fillStyle     (FillStyle(defaultName))
{
  // no-op
}




PlotStyle::~PlotStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void PlotStyle::reset() 
{
  parameterHandler.reset();
}




bool PlotStyle::setParameter(const std::string& par, const std::string& opt) 
{
  return parameterHandler.setParameter(par, opt);
}




std::string PlotStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (!OK) s = "";
  return s;
}




std::vector<std::string> PlotStyle::availableParameters() const 
{
  return parameterHandler.availableParameters();
}




std::vector<std::string> 
PlotStyle::availableOptions(const std::string& param) const 
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec.clear();
  return vec;
}




MarkerStyle& PlotStyle::markerStyle()
{
  return m_markerStyle;
}




LineStyle& PlotStyle::lineStyle()
{
  return m_lineStyle;
}




FillStyle& PlotStyle::fillStyle()
{
  return m_fillStyle;
}




void PlotStyle::setMarkerStyle(const MarkerStyle& newMarkerStyle)
{
  m_markerStyle = newMarkerStyle;
}




void PlotStyle::setLineStyle(const LineStyle& newLineStyle)
{
  m_lineStyle = newLineStyle;
}




void PlotStyle::setFillStyle(const FillStyle& newFillStyle)
{
  m_fillStyle = newFillStyle;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

