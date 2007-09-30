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

//
// (c) CERN 2002
//
// Header file for class Style
// 
// Created by user sang on Tue Sep 24 14:36:07 2002
//

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_STYLE_H
#define IAIDA_QPLOTTER_NEW_VIEWER_STYLE_H



#include "AxisStyle.h"
#include "LegendBoxStyle.h"
#include "PlotStyle.h"
#include "TitleStyle.h"



namespace AidaNative {



/**
 * Aggregation of styles (like the 'graphical context'
 * in X11). Not complete!!!
 *
 * <b>IMPORTANT</b>: calling reset() on any style object 
 * recursively calls reset() on all the aggregated objects.
 */

class Style
{
public: 
  Style(const std::string& nameOfDefaultTable = ""); 
  virtual ~Style(void);

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
  /// Get the global style for the data plots
  virtual PlotStyle& plotStyle();
  /// Set the global style for the data plots
  virtual void setPlotStyle(const PlotStyle&);
  /// Set the axis style for the x axis
  virtual void setXAxisStyle(const AxisStyle&);
  /// Set the axis style for the y axis
  virtual void setYAxisStyle(const AxisStyle&);
  /// Set the axis style for the z axis
  virtual void setZAxisStyle(const AxisStyle&);
  /// Set the style for the title
  virtual void setTitleStyle(const TitleStyle&);
  /// Set the style for the legend box
  virtual void setLegendBoxStyle(const LegendBoxStyle&);
  /// Get access to the x axis style for direct manipulation
  virtual AxisStyle& xAxisStyle();
  /// Get access to the y axis style for direct manipulation
  virtual AxisStyle& yAxisStyle();
  /// Get access to the z axis style for direct manipulation
  virtual AxisStyle& zAxisStyle();
  /// Get access to title style for direct manipulation
  virtual TitleStyle& titleStyle();
  /// Get access to legend style for direct manipulation
  virtual LegendBoxStyle& legendBoxStyle();

  //ND is this gone in A3?
  /// Toggle the display of a given Annotation item in the default legend box
  //virtual void showInLegendBox(const std::string& item, bool);

private:
  static ParameterTable newDefaultPTable(const std::string& name = "");

private:
  ParameterHandler parameterHandler;
  PlotStyle        m_plotStyle;
  AxisStyle        m_xAxisStyle;
  AxisStyle        m_yAxisStyle;
  AxisStyle        m_zAxisStyle;
  TitleStyle       m_titleStyle;
  LegendBoxStyle   m_legendBoxStyle;

};


} // end of namespace AidaNative



#endif // IAIDA_QPLOTTER_NEW_VIEWER_STYLE_H

