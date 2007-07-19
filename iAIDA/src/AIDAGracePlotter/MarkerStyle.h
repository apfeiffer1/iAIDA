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
 * \file Header file for class MarkerStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:34:15 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_MARKERSTYLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_MARKERSTYLE_H



#include "BrushStyle.h"



namespace AidaNative {




/**
 * Style for all markers (histo representation, legend box etc)
 */

class MarkerStyle
{
public:
  MarkerStyle(const std::string& nameOfDefaultTable = "");
  virtual ~MarkerStyle();

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
  /// Get a list of the colours supported
  virtual std::vector<std::string> availableColors() const;
  /// Current colour of the marker
  virtual std::string color() const;
  /// Current opacity (alpha)
  virtual double opacity() const;
  /// Set current marker colour, e.g. "darkGreen"
  virtual bool setColor(const std::string&);
  /// Set opacity (alpha), e.g. 0 (transparent), 1 (opaque), 0.5 (halfway).
  virtual bool setOpacity(double);
  /// List available shapes
  virtual std::vector<std::string> availableShapes() const;
  /// Get current shape
  virtual std::string shape() const;
  /// Set marker shape (e.g. "filledCircle")
  virtual bool setShape(const std::string&);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" table of shapes according to string argument
  static ParameterTable newDefaultShapeTable(const std::string& nameOfDefaultTable = "");

private:
  /// Available shapes
  const ParameterTable& defaultShapeTable() const;
  /// Shape name understood?
  bool shapeIsOK(const std::string& shapeName) const;
  /// Get default shape
  std::string defaultShape() const;

private:
  ParameterTable   m_defaultShapeTable;
  ParameterHandler parameterHandler;
  BrushStyle       brushStyle;
  std::string      m_shape;
  bool             shapeHasBeenSet;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_MARKERSTYLE_H

