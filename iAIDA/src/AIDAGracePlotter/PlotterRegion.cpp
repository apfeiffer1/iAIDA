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

// Implementation file for class PlotterRegion
// 
// Created by user sang on Wed Sep 18 17:59:06 2002
// Last update: Wed Sep 18 17:59:06 2002



#include "PlotterRegion.h"
#include "Style.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/IFunction.h"
#include "AIDA/IDataPointSet.h"
#include "AIDA/IAxis.h"
#include "AIDA/IAnnotation.h"
#include "GraceAIDAConverter.h"
#include "Utilities.h"
#include "StringConversion.h"
#include <unistd.h>
#include <utility>
#include <math.h>
#include <iostream>



namespace AidaNative {


/////////////
//         //
// Statics //
//         //
/////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
PlotterRegion::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;

  // (Example) rawGrace: 
  OptionList someOptions;
  t.addParameter("rawGrace", someOptions, "");

  return t;
}


const double PlotterRegion::DEFAULT_SCALE = 1.15;





////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

PlotterRegion::PlotterRegion(int graceGraphNumber,
			     Plotter* p,
			     double x, double y, 
			     double width, double height) 
  : grace           (Grace::theInstance()),
    parentPlotter   (p),
    parameterHandler(ParameterHandler(newDefaultPTable())),
    graphNumber     (graceGraphNumber),
    m_numberOfSets  (0)
{
  //ND
}



PlotterRegion::~PlotterRegion() 
{
  clear();
  grace.redraw();
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void PlotterRegion::clear()
{
  for (int i = 0; i < m_numberOfSets; ++i) {
    grace.killDataSet(graphNumber, i);
  }
  m_numberOfSets = 0;

  // set an empty histogram without title
  grace.setEmpty();

  // remove stat box from corner
  grace.removeAnnotation(graphNumber);
}



bool PlotterRegion::plot(const AIDA::IHistogram1D& h, const Style& s, 
			 const std::string& options)
{
  return plot(h, &s, options);
}




bool PlotterRegion::plot(const AIDA::IHistogram1D& h, const std::string& options)
{
  return plot(h, 0, options);
}



bool PlotterRegion::plot(const AIDA::IHistogram2D& h, const Style& s, 
			 const std::string& options)
{
  return plot(h, &s, options);
}




bool PlotterRegion::plot(const AIDA::IHistogram2D& h, const std::string& options)
{
  return plot(h, 0, options);
}




bool PlotterRegion::plot(const AIDA::IProfile1D& h, const std::string& options)
{
  return plot(h, 0, options);
}




bool PlotterRegion::plot(const AIDA::IProfile1D& h, const Style& s, 
			 const std::string& options)
{
  return plot(h, &s, options);
}




bool PlotterRegion::plot(const AIDA::ICloud2D& h, const std::string& options)
{
  return plot(h, 0, options);
}




bool PlotterRegion::plot(const AIDA::ICloud2D& h, const Style& s, 
			 const std::string& options)
{
  return plot(h, &s, options);
}




bool PlotterRegion::plot(const AIDA::IFunction& f, 
			 const std::string& options)
{
  return plot(f, 0, options);
}




bool PlotterRegion::plot(const AIDA::IFunction& f, const Style& s,
			 const std::string& options)
{
  return plot(f, &s, options);
}




bool PlotterRegion::plot(const AIDA::IDataPointSet& d, 
			 const std::string& options)
{
  return plot(d, 0, options);
}




bool PlotterRegion::plot(const AIDA::IDataPointSet& d, const Style& s,
			 const std::string& options)
{
  return plot(d, &s, options);
}




Style& PlotterRegion::style()
{
  return m_style;
}




bool PlotterRegion::setStyle(const Style& s)
{
  m_style = s;
  return true;
}




Layout& PlotterRegion::layout()
{
  return m_layout;
}




bool PlotterRegion::setLayout(const Layout& x)
{
  m_layout = x;
  return true;
}




bool PlotterRegion::setParameter(const std::string& par, const std::string& opt)
{
  if ( par == "rawGrace") {
    // need to replace single by double quotes in cmd ...
    grace.hackRawGraceCommand(opt);
  }
  return parameterHandler.setParameter(par, opt);
}



std::string PlotterRegion::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool ok = parameterHandler.getParameterValue(parameterName, s);
  if (ok) return s;
  else return "";
}



std::vector<std::string> PlotterRegion::availableParameters() const
{
  return std::vector<std::string>(parameterHandler.availableParameters());
}




std::vector<std::string> 
PlotterRegion::availableOptions(const std::string& parameterName) const
{
  std::vector<std::string> v;
  bool ok = parameterHandler.getAvailableOptions(parameterName, v);
  if (ok) return std::vector<std::string>(v);
  else return std::vector<std::string>();
}


std::string PlotterRegion::xLabel()
{
  return style().xAxisStyle().label();
}




std::string PlotterRegion::yLabel()
{
  return style().yAxisStyle().label();
}




bool PlotterRegion::setXLabel(const std::string& s)
{
  seizeFocus();
  style().xAxisStyle().setLabel(s);
  return grace.setXAxisLabel(s);
}



bool PlotterRegion::setYLabel(const std::string& s)
{
  seizeFocus();
  style().yAxisStyle().setLabel(s);
  return grace.setYAxisLabel(s);
}





bool PlotterRegion::setXLimits(double min, double max)
{
  seizeFocus();
  return grace.setXLimits(std::make_pair(min,max));
}




bool PlotterRegion::setYLimits(double min, double max)
{
  seizeFocus();
  return grace.setYLimits(std::make_pair(min,max));
}





LegendBox& PlotterRegion::legendBox()
{
  return m_legendBox;
}




bool PlotterRegion::setLegendBox(const LegendBox& x)
{
  m_legendBox = x;
  return true;
}




/////////////////////
//                 //
// private methods //
//                 //
/////////////////////

bool PlotterRegion::plot(const AIDA::IHistogram1D& h, 
			 const Style* s,
			 const std::string& options)
{
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(h);
  if (!contains(options, "overlay")) clear();
  Grace::DataSet d = GraceAIDAConverter::histo1DAsDoubles(&h);
  const std::string t = h.title();
  ++m_numberOfSets;
  applyStyles();
  if (contains(options, "markers")) 
    grace.hackPlotPoints1D(d, t, DEFAULT_SCALE, m_numberOfSets-1, ann, graphNumber); 
  else 
    grace.hackPlotHisto1D (d, t, DEFAULT_SCALE, m_numberOfSets-1, ann, graphNumber);
  return true;
}




bool PlotterRegion::plot(const AIDA::IHistogram2D& h, 
			 const Style* s,
			 const std::string& options)
{
  if (!squareBins(h)) return false;
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(h);
  if (!contains(options, "overlay")) clear();
  //ND error message? Messages generally are crap at the moment :-(
  Grace::DataSet d = GraceAIDAConverter::histo2DAsDoubles(&h);
  const std::string t = h.title();
  ++m_numberOfSets;
  applyStyles();
  const double xlow  = h.xAxis().lowerEdge();
  const double xhigh = h.xAxis().upperEdge();
  const double ylow  = h.yAxis().lowerEdge();
  const double yhigh = h.yAxis().upperEdge();
  std::pair<Grace::AxisLimits, Grace::AxisLimits> limits = 
    std::make_pair( std::make_pair(xlow,xhigh),
		    std::make_pair(ylow,yhigh) );
  //if (contains(options, "color")) grace.hackPlotColor2D(d, t, DEFAULT_SCALE,
  //m_numberOfSets-1, limits);
  //else
  grace.hackPlotBox2D(d, t, limits, h.xAxis().bins(),
		      DEFAULT_SCALE, m_numberOfSets-1, ann, graphNumber);
  return true;
}




bool PlotterRegion::plot(const AIDA::IProfile1D& h, 
			 const Style* s,
			 const std::string& options)
{
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(h);
  if (!contains(options, "overlay")) clear();
  Grace::DataSet d = GraceAIDAConverter::profile1DAsDoubles(&h);
  ++m_numberOfSets;
  const std::string t = h.title();
  applyStyles();
  grace.hackPlotProfile(d, t, DEFAULT_SCALE,
			m_numberOfSets-1, ann, graphNumber); 
  return true;
}




bool PlotterRegion::plot(const AIDA::ICloud2D& h, 
			 const Style* s,
			 const std::string& options)
{
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(h);
  if (!contains(options, "overlay")) clear();
  Grace::DataSet d = GraceAIDAConverter::cloud2DAsDoubles(&h);
  const std::string t = h.title();
  ++m_numberOfSets;
  applyStyles();
  grace.hackPlotCloud2D(d, t, DEFAULT_SCALE,
			m_numberOfSets-1, ann, graphNumber); 
  return true;
}




bool PlotterRegion::plot(const AIDA::IFunction& f, 
			 const Style* s,
			 const std::string& options)
{
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(f);
  if (!contains(options, "overlay")) clear();
  const int numberOfPoints = 500; //ND HACK!!!!
  std::pair<double,double> xlimits = extractLimits(options);
  Grace::DataSet d = GraceAIDAConverter::function1DAsDoubles(&f, 
							     xlimits.first,
							     xlimits.second,
							     numberOfPoints);
  applyStyles();
  ++m_numberOfSets;
  grace.hackPlotFunction1D(d, 
			   numberOfPoints, xlimits.first, xlimits.second,
			   DEFAULT_SCALE,
			   m_numberOfSets-1, ann, graphNumber);
  return true;
}




bool PlotterRegion::plot(const AIDA::IDataPointSet& dps, 
			 const Style* s,
			 const std::string& options)
{
  if (dps.dimension() != 2) {
    std::cout << "*** GracePlotter ERROR: only 2D DataPointSets supported" << std::endl;
    return false;
  }
  std::string ann;
  if (contains(options, "annotation")) ann = annoString(dps);
  if (!contains(options, "overlay")) clear();
  const double scale = DEFAULT_SCALE;
  const double xlow  = dps.lowerExtent(0);
  const double xhigh = dps.upperExtent(0);
  const double ylow  = dps.lowerExtent(1);
  const double yhigh = dps.upperExtent(1);
  const double xrange = xhigh - xlow;
  const double yrange = yhigh - ylow;
  const double xcentre = xlow + (xrange/2.0);
  const double ycentre = ylow + (yrange/2.0);
  const double newXRange = xrange * scale;
  const double newYRange = yrange * scale;
  const double newXLow   = xcentre - (newXRange/2.0);
  const double newXHigh  = xcentre + (newXRange/2.0);
  const double newYLow   = ycentre - (newYRange/2.0);
  const double newYHigh  = ycentre + (newYRange/2.0);
  Grace::AxisLimits axisLimitsX = std::make_pair(newXLow, newXHigh);
  Grace::AxisLimits axisLimitsY = std::make_pair(newYLow, newYHigh);
  Grace::DataSet d = GraceAIDAConverter::dataPointSet2DAsDoubles(&dps);
  applyStyles();
  ++m_numberOfSets;
  const std::string t = dps.title();
  grace.hackPlotDataPointSet2D(d, t,
			       std::make_pair(axisLimitsX, axisLimitsY),
			       DEFAULT_SCALE,
			       m_numberOfSets-1, ann, graphNumber);
  return true;
}




void PlotterRegion::seizeFocus() const
{
  if (graphNumber != grace.currentGraphNumber()) {
    grace.withGraph(graphNumber);
  }
}




void PlotterRegion::refresh()
{
  if (m_numberOfSets > 0) {
    seizeFocus();
    setAxisLabels();
    applyStyles();
  }
}




bool PlotterRegion::squareBins(const AIDA::IHistogram2D& h)
{
  return (h.xAxis().isFixedBinning() && 
	  h.yAxis().isFixedBinning() 
	  && binningSimilar(h.xAxis(), h.yAxis())
	  );
}




bool PlotterRegion::binningSimilar(const AIDA::IAxis& x, const AIDA::IAxis& y)
{
  if (x.bins() < 1 || y.bins() < 1) return false;
  const double xBinWidth = x.binWidth(0);
  const double yBinWidth = y.binWidth(0);
  if (xBinWidth <= 0.0 || yBinWidth <= 0.0) return false;
  const double tolerance = 0.01; //ND hack!!!!
  const double difference = fabs(xBinWidth - yBinWidth);
  return (difference/xBinWidth <= tolerance);
}




void PlotterRegion::applyStyles()
{
  applyAxisLabelStyles();
}




void PlotterRegion::applyAxisLabelStyles()
{
  Grace::Font xf  = Utilities::resolveFont(style().xAxisStyle().labelStyle());
  Grace::Font yf  = Utilities::resolveFont(style().yAxisStyle().labelStyle());
  const double xsize = 
    Utilities::fontSizeAsDouble(style().xAxisStyle().labelStyle().fontSize());
  const double ysize = 
     Utilities::fontSizeAsDouble(style().yAxisStyle().labelStyle().fontSize());
  seizeFocus();
  grace.setXAxisLabelFont(xf);
  grace.setYAxisLabelFont(yf);
  grace.setXAxisLabelSize(xsize);
  grace.setYAxisLabelSize(ysize);
}



void PlotterRegion::setAxisLabels()
{
  const std::string xlab = style().xAxisStyle().label();
  const std::string ylab = style().yAxisStyle().label();
  grace.setXAxisLabel(xlab);
  grace.setYAxisLabel(ylab);
}




bool PlotterRegion::contains(const std::string& s, 
			     const std::string& sub)
{
  return (s.find(sub) != std::string::npos);
}




std::pair<double,double> PlotterRegion::extractLimits(const std::string& theString)
{
  std::pair<double,double> def;
  def = std::make_pair(0.0, 1.0);
  typedef unsigned int SizeT; // STL incompatibility hack
  SizeT startPos = theString.find("[");
  SizeT endPos   = theString.find("]");
  if (startPos >= endPos            || 
      startPos == std::string::npos || 
      endPos   == std::string::npos   ) return def;
  std::string bothNumbers = theString.substr(startPos+1,endPos-1);
  SizeT commaPos = bothNumbers.find(",");
  if (commaPos == std::string::npos) return def;
  std::string firstNumber  = bothNumbers.substr(0,commaPos);
  std::string secondNumber = bothNumbers.substr(commaPos+1, std::string::npos);
  double xlow = -999.0, xhigh = -999.0;
  bool convertedOK1 = false, convertedOK2 = false;
  convertedOK1 = to_value(firstNumber, xlow);
  convertedOK2 = to_value(secondNumber, xhigh);
  if (!convertedOK1 || !convertedOK2) return def;
  return std::make_pair(xlow,xhigh);
}





std::string PlotterRegion::annoString(const AIDA::IHistogram1D& h)
{
  return annotationAsString(h.annotation());
}




std::string PlotterRegion::annoString(const AIDA::IHistogram2D& h)
{
  return annotationAsString(h.annotation());
}




std::string PlotterRegion::annoString(const AIDA::IProfile1D& h)
{
  return annotationAsString(h.annotation());
}




std::string PlotterRegion::annoString(const AIDA::IFunction& f)
{
  return annotationAsString(f.annotation());
}




std::string PlotterRegion::annoString(const AIDA::IDataPointSet& h)
{
  return annotationAsString(h.annotation());
}




std::string PlotterRegion::annoString(const AIDA::ICloud2D& h)
{
  return annotationAsString(h.annotation());
}




std::string PlotterRegion::annotationAsString(const AIDA::IAnnotation& a)
{
  const int n = a.size();
  if (n < 1) return std::string("");
  // this stuff was an experiment...
  /*
  int maxKeyLen = 0;
  int maxValLen = 0;
  for (int i = 0; i < n; ++i) {
    const std::string key = a.key(i);
    const std::string val = a.value(i);
    const int keyLen = key.length();
    const int valLen = val.length();
    if (keyLen > maxKeyLen) maxKeyLen = keyLen;
    if (valLen > maxValLen) maxValLen = valLen;
  }
  */
  // ... to here (attempt to get correct key/val alignment)
  std::string result;
  for (int i = 0; i < n; ++i) {
    std::string key = a.key(i);
    std::string val = a.value(i);
    result += (key + "  " );
    result += (val + "\\n");
  }
  return result;
}




} // end of namespace AidaNative



