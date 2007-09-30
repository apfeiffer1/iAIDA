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
 * \file Header file for class Layout
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Sep 25 11:56:41 2002
// 

#ifndef IAIDA_QPLOTTER_NEWVIEWER_LAYOUT_H
#define IAIDA_QPLOTTER_NEWVIEWER_LAYOUT_H


#include "ParameterHandler.h"
#include "TextMessage.h"




namespace AidaNative {



/**
 * Simple container class holding various little numerical parameters 
 * for fine tuning the layout of plots (margins, length of ticks etc.)
 */

class Layout
{
public:
  Layout(const std::string& nameOfDefaultTable = "");
  virtual ~Layout();

private:
  static const int ERROR_VALUE = -999;

public:
  /// Reset to default (construction time) values
  virtual void reset();
  /// Returns ERROR_VALUE if parameter name is not known
  virtual double parameterValue(const std::string& parameterName) const;
  /// Returns false if parameter name not known
  virtual bool setParameterValue(const std::string& parameterName, double newValue);
  virtual std::vector<std::string> availableParameters() const;

private:
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  static std::string doubleAsString(double);
  static double stringAsDouble(const std::string&);

private:
  static const TextMessage unknownParameter;

private:
  /// @brief Use a ParameterHandler for now. It stores parameter values as 
  /// strings, not doubles, but we can just convert them on the fly. 
  /// When/if we have time we can change it to something less stupid.
  ParameterHandler parameterHandler;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEWVIEWER_LAYOUT_H

