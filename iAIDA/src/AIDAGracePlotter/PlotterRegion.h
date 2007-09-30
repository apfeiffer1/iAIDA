// -*- C++ -*-

//
// (c) CERN 2002
//

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
// Header file for class PlotterRegion
// 
// Created by user sang on Wed Sep 18 17:59:06 2002
// Last update: Wed Sep 18 17:59:06 2002
//

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_PLOTTER_REGION_H
#define IAIDA_QPLOTTER_NEW_VIEWER_PLOTTER_REGION_H


#include "Grace.h"
#include "Plotter.h"
#include "Style.h"
#include "Layout.h"
#include "LegendBox.h"


namespace AIDA {
  class IHistogram1D;
  class IHistogram2D;
  class IProfile1D;
  class ICloud2D;
  class IFunction;
  class IDataPointSet;
  class IAxis;
  class IAnnotation;
}




namespace AidaNative {


class Style;
class Layout;
class TextBox;
class LegendBox;



class PlotterRegion
{
public: 
  PlotterRegion(int graceGraphNumber, Plotter* parent,
		double x, double y, double width, double height); 
  virtual ~PlotterRegion(void);

public:
  virtual void clear();
  virtual void refresh();
  // not in Aida 3
  //virtual bool setClipping(bool);
  virtual bool plot(const AIDA::IHistogram1D&, const Style&, 
		    const std::string& options = "");
  virtual bool plot(const AIDA::IHistogram1D&, const std::string& options = "");
  virtual bool plot(const AIDA::IHistogram2D&, const Style&, 
		    const std::string& options = "");
  virtual bool plot(const AIDA::IHistogram2D&, const std::string& options = "");
  virtual bool plot(const AIDA::IProfile1D&, const Style&, 
		    const std::string& options = "");
  virtual bool plot(const AIDA::IProfile1D&, const std::string& options = "");
  virtual bool plot(const AIDA::ICloud2D&, const Style&,
		    const std::string& options = "");
  virtual bool plot(const AIDA::ICloud2D&, const std::string& options = "");
  virtual bool plot(const AIDA::IFunction&, const Style&,
		    const std::string& options = "");
  virtual bool plot(const AIDA::IFunction&, const std::string& options = "");
  virtual bool plot(const AIDA::IDataPointSet&, const std::string& options = "");
  virtual bool plot(const AIDA::IDataPointSet&, const Style&,
		    const std::string& options = "");


  virtual Style& style();
  virtual bool setStyle(const Style&);
  /// apply it now
  //virtual bool applyStyle(const Style&);
  virtual Layout& layout();
  virtual bool setLayout(const Layout&);
  virtual bool setParameter(const std::string&, const std::string& options);
  std::string parameterValue(const std::string& parameterName) const;
  virtual std::vector<std::string> availableParameters() const;
  virtual std::vector<std::string> availableOptions(const std::string& parameterName) const;
  //ND keep as convenience methods?
  virtual std::string xLabel();
  virtual std::string yLabel();
  virtual bool setXLabel(const std::string&);
  virtual bool setYLabel(const std::string&);
  virtual bool setXLimits(double min, double max);
  virtual bool setYLimits(double min, double max);
  virtual LegendBox& legendBox();
  virtual bool setLegendBox(const LegendBox&);

private:
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  void seizeFocus() const;
  bool plot(const AIDA::IHistogram1D&, const Style*, const std::string& options);
  bool plot(const AIDA::IHistogram2D&, const Style*, const std::string& options);
  bool plot(const AIDA::IProfile1D&, const Style*, const std::string& options);
  bool plot(const AIDA::ICloud2D&, const Style*, const std::string& options);
  bool plot(const AIDA::IFunction&, const Style*, const std::string& options);
  bool plot(const AIDA::IDataPointSet&, const Style*, const std::string& options);
  /// Recursively apply all the styles which are controlled at this level
  void applyStyles();
  void applyAxisLabelStyles();
  void setAxisLabels();
  
private:
  /// returns true if 'wholeString' contains 'subString'
  static bool contains(const std::string& wholeString, const std::string& subString);
  static bool squareBins(const AIDA::IHistogram2D&);
  static bool binningSimilar(const AIDA::IAxis& x, const AIDA::IAxis& y);
  static std::pair<double,double> extractLimits(const std::string&);
  static std::string annoString(const AIDA::IHistogram1D&);
  static std::string annoString(const AIDA::IHistogram2D&);
  static std::string annoString(const AIDA::IProfile1D&);
  static std::string annoString(const AIDA::ICloud2D&);
  static std::string annoString(const AIDA::IFunction&);
  static std::string annoString(const AIDA::IDataPointSet&);
  static std::string annotationAsString(const AIDA::IAnnotation& a);

private:
  typedef std::vector<std::pair<int,Grace::DataSet> > SetList;
  static const double DEFAULT_SCALE;

private:
  Grace&    grace;
  Plotter*  parentPlotter;
  ParameterHandler parameterHandler;
  int       graphNumber;
  SetList   setList;
  Style     m_style;
  Layout    m_layout;
  LegendBox m_legendBox;
  int       m_numberOfSets;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_PLOTTER_REGION_H

