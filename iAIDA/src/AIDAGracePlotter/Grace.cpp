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
 * \file Implementation of class Grace
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Oct  1 14:46:39 2002
// 

#ifdef __sun
# include <stdlib.h>
# include <assert.h>
# include <math.h>
#endif

// also for gcc 3.3
#include <assert.h>
#include <cmath>

#include <iostream>
#include "Grace.h"
#include "StringConversion.h"
#include "grace_np.h"



/// extern C global function for catching Grace "exceptions"
void formattedAIDAGraceError(const char* msg)
{
  std::string s = msg;
  bool rethrow = true;
  // handle some known cases, re-throwing or suppressing as we like
  if (s == "GraceOneWrite: Broken pipe") {
    s = "Grace killed before Plotter"; rethrow = false;
  }
  if (s == "No grace subprocess") {
    s = "Grace not running! - please check your $PATH and try again"; 
    rethrow = true;
  }
  // else if (
  // ... etc ...
  if (rethrow) std::cerr << "*** AidaNative/Grace error - " << s << std::endl;
}



namespace AidaNative {

/////////////
//         //
// Statics //
//         //
/////////////

const int      Grace::PAGE_SIZE_X            =    300;
const int      Grace::PAGE_SIZE_Y            =    300;
const double   Grace::INITIAL_X_LOW          =      0.0;
const double   Grace::INITIAL_X_HIGH         =      1.0;
const double   Grace::INITIAL_Y_LOW          =      0.0;
const double   Grace::INITIAL_Y_HIGH         =      1.0;
const double   Grace::DEFAULT_MARGIN         =      0.15;
const double   Grace::DEFAULT_SCALE          =      1.10;
const double   Grace::ANNOTATION_FONT_SIZE   =      0.70;
const double   Grace::ANNOTATION_INDENT      =      0.04;
const double   Grace::DEFAULT_ANN_POS_X      = ANNOTATION_INDENT + DEFAULT_MARGIN;
const double   Grace::DEFAULT_ANN_POS_Y      = 1 - DEFAULT_MARGIN - ANNOTATION_INDENT;
const double   Grace::SCALE_FUDGE_FACTOR     =     35.0;
const double   Grace::LOWEST_DOUBLE          =     -1E9; //ND std limits?
const double   Grace::HIGHEST_DOUBLE         =      1E9; //ND std limits?
const int      Grace::DEFAULT_SYMBOL_SIZE    =    100;
const int      Grace::MAX_SYMBOL_SIZE        =    500;



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

Grace::Grace(int xsize, int ysize)
  : ok(false), m_currentGraph(0), m_rows(1), m_cols(1), m_numberOfAnnotations(0)
{
  // private constructor
  registerErrorCallback();
  start();
  initialize(xsize, ysize);
}




Grace::~Grace()
{
  if (isRunning()) stop();
}

  // ================================================================================
static int linecount=0;
  void AP_GracePrintf(const std::string &fmtIn) {
    const char * fmt = fmtIn.c_str();
#ifdef DEBUG_PRINT_GRACE
    std::cout << "apGPF0: line " << linecount++ << " : " 
	      << "fmt='" << fmt << "'"
	      << std::endl;
#endif // DEBUG_PRINT_GRACE
    GracePrintf(fmt);
  }

  template <class T>
  void AP_GracePrintf(const std::string &fmtIn, T val1) {
    const char * fmt = fmtIn.c_str();
#ifdef DEBUG_PRINT_GRACE
    std::cout << "apGPF1: line " << linecount++ << " : " 
	      << "fmt='" << fmt << "'"
	      << "val1=" << val1 << " " 
	      << std::endl;
#endif // DEBUG_PRINT_GRACE
    GracePrintf(fmt, val1);
  }
  template <class T1, class T2>
  void AP_GracePrintf(const std::string &fmtIn, T1 val1, T2 val2) {
    const char * fmt = fmtIn.c_str();
#ifdef DEBUG_PRINT_GRACE
    std::cout << "apGPF2: line " << linecount++ << " : " 
	      << "fmt='" << fmt << "'"
	      << "val1=" << val1 << " " 
	      << "val2=" << val2 << " " 
	      << std::endl;
#endif // DEBUG_PRINT_GRACE
    GracePrintf(fmt, val1, val2);
  }

template <class T1, class T2, class T3, class T4, class T5>
void AP_GracePrintf(const std::string &fmtIn, T1 val1, T2 val2, T3 val3, T4 val4, T5 val5) {
    const char * fmt = fmtIn.c_str();
#ifdef DEBUG_PRINT_GRACE
    std::cout << "apGPF5: line " << linecount++ << " : " 
	      << "fmt='" << fmt << "' "
	      << "val1=" << val1 << " " 
	      << "val2=" << val2 << " " 
	      << "val3=" << val3 << " " 
	      << "val4=" << val4 << " " 
	      << "val5=" << val5 << " " 
	      << std::endl;
#endif // DEBUG_PRINT_GRACE
    GracePrintf(fmt, val1, val2);
  }

  // ================================================================================



////////////////////
//                //
// Public methods //
//                //
////////////////////

Grace& Grace::theInstance(int xsize, int ysize)
{
  static Grace g(xsize, ysize);
  return g;
}




bool Grace::start()
{
  if (isRunning()) return false;
  
  const std::string xmgrace("xmgrace");
  int flag = GraceOpenVA(const_cast<char*>(xmgrace.c_str()), 2048, 
			 "-noask", "-barebones", "-nosafe", "-free",
			 NULL);
  if (flag == -1 || !GraceIsOpen()) {
    std::cerr << "*** AidaNative/Grace error - unable to start plotter"
	      << std::endl;
    ok = false;
  }
  else ok = true;
  return ok;
}

bool Grace::stop()
{
  if (GraceIsOpen()) GraceClose();
  ok = false;
  return true;
}

void Grace::hackRawGraceCommand(const std::string& cmd)
{
  AP_GracePrintf( cmd.c_str() );
}

bool Grace::isRunning()
{
  return GraceIsOpen();
}




bool Grace::initialize(int xsize, int ysize)
{
  if (!isRunning()) return false;
  // hack to stop scroll bars appearing :-)
  AP_GracePrintf("page size %d %d", xsize, ysize);
  // default x,y limits on empty plot displayed at setup
  AP_GracePrintf("world xmin %f", INITIAL_X_LOW);
  AP_GracePrintf("world ymin %f", INITIAL_Y_LOW);
  AP_GracePrintf("world xmax %f", INITIAL_X_HIGH);
  AP_GracePrintf("world ymax %f", INITIAL_Y_HIGH);
  // set plot margin to (def) 15% of page size
  AP_GracePrintf("view xmin %f",   DEFAULT_MARGIN);
  AP_GracePrintf("view xmax %f", 1-DEFAULT_MARGIN);
  AP_GracePrintf("view ymin %f",   DEFAULT_MARGIN);
  AP_GracePrintf("view ymax %f", 1-DEFAULT_MARGIN);
  // use a reasonable tick strategy
  autoTicks();
  // increase font size a bit
  AP_GracePrintf("xaxis label char size 1.2");
  AP_GracePrintf("yaxis label char size 1.2");
  // set legend box to transparent, top left corner (top right is tricky
  // because the top RIGHT corner moves depending on the legend text and 
  // the font size. IF the user wants to move it he can do it by hand
  // using the mouse or (in future) the parameters/layout of the region.
  setLegendBoxFill(empty);
  setLegendBoxPosition(DEFAULT_MARGIN, 1-DEFAULT_MARGIN);
  return true;
}




bool Grace::defineNewDataSetAsPoints1D(const DataSet& v, 
				       int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints = v.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string pointCommand = setSelection+" point %f, %f";
  const std::string yErrorPlusCommand  = setSelection+".y1[%d] = %f";
  const std::string yErrorMinusCommand = setSelection+".y2[%d] = %f";
  const std::string targetSelection = setSelection+" type xydydy";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& x = v[i];
    if (x.size() == 6) {
      const double binCenter  = x[0];
      const double binHeight  = x[1];
      const double errorPlus  = x[4];
      const double errorMinus = x[5];
      AP_GracePrintf(pointCommand.c_str(), binCenter, binHeight);
      AP_GracePrintf(yErrorPlusCommand.c_str(),  i, errorPlus);
      AP_GracePrintf(yErrorMinusCommand.c_str(), i, errorMinus);
      okay = true;
    }
  }
  return okay;
}




bool Grace::defineNewDataSetAsHisto1D(const DataSet& v, 
				      int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  bool firstPoint = true;
  double lastPointX = 0.0;
  const std::string com = "g"+gn+".s"+sn+" point %f, %f";

//-ap   std::cerr << "Grace::defineNewDataSetAsHisto1D> grace string: " << com 
//-ap  	    << " DS len = " << v.size()
//-ap  	    << std::endl;

  for (unsigned int i = 0; i < v.size(); ++i) {
  const std::vector<double>& x = v[i];
    if (x.size() == 6) {
      const double binCenter = x[0];
      const double binHeight = x[1];
      const double binLow    = binCenter - x[2];
      const double binHigh   = binCenter + x[3];
      if (firstPoint) {
	// add point at (xmin,0) so it draws correctly inside wider x limits
	AP_GracePrintf(com, binLow, 0.0);
	firstPoint = false;
      }
      AP_GracePrintf(com, binLow,  binHeight);
      AP_GracePrintf(com, binHigh, binHeight);
      okay = true;
      lastPointX = binHigh;
    }
  }
  if (okay) {
    // add point at (xmin,0) so it draws correctly inside wider x limits
    AP_GracePrintf(com.c_str(), lastPointX, 0.0);
  }

  return okay;
}




bool Grace::defineNewDataSetAsDots2D(const DataSet& points, 
				     int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints = points.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string xyValueCommand  = setSelection+" point %f, %f";
  const std::string targetSelection = setSelection+" type xy";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& v = points[i];
    if (v.size() > 1) {
      const double x  = v[0];
      const double y  = v[1];
      AP_GracePrintf(xyValueCommand.c_str(), x, y);
      okay = true;
    }
  }
  return okay;
}



bool Grace::defineNewDataSetAsBox2D(const DataSet& points,
				    int numberOfBins, 
				    int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  const DataSet scaledPoints = scaleForBoxPlot(points, numberOfBins);
  bool okay = false;
  const unsigned int numberOfPoints = scaledPoints.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string xyValueCommand  = setSelection+" point %f, %f";
  const std::string zValueCommand   = setSelection+".y1[%d] = %f";
  const std::string targetSelection = setSelection+" type xysize";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
    const std::vector<double>& v = scaledPoints[i];
    if (v.size() > 2) {
      const double x  = v[0];
      const double y  = v[1];
      const double z  = v[2];
      AP_GracePrintf(xyValueCommand.c_str(), x, y);
      AP_GracePrintf(zValueCommand.c_str(), i, z);
      okay = true;
    }
  }
  return okay;
}




bool Grace::defineNewDataSetAsColor2D(const DataSet& points, 
				      int graphNum, int datasetNum)
{
  //ND NOT WORKING!!!
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints = points.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string xyValueCommand  = setSelection+" point %f, %f";
  const std::string zValueCommand   = setSelection+".y1[%d] = %f";
  const std::string targetSelection = setSelection+" type xycolor";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& v = points[i];
    if (v.size() > 2) {
      const double x  = v[0];
      const double y  = v[1];
      const double z  = v[2];
      AP_GracePrintf(xyValueCommand.c_str(), x, y);
      AP_GracePrintf(zValueCommand.c_str(), i, z);
      okay = true;
    }
  }
  return okay;
}




bool Grace::defineNewDataSetAsPoints2D(const DataSet& v, 
				       int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints = v.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string pointCommand = setSelection+" point %f, %f";
  const std::string xErrorPlusCommand  = setSelection+".y2[%d] = %f";
  const std::string xErrorMinusCommand = setSelection+".y1[%d] = %f";
  const std::string yErrorPlusCommand  = setSelection+".y4[%d] = %f";
  const std::string yErrorMinusCommand = setSelection+".y3[%d] = %f";
  const std::string targetSelection = setSelection+" type xydxdxdydy";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& x = v[i];
    if (x.size() > 5) {
      const double binCenterX  = x[0];
      const double binCenterY  = x[1];
      const double errorXPlus  = x[2];
      const double errorXMinus = x[3];
      const double errorYPlus  = x[4];
      const double errorYMinus = x[5];
      AP_GracePrintf(pointCommand.c_str(), binCenterX, binCenterY);
      AP_GracePrintf(xErrorPlusCommand.c_str(),  i, errorXPlus);
      AP_GracePrintf(xErrorMinusCommand.c_str(), i, errorXMinus);
      AP_GracePrintf(yErrorPlusCommand.c_str(),  i, errorYPlus);
      AP_GracePrintf(yErrorMinusCommand.c_str(), i, errorYMinus);
      okay = true;
    }
  }
  return okay;
}




bool Grace::defineNewDataSetAsLine1D(const DataSet& points, 
				     int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints = points.size();
  const std::string setSelection = "g"+gn+".s"+sn;
  const std::string xyValueCommand  = setSelection+" point %f, %f";
  const std::string targetSelection = setSelection+" type xy";
  //AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& v = points[i];
    if (v.size() > 1) {
      const double x  = v[0];
      const double y  = v[1];
      AP_GracePrintf(xyValueCommand.c_str(), x, y);
      okay = true;
    }
  }
  return okay;
}




bool Grace::defineNewDataSetAsProfile(const DataSet& v, 
				      int graphNum, int datasetNum)
{
  std::string gn = to_string(graphNum);
  std::string sn = to_string(datasetNum);
  if (gn.length() < 1 || sn.length() < 1) {
    return false;
  }
  bool okay = false;
  const unsigned int numberOfPoints    = v.size();
  const std::string setSelection       = "g"+gn+".s"+sn;
  const std::string pointCommand       = setSelection+" point %f, %f";
  const std::string yErrorPlusCommand  = setSelection+".y1[%d] = %f";
  const std::string yErrorMinusCommand = setSelection+".y2[%d] = %f";
  const std::string yRMSPlusCommand    = setSelection+".y3[%d] = %f";
  const std::string yRMSMinusCommand   = setSelection+".y4[%d] = %f";
  const std::string targetSelection    = setSelection+" type xyboxplot";
  AP_GracePrintf(targetSelection.c_str());
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
  const std::vector<double>& x = v[i];
    if (x.size() == 6) {
      const double binCenter  = x[0];
      const double binHeight  = x[1];
      const double errorPlus  = binHeight + x[2];
      const double errorMinus = binHeight - x[3];
      const double rmsPlus    = binHeight + x[4];
      const double rmsMinus   = binHeight - x[5];
      AP_GracePrintf(pointCommand.c_str(), binCenter, binHeight);
      AP_GracePrintf(yErrorPlusCommand.c_str(),  i, errorPlus);
      AP_GracePrintf(yErrorMinusCommand.c_str(), i, errorMinus);
      AP_GracePrintf(yRMSPlusCommand.c_str(),  i, rmsPlus);
      AP_GracePrintf(yRMSMinusCommand.c_str(), i, rmsMinus);
      okay = true;
    }
  }
  return okay;
}





std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForPoints1D(const Grace::DataSet& v, double scale)
{
  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() == 6) {
      const double binCenter = v[i][0];
      const double binHeight = v[i][1];
      const double binXLow    = binCenter - v[i][2];
      const double binXHigh   = binCenter + v[i][3];
      const double binYLow    = binHeight - v[i][4];
      const double binYHigh   = binHeight + v[i][5];
      if (binXLow  < xlow)  xlow  = binXLow;
      if (binXHigh > xhigh) xhigh = binXHigh;
      if (binYLow  < ylow)  ylow  = binYLow;
      if (binYHigh > yhigh) yhigh = binYHigh;
    }
  }
  if (scale > 0.0) yhigh *= scale;
  else yhigh *= DEFAULT_SCALE;
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}




std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForHisto1D(const Grace::DataSet& v, double scale)
{
  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() == 6) {
      const double binCenter = v[i][0];
      const double binHeight = v[i][1];
      const double binXLow    = binCenter - v[i][2];
      const double binXHigh   = binCenter + v[i][3];
      const double binYLow    = 0;
      const double binYHigh   = binHeight;
      if (binXLow  < xlow)  xlow  = binXLow;
      if (binXHigh > xhigh) xhigh = binXHigh;
      if (binYLow  < ylow)  ylow  = binYLow;
      if (binYHigh > yhigh) yhigh = binYHigh;
    }
  }
  if (scale > 0.0) yhigh *= scale;
  else yhigh *= DEFAULT_SCALE;
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}




std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForProfile(const Grace::DataSet& v, double scale)
{
  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() == 6) {
      const double binCenter = v[i][0];
      const double binHeight = v[i][1];
      const double binXLow    = binCenter - v[i][2];
      const double binXHigh   = binCenter + v[i][3];
      const double binYLow    = binHeight - v[i][5];
      const double binYHigh   = binHeight + v[i][4];
      if (binXLow  < xlow)  xlow  = binXLow;
      if (binXHigh > xhigh) xhigh = binXHigh;
      if (binYLow  < ylow)  ylow  = binYLow;
      if (binYHigh > yhigh) yhigh = binYHigh;
    }
  }
  if (scale > 0.0) yhigh *= scale;
  else yhigh *= DEFAULT_SCALE;
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}




std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForDots2D(const Grace::DataSet& v, double scale)
{
  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() > 1) {
      const double x = v[i][0];
      const double y = v[i][1];
      if (x < xlow)  xlow  = x;
      if (x > xhigh) xhigh = x;
      if (y < ylow)  ylow  = y;
      if (y > yhigh) yhigh = y;
    }
  }
  const double xrange = xhigh - xlow;
  const double yrange = yhigh - ylow;
  const double centreX = xlow + (xrange/2.0);
  const double centreY = ylow + (yrange/2.0);
  double sc = (scale > 0.0 ? scale : DEFAULT_SCALE);
  const double newXRange = xrange * sc;
  const double newYRange = yrange * sc;
  xlow  = centreX - (newXRange/2.0);
  xhigh = centreX + (newXRange/2.0);
  ylow  = centreY - (newYRange/2.0);
  yhigh = centreY + (newYRange/2.0);
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}



std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForBox2D(const Grace::DataSet& v, double scale)
{
  //////// hack
  //ND
  AxisLimits xli = std::make_pair(0., 10.);
  AxisLimits yli = std::make_pair(0., 10.);
  return std::make_pair(xli, yli);
  ///////////////

  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() > 1) {
      const double x = v[i][0];
      const double y = v[i][1];
      if (x < xlow)  xlow  = x;
      if (x > xhigh) xhigh = x;
      if (y < ylow)  ylow  = y;
      if (y > yhigh) yhigh = y;
    }
  }
  if (scale > 0.0) yhigh *= scale;
  else yhigh *= DEFAULT_SCALE;
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}




std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForColor2D(const Grace::DataSet& v, double scale)
{
  return calculateLimitsForBox2D(v,scale);
}





std::pair<Grace::AxisLimits,Grace::AxisLimits> 
Grace::calculateLimitsForLine1D(const Grace::DataSet& v, double scale)
{
  double xhigh =  LOWEST_DOUBLE;
  double yhigh =  LOWEST_DOUBLE;
  double xlow  =  HIGHEST_DOUBLE;
  double ylow  =  HIGHEST_DOUBLE;
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (v[i].size() > 1) {
      const double x = v[i][0];
      const double y = v[i][1];
      if (x < xlow)  xlow  = x;
      if (x > xhigh) xhigh = x;
      if (y < ylow)  ylow  = y;
      if (y > yhigh) yhigh = y;
    }
  }
  if (scale > 0.0) yhigh *= scale;
  else yhigh *= DEFAULT_SCALE;
  AxisLimits xlim = std::make_pair(xlow, xhigh);
  if ( yhigh - ylow == 0 ) {
    yhigh = ylow + 1.;
  }
  AxisLimits ylim = std::make_pair(ylow, yhigh);
  return std::make_pair(xlim, ylim);
}




bool Grace::setLimits(const std::pair<Grace::AxisLimits,Grace::AxisLimits>& lims)
{
  bool a = setXLimits(lims.first);
  bool b = setYLimits(lims.second);
  return a && b;
}




bool Grace::setXLimits(const AxisLimits& lims)
{
  AP_GracePrintf("world xmin %f", lims.first);
  AP_GracePrintf("world xmax %f", lims.second);
  // add some ticks, otherwise Grace complains about too 
  // many ticks when using multiple zones, then add 
  // autoTicks to give Grace the freedom for manipulation
  AP_GracePrintf("xaxis tick major %f", lims.first/5.);
  AP_GracePrintf("xaxis tick minor %f", lims.first/10.);
  autoTicks();
  return true;
}


bool Grace::setYLimits(const AxisLimits& lims)
{
  AP_GracePrintf("world ymin %f", lims.first);
  AP_GracePrintf("world ymax %f", lims.second);
  // add some ticks, otherwise Grace complains about too 
  // many ticks when using multiple zones, then add 
  // autoTicks to give Grace the freedom for manipulation
  AP_GracePrintf("yaxis tick major %f", lims.first/5.);
  AP_GracePrintf("yaxis tick minor %f", lims.first/10.);
  autoTicks();
  return true;
}




bool Grace::printToPostscript(const std::string& filename)
{
  AP_GracePrintf("hardcopy device \"postscript\"");
  std::string print;
  print = "print to device";
  AP_GracePrintf(print.c_str());
  print = "print to \"" + filename + "\"";
  AP_GracePrintf(print.c_str());
  AP_GracePrintf("print");
  return true;
}



void Grace::hackPlotHisto1D(const DataSet& v, const std::string& t, 
			    double scaleFactor, int setNumber,
			    const std::string& annotation,
			    int graphNum)
{
  activateSet(setNumber);
  setSymbolType(setNumber, noSymbol);
  setSymbolFill(setNumber, empty);
  setFillColor(setNumber, yellow); //ND default for now (nicer than black)
  setFillType(setNumber, empty);
  if (setNumber == 0) {
    setGlobalTitle(t);
    std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForHisto1D(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsHisto1D(v, graphNum, setNumber);
}



void Grace::hackPlotProfile(const DataSet& v, const std::string& t, 
			    double scaleFactor, int setNumber,
			    const std::string& annotation,
			    int graphNum)
{
  std::string dataID = "s"+to_string(setNumber);
  activateSet(setNumber);
  setSymbolType(setNumber, circle);
  setSymbolFill(setNumber, empty);
  AP_GracePrintf(std::string(dataID+" line pattern 0").c_str());
  AP_GracePrintf(std::string(dataID+" fill type 0").c_str());
  AP_GracePrintf(std::string(dataID+" fill color 2").c_str());
  if (setNumber == 0) {
    setGlobalTitle(t);
    std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForProfile(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsProfile(v, graphNum, setNumber);
}




void Grace::hackPlotPoints1D(const DataSet& v, const std::string& t, 
			     double scaleFactor, int setNumber,
			     const std::string& annotation,
			     int graphNum)
{
  activateSet(setNumber);
  setSymbolType(setNumber, circle);
  setSymbolFill(setNumber, filled);
  setSymbolFillColor(setNumber, black);
  setSymbolOutlineColor(setNumber, black);
  setErrorBarColor(setNumber, black);
  setSymbolLineStyle(setNumber, noLine);
  setLineType(setNumber, noLine);
  if (setNumber == 0) {
    setGlobalTitle(t);
    std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForPoints1D(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsPoints1D(v, graphNum, setNumber);
}




void Grace::hackPlotBox2D(const DataSet& v, const std::string& t,
			  const std::pair<AxisLimits,AxisLimits>& hackLimits,
			  int numberOfBins,
			  double scaleFactor, int setNumber,
			  const std::string& annotation,
			  int graphNum)
{
  activateSet(setNumber);
  setSymbolType(setNumber, square);
  setSymbolFill(setNumber, empty);
  std::string dataID = "s"+to_string(setNumber);
  AP_GracePrintf(std::string(dataID+" line pattern 0").c_str());
  if (setNumber == 0) {
    setGlobalTitle(t);
    //std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForBox2D(v,scaleFactor);
    setLimits(hackLimits);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsBox2D(v, numberOfBins, graphNum, setNumber);
}



void Grace::hackPlotColor2D(const DataSet& v, const std::string& t, 
			    double scaleFactor, int setNumber,
			    const std::string& annotation,
			    int graphNum)
{
  activateSet(setNumber);
  setSymbolType(setNumber, square);
  setSymbolFill(setNumber, filled);
  std::string dataID = "s"+to_string(setNumber);
  AP_GracePrintf(std::string(dataID+" line pattern 0").c_str());
  if (setNumber == 0) {
    setGlobalTitle(t);
    std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForColor2D(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsColor2D(v, graphNum, setNumber);
}



void Grace::hackPlotCloud2D(const DataSet& v, const std::string& t, 
			    double scaleFactor, int setNumber,
			    const std::string& annotation,
			    int graphNum)
{
  activateSet(setNumber);
  setSymbolType(setNumber, circle);
  setSymbolFill(setNumber, empty);
  std::string dataID = "s"+to_string(setNumber);
  AP_GracePrintf(std::string(dataID+" line pattern 0").c_str());
  AP_GracePrintf(std::string(dataID+" symbol size 0").c_str());
  if (setNumber == 0) {
    setGlobalTitle(t);
    std::pair<AxisLimits,AxisLimits> lims = calculateLimitsForDots2D(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsDots2D(v, graphNum, setNumber);
}




void Grace::hackPlotFunction1D(const DataSet& v,
			       int np, double low, double high,
			       double scaleFactor, int setNumber,
			       const std::string& annotation,
			       int graphNum)
{
  std::string dataID = "s"+to_string(setNumber);
  AP_GracePrintf(std::string(dataID+" on").c_str());
  setSymbolType(setNumber, noSymbol);
  setSymbolFill(setNumber, empty);
  AP_GracePrintf(std::string(dataID+" line pattern 1").c_str());
  AP_GracePrintf(std::string(dataID+" line color 2").c_str());
  AP_GracePrintf(std::string(dataID+" symbol size 0").c_str());
  if (setNumber == 0) {
    //ND note no title plotting!
    std::pair<AxisLimits,AxisLimits> lims = 
      calculateLimitsForLine1D(v,scaleFactor);
    setLimits(lims);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsLine1D(v, graphNum, setNumber);
}





void Grace::hackPlotDataPointSet2D(const DataSet& v, const std::string& t, 
				   const std::pair<AxisLimits,AxisLimits>& limits,
				   double scaleFactor, int setNumber,
				   const std::string& annotation,
				   int graphNum)
{
  std::string dataID = "s"+to_string(setNumber);
  AP_GracePrintf(std::string(dataID+" on").c_str());
  setSymbolType(setNumber, circle);
  setSymbolFill(setNumber, filled);
  setSymbolLineStyle(setNumber, noLine);
  AP_GracePrintf(std::string(dataID+" line pattern 0").c_str());
  AP_GracePrintf(std::string(dataID+" line color 1").c_str());
  //AP_GracePrintf(std::string(dataID+" symbol size 1").c_str());
  if (setNumber == 0) {
    setGlobalTitle(t);
    setLimits(limits);
  }
  if (annotation.length() > 0) showAnnotation(annotation);
  defineNewDataSetAsPoints2D(v, graphNum, setNumber);  
}




void Grace::arrange(int rows, int columns)
{
  // update numbers ...
  m_rows = rows;
  m_cols = columns;

  double pageOffset = 0.1;
  double vGap = DEFAULT_MARGIN;
  double hGap = DEFAULT_MARGIN;
  AP_GracePrintf("arrange(%d,%d,%f,%f,%f)", rows, columns, pageOffset, hGap, vGap);
}




void Grace::withGraph(int n)
{
  m_currentGraph = n;
  AP_GracePrintf("focus g%d", n);
}




void Grace::withString(int n)
{
  AP_GracePrintf("with string %d", n);
}




int Grace::currentGraphNumber() const
{
  return m_currentGraph;
}



void Grace::killGraph(int n)
{
  AP_GracePrintf("kill g%d", n);
  setGlobalTitle(""); // remove title for this "graph"
  emptyHisto();
  removeAnnotation(n);
}

void Grace::removeAnnotation(int n)
{
  // remove the annotation going with this graph
  std::map<int, int>::iterator annoItr = m_annoMap.find(n);
  if (annoItr != m_annoMap.end()) { // we in fact have found an annotation
    withString(annoItr->second);
    setString("");
    showString(false);

    //-ap   std::cerr << "anno for graph " << n << " with string " << annoItr->second << " removed " << std::endl;
  }
}

void Grace::emptyHisto()
{
  AxisLimits xlim = std::make_pair(0,1);
  AxisLimits ylim = std::make_pair(0,1);
  std::pair<AxisLimits,AxisLimits> lims = std::make_pair(xlim, ylim);
  setLimits(lims);
}

void Grace::setEmpty()
{
  setGlobalTitle("");
  // emptyHisto();
  redraw();
}




void Grace::killDataSet(int graphNumber, int setNumber)
{
  AP_GracePrintf("kill g%d.s%d", graphNumber, setNumber);
}




bool Grace::redraw()
{
  autoTicks();
  AP_GracePrintf("redraw");
  return true;
}



bool Grace::setGlobalTitle(const std::string& title)
{
  std::string s = "title \"" + title + "\"";
  AP_GracePrintf(s.c_str());
  return true;
}


bool Grace::setXAxisLabel(const std::string& newLabel)
{
  std::string s = "xaxis label \"" + newLabel + "\"";
  AP_GracePrintf(s.c_str());
  return true;
}



bool Grace::setYAxisLabel(const std::string& newLabel)
{
  std::string s = "yaxis label \"" + newLabel + "\"";
  AP_GracePrintf(s.c_str());
  return true;
}



void Grace::setGlobalTitleFont(Font f)
{
  AP_GracePrintf("title font %d", f);
}



void Grace::setXAxisLabelFont(Font f)
{
  AP_GracePrintf("xaxis label font %d", f);
}



void Grace::setYAxisLabelFont(Font f)
{
  AP_GracePrintf("yaxis label font %d", f);
}



void Grace::setGlobalTitleSize(double s)
{
  AP_GracePrintf("title size %f", s);
}




void Grace::setXAxisLabelSize(double s)
{
  AP_GracePrintf("xaxis label char size %f", s);
}



void Grace::setYAxisLabelSize(double s)
{
  AP_GracePrintf("yaxis label char size %f", s);
}




void Grace::showLegend(bool b)
{
  std::string s = "legend ";
  s += (b ? "on" : "off");
  AP_GracePrintf(s.c_str());
}




void Grace::setLegendBoxFill(Fill f)
{
  AP_GracePrintf("legend box fill pattern %d", f);
}




bool Grace::setLegendBoxPosition(double x, double y)
{
  if (x < 0.0 || y < 0.0) return false;
  AP_GracePrintf("legend loctype view"); // ... 'view' or 'world'
  AP_GracePrintf("legend %f, %f", x, y);
  return true;
}




void Grace::activateSet(int setNumber)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" on";
  AP_GracePrintf(s.c_str());
}




void Grace::deactivateSet(int setNumber)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" off";
  AP_GracePrintf(s.c_str());
}




void Grace::setSymbolType(int setNumber, Grace::Symbol sym)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" symbol "+to_string(sym);
  AP_GracePrintf(s.c_str());
}



void Grace::setSymbolFill(int setNumber, Grace::Fill fill)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" symbol fill pattern "+to_string(fill);
  AP_GracePrintf(s.c_str());
}



void Grace::setLineType(int setNumber, Grace::Line line)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" line pattern "+to_string(line);
  AP_GracePrintf(s.c_str());
}



void Grace::setSymbolOutlineColor(int setNumber, Grace::Color c)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" symbol color "+to_string(c);
  AP_GracePrintf(s.c_str());  
}




void Grace::setSymbolFillColor(int setNumber, Grace::Color c)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" symbol fill color "+to_string(c);
  AP_GracePrintf(s.c_str());  
}




void Grace::setSymbolSize(int setNumber, int size)
{
  if (setNumber < 0 || size < 0 || size > MAX_SYMBOL_SIZE) return;
  std::string s = "s"+to_string(setNumber)+" symbol size "+to_string(size);
  AP_GracePrintf(s.c_str());  
}




void Grace::setSymbolLineStyle(int setNumber, Line line)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" symbol linestyle "+to_string(line);
  AP_GracePrintf(s.c_str());  
}




void Grace::setLineColor(int setNumber, Grace::Color c)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" line color "+to_string(c);
  AP_GracePrintf(s.c_str());  
}




void Grace::setErrorBarColor(int setNumber, Grace::Color c)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" errorbar color "+to_string(c);
  AP_GracePrintf(s.c_str());  
}





void Grace::setFillColor(int setNumber, Grace::Color c)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" fill color "+to_string(c);
  AP_GracePrintf(s.c_str());  
}




void Grace::setFillType(int setNumber, Grace::Fill f)
{
  if (setNumber < 0) return;
  std::string s = "s"+to_string(setNumber)+" fill type "+to_string(f);
  AP_GracePrintf(s.c_str());  
}





void Grace::autoTicks()
{
  AP_GracePrintf("autoticks");
}





Grace::DataSet Grace::scaleForBoxPlot(const Grace::DataSet& points,
				      int numberOfBins)
{
  assert (SCALE_FUDGE_FACTOR > 0.0);
  const double scale = SCALE_FUDGE_FACTOR/numberOfBins;
  const unsigned int numberOfPoints = points.size();
  if (numberOfPoints < 1 || points[0].size() < 2) {
    return DataSet(); //short circuit!
  }
  double lowestValue  =  1E10;
  double highestValue = -1E10;
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
    const std::vector<double>& v = points[i];
    if (v.size() > 2) {
      const double z = v[2];
      if (z < lowestValue)  lowestValue  = z;
      if (z > highestValue) highestValue = z;
    }
  }
  const double range = (highestValue - lowestValue);
  if (range <= 0.0) return DataSet();
  DataSet result;
  for (unsigned int i = 0; i < numberOfPoints; ++i) {
    const std::vector<double>& v = points[i];
    if (v.size() > 2) {
      const double z = v[2];
      const double scaledZ = (z - lowestValue) / range;
      std::vector<double> tmp;
      tmp.push_back(v[0]);
      tmp.push_back(v[1]);
      tmp.push_back(std::sqrt(scaledZ)*scale);
      result.push_back(tmp);
    }
  }
  return DataSet(result);
}



void Grace::setStringFontSize(double d)
{
  AP_GracePrintf("string char size %f", d);
}




void Grace::setStringPosition(double x, double y)
{
  if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0) return;
  AP_GracePrintf("string %f, %f", x, y);
}




void Grace::showString(bool on)
{
  std::string s = "string ";
  s += (on ? "on" : "off");
  AP_GracePrintf(s.c_str());
}




void Grace::setString(const std::string& s)
{
  const std::string com = "string def \"" + s + "\"";
  AP_GracePrintf(com.c_str());
}



std::pair<int, int> Grace::whereIs(int graph) 
{
  //    0 1 2 3    0 1 2
  //
  // 0  0 1 2 3    0 1 2
  // 1  4 5 6 7    3 4 5 
  // 2  8 9 1011   6 7 8

  int x = graph % m_cols;	// x index counted from left

  int y = graph/m_cols;		// y index counted from top
  y = m_rows - y - 1;		// ... now counted from bottom

//-ap   std::cerr << "whereIs for graph " << graph 
//-ap 	    << " x =" << x << ", y =" << y
//-ap 	    << std::endl;

  return std::make_pair(x,y);

}

void Grace::showAnnotation(const std::string& ann)
{
  if (ann.size() == 0) return;	// ignore empty strings ...

  std::pair<int, int> location = whereIs(m_currentGraph);

  int xRegionNr = location.first;
  int yRegionNr = location.second;

  // "stepsize" in x/y
  double dX = 1./(double(m_cols));
  double dY = 1./(double(m_rows));

  // the following needs to be a little better adjusted for the gaps ...
  // for now, just a rough guess ...
  double xPos = double(xRegionNr) * (dX*0.8) + 0.75*dX;
  double yPos = double(yRegionNr) * (dY*0.8) + 0.85*dY;


  std::map<int, int>::iterator annoItr = m_annoMap.find(m_currentGraph);
  if (annoItr == m_annoMap.end()) { // not yet in map, update ...
    m_annoMap[m_currentGraph] = m_numberOfAnnotations; 
    m_numberOfAnnotations++;
  }
  // now it's in, so we can use it ...
  annoItr = m_annoMap.find(m_currentGraph);

//-ap   std::cerr << "graph " << m_currentGraph
//-ap 	    << " xI " << xRegionNr
//-ap 	    << " yI " << yRegionNr
//-ap 	    << " x " << xPos
//-ap 	    << " y " << yPos
//-ap 	    << " size " << ann.size()
//-ap 	    << " second " << annoItr->second
//-ap 	    << std::endl;


  withString(annoItr->second);
  setStringFontSize(ANNOTATION_FONT_SIZE/(1.+double(m_rows-1)*0.7)); // scale somehow with number of rows
  setStringPosition(xPos, yPos);
  setString(ann.c_str());
  showString(true);
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void Grace::registerErrorCallback()
{
  GraceRegisterErrorFunction(&formattedAIDAGraceError);
}




} // end of namespace AidaNative

