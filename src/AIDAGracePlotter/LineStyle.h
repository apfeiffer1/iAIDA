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
 * \file Header file for class LineStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:19:47 2002
// 

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_LINESTYLE_H
#define IAIDA_QPLOTTER_NEW_VIEWER_LINESTYLE_H



#include "BrushStyle.h"



namespace AidaNative {



/**
 * Style for all lines (axis lines, legend box outlines, etc.)
 */

class LineStyle
{
public:
  LineStyle(const std::string& nameOfDefaultTable = "");
  virtual ~LineStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string&, const std::string& options = "");
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& param) const;
  /// Get a list of the colours supported by this implementation.
  virtual std::vector<std::string> availableColors() const;
  /// Current colour of this line style.
  virtual std::string color() const;
  /// Current opacity (alpha) of this line style.
  virtual double opacity() const;
  /// Set current colour, e.g. "darkGreen", "black"
  virtual bool setColor(const std::string&);
  /// Set opacity (alpha), e.g. 0 (transparent), 1 (opaque), 0.5 (halfway).
  virtual bool setOpacity(double);
  /// Get list of available line types
  virtual std::vector<std::string> availableLineTypes() const;
  /// Get current line type
  virtual std::string lineType() const;
  /// Get current line thickness
  virtual int thickness() const;
  /// Set line type (e.g. "dotted")
  virtual bool setLineType(const std::string&);
  /// Set line thickness (e.g. 3)
  virtual bool setThickness(int);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" table of line types according to string argument
  static ParameterTable newDefaultLineTypeTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" line thickness according to input string
  static int newDefaultThickness(const std::string& nameOfDefaultTable = "");
  /// Maximum permitted line thickness
  static const int MAX_THICKNESS;

private:
  /// Available line types
  const ParameterTable& defaultLineTypeTable() const;
  /// Line type name string understood?
  bool lineTypeIsOK(const std::string& lineTypeName) const;
  /// Get the default line type
  const std::string defaultLineType() const;
  /// Get the default line thickness
  const int defaultThickness() const;

private:
  ParameterTable   m_defaultLineTypeTable;
  int              m_defaultThickness;
  ParameterHandler parameterHandler;
  BrushStyle       brushStyle;
  int              m_thickness;
  std::string      m_lineType;
  bool             thicknessHasBeenSet;
  bool             lineTypeHasBeenSet;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_LINESTYLE_H

