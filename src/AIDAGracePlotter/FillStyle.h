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
 * \file 
 *
 * Header file for class FillStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:29:21 2002
// 


#ifndef IAIDA_QPLOTTER_NEW_VIEWER_FILLSTYLE_H
#define IAIDA_QPLOTTER_NEW_VIEWER_FILLSTYLE_H




#include "BrushStyle.h"



namespace AidaNative {



/**
 * Style for all filled areas
 */

class FillStyle
{
public:
  FillStyle(const std::string& nameOfDefaultTable = "");
  virtual ~FillStyle();

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
  /// Current colour of this brush style.
  virtual std::string color() const;
  /// Current opacity (alpha) of this brush style.
  virtual double opacity() const;
  /// Set current colour, e.g. "darkGreen"
  virtual bool setColor(const std::string&);
  /// Set opacity (alpha), e.g. 0 (transparent), 1 (opaque), 0.5 (halfway).
  virtual bool setOpacity(double);
  /// Get available fill pattern types
  virtual std::vector<std::string> availablePatterns() const;
  /// Get current fill pattern type
  virtual std::string pattern() const;
  /// Set fill pattern type (e.g. "verticalLines")
  virtual bool setPattern(const std::string&);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" table of patterns according to string argument
  static ParameterTable newDefaultPatternTable(const std::string& nameOfDefaultTable = "");

private:
  /// Available patterns
  const ParameterTable& defaultPatternTable() const;
  /// Pattern name understood?
  bool patternIsOK(const std::string& patternName) const;
  /// Get default pattern name
  std::string defaultPattern() const;

private:
  ParameterTable   m_defaultPatternTable;
  ParameterHandler parameterHandler;
  BrushStyle       brushStyle;
  std::string      m_pattern;
  bool             patternHasBeenSet;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_FILLSTYLE_H

