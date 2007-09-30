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
 * \file Implementation of class ParameterHandler
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug 20 15:50:12 2002
// 



#include <assert.h>


#include "ParameterHandler.h"




namespace AidaNative {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

ParameterHandler::ParameterHandler(const ParameterTable& table)
  : m_defaults(table)
{
  // no-op
}




ParameterHandler::~ParameterHandler()
{
  modifiedParameters.clear();
}




ParameterHandler::ParameterHandler(const ParameterHandler& rhs)
  : m_defaults        (rhs.m_defaults),
    modifiedParameters(rhs.modifiedParameters)
{
  // no-op
}




ParameterHandler& ParameterHandler::operator=(const ParameterHandler& rhs)
{
  if (this != &rhs) {
    m_defaults         = rhs.m_defaults;
    modifiedParameters = rhs.modifiedParameters; 
  }
  return *this;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void ParameterHandler::reset()
{
  modifiedParameters.clear();
}




bool ParameterHandler::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterIsOK(par);
  if (!OK) return false;
  else {
    // OK, the parameter is known. Are the options reasonable?
    std::vector<std::string> availableOptions;
    const bool stillOK = defaults().availableOptions(par, availableOptions);
    assert (stillOK);
    if (availableOptions.empty()) {
      // either the option can have any value or it will be ignored.
      reallySetParameter(par,opt);
      return true;
    }
    else {
      // restricted set of options - check this one is a valid choice.
      for (unsigned int i = 0; i < availableOptions.size(); ++i) {
	if (opt == availableOptions[i]) {
	  reallySetParameter(par,opt);
	  return true;
	}
      }
      return false;
    }
  }
}




bool ParameterHandler::getParameterValue(const std::string& parameterName,
					 std::string& parameterValue) const
{
  if (parameterIsOK(parameterName)) {
    // Do we have a non-default value set? If so, return it.
    for (StringMapCIt i = modifiedParameters.begin(); 
	 i != modifiedParameters.end(); ++i) {
      if (i->first == parameterName) {
	parameterValue = i->second;
	return true;
      }
    }
    // No local value was set, so just return the default.
    std::string s;
    const bool OK = defaults().getDefaultValue(parameterName, s);
    assert (OK);
    if (OK) {
      parameterValue = s;
      return true;
    }
    else return false; // something went wrong! parameterIsOK should have returned false!
  }
  else return false;
}




std::vector<std::string> ParameterHandler::availableParameters() const
{
  return defaults().availableParameters();
}




bool ParameterHandler::getAvailableOptions(const std::string& param,
					   std::vector<std::string>& options) const
{
  const bool OK = parameterIsOK(param);
  if (!OK) return false;
  else {
    const bool stillOK = defaults().availableOptions(param, options);
    assert (stillOK);
    return true;
  }
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& ParameterHandler::defaults() const
{
  return m_defaults;
}




bool ParameterHandler::parameterIsOK(const std::string& parameterName) const
{
  std::vector<std::string> vec = defaults().availableParameters();
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (parameterName == vec[i]) return true;
  }
  return false;
}




void ParameterHandler::reallySetParameter(const std::string& par, 
					  const std::string& options)
{
  modifiedParameters[par] = options;
}




} // end of namespace AidaNative

