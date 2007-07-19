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
 * \file Header file for class BrushStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:09:57 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_BRUSHSTYLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_BRUSHSTYLE_H




#include "BaseStyle.h"




namespace AidaNative {



/**
 * Superclass for anything which can be different colours & opacity
 */

class BrushStyle
{
public:
  BrushStyle(const std::string& nameOfDefaultTable = "");
  virtual ~BrushStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string&, const std::string& options = "");
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& param) const;
  /// Get a list of the colours supported
  virtual std::vector<std::string> availableColors() const;
  /// Current colour of this brush style.
  virtual std::string color() const;
  /// Current opacity (alpha) of this brush style.
  virtual double opacity() const;
  /// Set current colour, e.g. "darkGreen"
  virtual bool setColor(const std::string&);
  /// Set opacity (alpha), e.g. 0 (transparent), 1 (opaque), 0.5 (halfway).
  virtual bool setOpacity(double);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" table of colours according to string argument
  static ParameterTable newDefaultColorTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" opacity according to string argument
  static double newDefaultOpacity(const std::string& nameOfDefaultTable = "");

private:
  /// Available colours
  const ParameterTable& defaultColorTable() const;
  /// Colour name string is understood?
  bool colorIsOK(const std::string& colorName) const;
  /// Get default colour
  const std::string defaultColor() const;
  /// Get default opacity
  const double defaultOpacity() const;

private:
  ParameterTable   m_defaultColorTable;
  double           m_defaultOpacity;
  ParameterHandler parameterHandler;
  BaseStyle        baseStyle;
  double           m_opacity;
  std::string      m_color;
  bool             opacityHasBeenSet;
  bool             colorHasBeenSet;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_BRUSHSTYLE_H

