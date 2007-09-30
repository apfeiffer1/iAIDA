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
 * \file Header file for class TitleStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 22:45:52 2002
// 

#ifndef IAIDA_TITLESTYLE_H
#define IAIDA_TITLESTYLE_H


#include "TextStyle.h"




namespace AidaNative {



/**
 * Style for the title of a region or the plotter as a whole
 */

class TitleStyle
{
public:
  TitleStyle(const std::string& nameOfDefaultTable = "");
  virtual ~TitleStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& paramName) const;
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// get the style of the title text
  virtual TextStyle& textStyle();
  /// set the style of the title text
  virtual bool setTextStyle(const TextStyle& textStyle);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  
private:
  ParameterHandler parameterHandler;
  TextStyle m_textStyle;

};



} // end of namespace AidaNative


#endif // IAIDA_TITLESTYLE_H

