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
 * \file Header file for class BaseStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 15:54:34 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_BASE_STYLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_BASE_STYLE_H



#include "ParameterHandler.h"




namespace AidaNative {



/**
 * Base for all style classes
 */

class BaseStyle
{
public:
  BaseStyle(const std::string& nameOfDefaultTable = "");
  virtual ~BaseStyle();

public:
  /// @brief Return to original (construction time) state. Explicitly 
  /// set parameters will be set to defaults and may be overridden 
  virtual void reset();
  /**
   * @brief Set a parameter
   *
   * Set a parameter. 
   * @param paramName Name of the parameter.
   * @param options String of options.
   * @return false if parameter (or its options) unknown or invalid.
   */
  virtual bool setParameter(const std::string& parameterName, 
			    const std::string& options = "");
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& param) const;

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");

private:
  ParameterHandler parameterHandler;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_BASE_STYLE_H

