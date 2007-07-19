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
 * \file Header file for class LegendBoxStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Thu Sep 26 21:12:50 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_LEGENDBOXSTYLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_LEGENDBOXSTYLE_H


#include "TextStyle.h"
#include "FillStyle.h"
#include "LineStyle.h"




namespace AidaNative {



/**
 * Style applicable to legend boxes
 */

class LegendBoxStyle
{
public:
  LegendBoxStyle(const std::string& nameOfDefaultTable = "");
  virtual ~LegendBoxStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string& parameterName, 
			    const std::string& options = "");
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& param) const;
  /// Style of the border around the box
  virtual LineStyle& borderStyle();
  /// Style of the background in the box
  virtual FillStyle& fillStyle();
  /// Style of all the text in the box
  virtual TextStyle& textStyle();
  virtual void setBorderStyle(const LineStyle&);
  virtual void setFillStyle(const FillStyle&);
  virtual void setTextStyle(const TextStyle&);

private:
  ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");

private:
  ParameterHandler parameterHandler;
  LineStyle        m_borderStyle;
  FillStyle        m_fillStyle;
  TextStyle        m_textStyle;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_LEGENDBOXSTYLE_H

