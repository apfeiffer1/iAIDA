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
 * \file Header file for class PlotStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Sep 25 14:05:13 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTSTYLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTSTYLE_H



#include "MarkerStyle.h"
#include "LineStyle.h"
#include "FillStyle.h"
#include "TextMessage.h"



namespace AidaNative {



class ParameterTable;



/**
 * 
 * Class for manipulating the style of the representation of 
 * the histogram, function, etc.
 * to be drawn. Parameters could include:
 *   - "contourPalette" "black:0., darkBlue:1., blue:9., red:9.5"
 *   - etc...
 */

class PlotStyle
{
public:
  PlotStyle(const std::string& nameOfDefaultTable = "");
  virtual ~PlotStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string & paramName, 
			    const std::string & options);
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string&) const;
  /// Get current marker style in use
  virtual MarkerStyle& markerStyle();
  /// Get current line style in use
  virtual LineStyle& lineStyle();
  /// Get current fill style in use
  virtual FillStyle& fillStyle();
  /// Set style of markers
  virtual void setMarkerStyle(const MarkerStyle&);
  /// Set style of lines 
  virtual void setLineStyle(const LineStyle&);
  /// Set fill style of histo, box plot etc.
  virtual void setFillStyle(const FillStyle&);

private:
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");

private:
  ParameterHandler parameterHandler;
  MarkerStyle      m_markerStyle;
  LineStyle        m_lineStyle;
  FillStyle        m_fillStyle;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTSTYLE_H

