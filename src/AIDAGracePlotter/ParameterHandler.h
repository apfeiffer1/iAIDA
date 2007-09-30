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
 * \file Header file for class ParameterHandler
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug 20 15:50:12 2002
// 

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_PARAMETER_HANDLER_H
#define IAIDA_QPLOTTER_NEW_VIEWER_PARAMETER_HANDLER_H



#include "ParameterTable.h"




namespace AidaNative {




/**
 * Helper proxy class for the Style classes. Constructed with a lookup
 * table of type ParameterTable, which holds the parameters and their possible 
 * values. Keeps track of what has been changed and what is default. Supplies
 * current values and lists of values as required. It's basically an implementation
 * detail. I found that I was writing the same code again and again in the XXStyle
 * classes for dealing with parameters, so I put it in here instead.
 */

class ParameterHandler
{
public:
  ParameterHandler(const ParameterTable&);
  ParameterHandler(const ParameterHandler&);
  ParameterHandler& operator=(const ParameterHandler&);
  virtual ~ParameterHandler();

private:
  /// Must construct with a lookup table. No default constructor provided.
  ParameterHandler();

public:
  /// Resets to defaults.
  void reset();
  /// @brief Sets a parameter to a specific value.
  /// @return false if parameter not known
  bool setParameter(const std::string& par, const std::string& opt = "");
  /// @brief Get current value of a named parameter.
  /// @return false if parameter not known.
  bool getParameterValue(const std::string& parameterName,
			 std::string& val) const;
  /// Get list of available parameters.
  std::vector<std::string> availableParameters() const;
  /// @brief Get list of available options for a named parameter.
  /// @return false if parameter unknown.
  bool getAvailableOptions(const std::string& param,
			   std::vector<std::string>& options) const;

private:
  /// The default parameters
  const ParameterTable& defaults() const;
  /// Checks parameter exists.
  bool parameterIsOK(const std::string& parameterName) const;
  /// Sets the parameter without checks.
  void reallySetParameter(const std::string& par, const std::string& options);
      
private:
  typedef std::map<std::string, std::string> StringMap;
  typedef StringMap::const_iterator StringMapCIt;

private:
  /// Table of default parameters (don't use it directly)
  ParameterTable m_defaults;
  /// Parameters which have been explicitly modified.
  StringMap modifiedParameters;
  
};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_PARAMETER_HANDLER_H

