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
 * \file Implementation of class Layout
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Sep 25 11:56:41 2002
// 

#ifdef __sun
# include <stdlib.h>
#endif



#include "Layout.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif



namespace AidaNative {




///////////////////////////////
//                           //
// Initialisation of statics //
//                           //
///////////////////////////////

//const double Layout::ERROR_VALUE = -999.0;



// Available parameters - if you change these update the tests :-)
ParameterTable
Layout::newDefaultPTable(const std::string& nameOfDefaultTable)
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
  //t.addParameter("globalTitleTopMargin",  opts, "10.0");
  //t.addParameter("xAxisToLabel",          opts,  "5.0");
  // ... lots more can be added here
  return t;
}




// const TextMessage 
// Layout::unknownParameter("parameter $1 is not known",
// 			 "Layout", TextMessage::WARNING);



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

Layout::Layout(const std::string& nameOfDefaultTable)
  : parameterHandler(ParameterHandler(newDefaultPTable(nameOfDefaultTable)))
{
  // no-op
}




Layout::~Layout()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void Layout::reset()
{
  parameterHandler.reset();
}




double Layout::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (!OK || s == "") {
    const TextMessage unknownParameter("parameter $1 is not known",
			 "Layout", TextMessage::WARNING);
    unknownParameter.show(parameterName);
    return ERROR_VALUE;
  }
  else return stringAsDouble(s);
}




bool Layout::setParameterValue(const std::string& parameterName, double newValue)
{
  std::string s = doubleAsString(newValue);
  bool OK = parameterHandler.setParameter(parameterName, s);
  return OK;
}




std::vector<std::string> Layout::availableParameters() const
{
  return std::vector<std::string>(parameterHandler.availableParameters());
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

std::string Layout::doubleAsString(double d)
{
  std::ostringstream ostr;
  ostr << d;
#ifndef BADENDS
  ostr << std::ends;
#endif
  return ostr.str();
}




double Layout::stringAsDouble(const std::string& s)
{
  if (s == "") return ERROR_VALUE;
  else {
    std::string ncs = s;
    float f = atof(const_cast<char*>(ncs.c_str()));
    return double(f);
  }
}




} // end of namespace AidaNative

