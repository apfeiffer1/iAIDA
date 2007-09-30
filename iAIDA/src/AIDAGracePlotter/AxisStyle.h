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
 * \file Header file for class AxisStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug 20 14:50:02 2002
// 

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_AXISSTYLE_H
#define IAIDA_QPLOTTER_NEW_VIEWER_AXISSTYLE_H





#include "TextStyle.h"
#include "LineStyle.h"



namespace AidaNative {


class ParameterTable;



/**
 * Collection of styles applicable to an axis. Simple container 
 * with a ParameterHandler inside.
 */

class AxisStyle
{
public:
  AxisStyle(const std::string& nameOfDefaultTable = "");
  virtual ~AxisStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string & paramName, 
			    const std::string & options = "");
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string&) const;
  /// The style of the actual axis line
  virtual LineStyle& lineStyle();
  /// The style of the numbers
  virtual TextStyle& tickLabelStyle();
  virtual void setLineStyle(const LineStyle&);
  virtual void setTickLabelStyle(const TextStyle&);  
  virtual std::string label() const;
  virtual void setLabel(const std::string& labelText);
  virtual TextStyle& labelStyle();
  virtual void setLabelStyle(const TextStyle&);

private:
  /// Make new table of default parameters of a given flavour
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");

private:
  ParameterHandler parameterHandler;
  BaseStyle        baseStyle;
  LineStyle        m_axisLineStyle;
  TextStyle        m_valueTextStyle;
  TextStyle        m_labelTextStyle;
  std::string      m_label;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_AXISSTYLE_H

