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
 * \file Header file for class Grace
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Oct  1 14:46:39 2002
// 

#ifndef IAIDA_GRACE_H
#define IAIDA_GRACE_H


#include <utility>
#include <vector>
#include <string>
#include <map>

/// Global error trapping function
extern "C" {
void formattedGraceError(const char*);
}


namespace AidaNative {



/**
 * Singleton for controlling Grace - generally translates into
 * a call to [void GracePrintf(const char*)] !!!
 */

class Grace
{
public:
  /// Calls stop()
 ~Grace();

private:
  /// Only accessible via theInstance()
  Grace(int xsize = PAGE_SIZE_X, int ysize = PAGE_SIZE_Y);
  /// Not defined
  Grace(const Grace&);
  /// Not defined
  Grace& operator=(const Grace&);

public:
  enum PlotType {
    XY, XYZ, XYDX, XYDXDX, XYDY, XYDYDY, XYDXDXDYDY
  };
  enum Color {
    white   =  0, black = 1, red   = 2, green  = 3, blue  = 4, 
    yellow  =  5, brown = 6, grey  = 7, violet = 8, cyan  = 9, 
    magenta = 10
  };
  enum Font {
    timesNormal     =  0, timesItalic         =  1, timesBold         =  2, 
    timesItalicBold =  3, helveticaNormal     =  4, helveticaItalic   =  5, 
    helveticaBold   =  6, helveticaItalicBold =  7, courierNormal     =  8, 
    courierItalic   =  9, courierBold         = 10, courierItalicBold = 11,
    symbol          = 12, zapfDingbats        = 13
  };
  enum Symbol {
    noSymbol = 0, circle = 1, square = 2, diamond = 3
  };
  enum Fill {
    empty = 0, filled = 1
  };
  enum Line {
    noLine = 0, solid = 1, dotted = 2, dashed = 3
  };
  typedef std::pair<double,double> AxisLimits;
  typedef std::vector<double> DataPoint;
  typedef std::vector<DataPoint> DataSet;
  class RGB;

public:
  /// The only instance
  static Grace& theInstance(int xsize = PAGE_SIZE_X, int ysize = PAGE_SIZE_Y);
  /// Start process
  bool start();
  /// Shut down
  bool stop();
  bool isRunning();
  /// Set initial parameters
  bool initialize(int xsize = PAGE_SIZE_X, int ysize = PAGE_SIZE_Y);
  /// Arrange several zones
  void arrange(int rows=1, int columns=1);
  /// refresh
  bool redraw();
  bool defineNewDataSetAsPoints1D(const DataSet&, int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsHisto1D(const DataSet&, int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsDots2D(const DataSet&, int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsBox2D(const DataSet&, int numberOfBins, 
			       int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsColor2D(const DataSet&, int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsPoints2D(const DataSet&, int graphNum, int datasetNum = 0);
  bool defineNewDataSetAsProfile(const DataSet&, int graphNum, int datasetNum = 0);
  /// stream in the data for a particular dataset in a particular graph
  bool defineNewDataSetAsLine1D(const DataSet&, int graphNum, int datasetNum = 0);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForHisto1D(const DataSet& data, 
			      double scaleFactor = DEFAULT_SCALE);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForPoints1D(const DataSet& data, 
			       double scaleFactor = DEFAULT_SCALE);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForProfile(const DataSet& data, 
			      double scaleFactor = DEFAULT_SCALE);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForDots2D(const DataSet& data, 
			     double scaleFactor = DEFAULT_SCALE);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForBox2D(const DataSet& data, 
			    double scaleFactor = DEFAULT_SCALE);
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForColor2D(const DataSet& data, 
			      double scaleFactor = DEFAULT_SCALE);
  /// Set the axis limits for a given representation
  std::pair<AxisLimits,AxisLimits> 
    calculateLimitsForLine1D(const DataSet& data, 
			     double scaleFactor = DEFAULT_SCALE);
  bool setLimits(const std::pair<AxisLimits,AxisLimits>&);
  bool setXLimits(const AxisLimits& lims);
  bool setYLimits(const AxisLimits& lims);
  void hackPlotPoints1D(const DataSet&, const std::string& title, 
			double scaleFactor = DEFAULT_SCALE, int setNumber=0,
			const std::string& annotation = "", int graphNum=0);
  void hackPlotHisto1D(const DataSet&, const std::string& title, 
		       double scaleFactor = DEFAULT_SCALE, int setNumber=0,
		       const std::string& annotation = "", int graphNum=0);
  void hackPlotProfile(const DataSet&, const std::string& title, 
		       double scaleFactor = DEFAULT_SCALE, int setNumber=0,
		       const std::string& annotation = "", int graphNum=0);
  void hackPlotCloud2D(const DataSet&, const std::string& title, 
		       double scaleFactor = 1.0, int setNumber=0,
		       const std::string& annotation = "", int graphNum=0);
  void hackPlotBox2D(const DataSet&, const std::string& title, 
		     const std::pair<AxisLimits,AxisLimits>&,
		     int numberOfBins,
		     double scaleFactor = DEFAULT_SCALE, int setNumber=0,
		     const std::string& annotation = "", int graphNum=0);
  void hackPlotColor2D(const DataSet&, const std::string& title, 
		       double scaleFactor = DEFAULT_SCALE, int setNumber=0,
		       const std::string& annotation = "", int graphNum=0);
  void hackPlotFunction1D(const DataSet&,  
			  int np, double xlow, double xhigh,
			  double scaleFactor = DEFAULT_SCALE, int setNumber=0,
			  const std::string& annotation = "", int graphNum=0);
  void hackPlotDataPointSet2D(const DataSet&, const std::string& title, 
			      const std::pair<AxisLimits,AxisLimits>&,
			      double scaleFactor = DEFAULT_SCALE, int setNumber=0,
			      const std::string& annotation = "", int graphNum=0);
  DataSet scaleForBoxPlot(const DataSet&, int);
  bool printToPostscript(const std::string& filename);
  /// Set focus to a particular graph
  void withGraph(int);
  /// Set focus to a particular string
  void withString(int);
  /// Get the number of the graph with current focus
  int  currentGraphNumber() const;
  /// Remove a graph and all its datasets
  void killGraph(int);
  /// remove a single dataset from a graph
  void killDataSet(int graph, int set);
  bool setGlobalTitle(const std::string&);
  bool setXAxisLabel(const std::string&);
  bool setYAxisLabel(const std::string&);
  void setGlobalTitleFont(Font);
  void setXAxisLabelFont(Font);
  void setYAxisLabelFont(Font);
  void setGlobalTitleSize(double);
  void setXAxisLabelSize(double);
  void setYAxisLabelSize(double);
  void showLegend(bool);
  /// coordinates of the top left corner (in viewport view)
  bool setLegendBoxPosition(double x, double y);
  void setLegendBoxFill(Fill);
  void activateSet(int setNumber);
  void deactivateSet(int setNumber);
  void setSymbolType(int setNumber, Symbol);
  void setSymbolFill(int setNumber, Fill);
  void setSymbolOutlineColor(int setNumber, Color);
  void setSymbolLineStyle(int setNumber, Line);
  void setSymbolFillColor(int setNumber, Color);
  void setErrorBarColor(int setNumber, Color);
  void setFillColor(int setNumber, Color);
  void setFillType(int setNumber, Fill);
  void setSymbolSize(int setNumber, int size = DEFAULT_SYMBOL_SIZE);
  void setLineType(int setNumber, Line);
  void setLineColor(int setNumber, Color);
  void autoTicks();
  void showAnnotation(const std::string&);
  void setStringFontSize(double);
  void setStringPosition(double xTopLeft, double yTopLeft);
  void showString(bool on);
  void setString(const std::string&);

  void setEmpty();
  void emptyHisto();

  void removeAnnotation(int graphNumber);

  void hackRawGraceCommand(const std::string&);

private:
  void registerErrorCallback();

  std::pair<int, int> whereIs(int graphNumber);

private:
  bool ok;
  int m_currentGraph;

  int m_rows;
  int m_cols;

  std::map<int, int> m_annoMap;
  int m_numberOfAnnotations;
  
private:
  static const int      PAGE_SIZE_X, PAGE_SIZE_Y;
  static const double   INITIAL_X_LOW, INITIAL_X_HIGH;
  static const double   INITIAL_Y_LOW, INITIAL_Y_HIGH;
  static const double   DEFAULT_MARGIN;
  static const double   DEFAULT_SCALE;
  static const int      DEFAULT_SYMBOL_SIZE;
  static const int      MAX_SYMBOL_SIZE;
  /// Overall scale for the boxes in box plot
  static const double   SCALE_FUDGE_FACTOR;
  static const double   ANNOTATION_FONT_SIZE;
  /// indent (in fraction of page size) of annotation wrt frame
  static const double   ANNOTATION_INDENT;
  /// Default position for top left corner of annotation
  static const double   DEFAULT_ANN_POS_X;
  static const double   DEFAULT_ANN_POS_Y;
  // std limits ??
  static const double   LOWEST_DOUBLE, HIGHEST_DOUBLE;

};



} // end of namespace AidaNative


#endif // IAIDA_GRACE_H

