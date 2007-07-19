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
 * \file Implementation of class GraceAIDAConverter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Mon Sep 30 19:27:10 2002
// 



#include <iostream>
#include "GraceAIDAConverter.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/IAxis.h"
#include "AIDA/IFunction.h"
#include "AIDA/IDataPointSet.h"
#include "AIDA/IDataPoint.h"
#include "AIDA/IMeasurement.h"



namespace AidaNative {

  // const TextMessage 
  // GraceAIDAConverter::functionIsNot1D("function is not 1D", "GraceAIDAConverter");

  // const TextMessage 
  // GraceAIDAConverter::dataPointSetIsNot2D("data point set is not 2D", "GraceAIDAConverter");



////////////////////
//                //
// Public methods //
//                //
////////////////////

std::vector<std::vector<double> >
GraceAIDAConverter::histo1DAsDoubles(const AIDA::IHistogram1D* histo)
{
  /*
  //////////////////////////////////////////
  //ND quick hack pending Emmanuel's real version
  std::cout << "*** ATTENTION! using histo hack" << std::endl;
  std::vector<double> d;
  std::vector<std::vector<double> > v;
  d.push_back(0.5); d.push_back(1.0); d.push_back(0.5); d.push_back(0.5); d.push_back(1.0); d.push_back(1.0);
  v.push_back(d); d.clear();
  d.push_back(1.5); d.push_back(4.0); d.push_back(0.5); d.push_back(0.5); d.push_back(2.0); d.push_back(2.0);
  v.push_back(d); d.clear();
  d.push_back(2.5); d.push_back(9.0); d.push_back(0.5); d.push_back(0.5); d.push_back(3.0); d.push_back(3.0);
  v.push_back(d); d.clear();
  d.push_back(3.5); d.push_back(16.0);d.push_back(0.5); d.push_back(0.5); d.push_back(4.0); d.push_back(4.0);
  v.push_back(d); d.clear();
  //////////////////////////////////////
  }
  */

  if (histo == 0) {
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > result;
  const AIDA::IAxis& axis = histo->axis();
  const int nb = axis.bins();
  for (int i = 0; i < nb; ++i) {
    std::vector<double> xyz;
    const double lowX       = axis.binLowerEdge(i);
    const double highX      = axis.binUpperEdge(i);
    const double widthX     = axis.binWidth(i);
    const double centerX    = lowX + widthX/2.0;
    const double errorXLow  = centerX - lowX;
    const double errorXHigh = highX - centerX;
    const double centerY    = histo->binHeight(i);
    const double errorYLow  = histo->binError(i);
    const double errorYHigh = errorYLow;
    xyz.push_back(centerX);
    xyz.push_back(centerY);
    xyz.push_back(errorXLow);
    xyz.push_back(errorXHigh);
    xyz.push_back(errorYLow);
    xyz.push_back(errorYHigh);
    result.push_back(xyz);
  }
  return std::vector<std::vector<double> >(result);
}




std::vector<std::vector<double> > 
GraceAIDAConverter::histo2DAsDoubles(const AIDA::IHistogram2D* histo)
{
  if (histo == 0) {
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > result;
  const AIDA::IAxis& xaxis = histo->xAxis();
  const AIDA::IAxis& yaxis = histo->yAxis();
  const int nbx = xaxis.bins();
  const int nby = yaxis.bins();
  double lowz  =  1E9;
  double highz = -1E9;
  for (int i = 0; i < nbx; ++i) {
    for (int j = 0; j < nby; ++j) {
      const double z = histo->binHeight(i,j);
      if (z > highz) highz = z;
      if (z < lowz)  lowz  = z;
    }
  }

  for (int i = 0; i < nbx; ++i) {
    const double lowX    = xaxis.binLowerEdge(i);
    const double highX   = xaxis.binUpperEdge(i);
    const double widthX  = highX - lowX;
    const double centerX = lowX + (widthX)/2.0;
    for (int j = 0; j < nby; ++j) {
      std::vector<double> xyz;
      const double lowY    = yaxis.binLowerEdge(j);
      const double highY   = yaxis.binUpperEdge(j);
      const double widthY  = highY - lowY;
      const double centerY = lowY + (widthY)/2.0;
      const double height  = histo->binHeight(i,j);
      xyz.push_back(centerX);
      xyz.push_back(centerY);
      xyz.push_back((height*41)/(highz*nbx));
      xyz.push_back(lowX);
      xyz.push_back(highX);
      xyz.push_back(lowY);
      xyz.push_back(highY);
      result.push_back(xyz);
    }
  }
  return std::vector<std::vector<double> >(result);
}




std::vector<std::vector<double> >
GraceAIDAConverter::profile1DAsDoubles(const AIDA::IProfile1D* histo)
{
  if (histo == 0) {
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > result;
  const AIDA::IAxis& axis = histo->axis();
  const int nb = axis.bins();
  for (int i = 0; i < nb; ++i) {
    std::vector<double> v;
    const double low        = axis.binLowerEdge(i);
    const double width      = axis.binWidth(i);
    const double center     = low + width/2.0;
    const double height     = histo->binHeight(i);
    const double upperError = histo->binError(i);
    const double lowerError = upperError;
    const double upperRMS   = histo->binRms(i);
    const double lowerRMS   = upperRMS;
    v.push_back(center);
    v.push_back(height);
    v.push_back(upperError);
    v.push_back(lowerError);
    v.push_back(upperRMS);
    v.push_back(lowerRMS);
    result.push_back(v);
  }
  return std::vector<std::vector<double> >(result);
}




std::vector<std::vector<double> >
GraceAIDAConverter::cloud2DAsDoubles(const AIDA::ICloud2D* histo)
{
  if (histo == 0) {
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > result;
  const int nev = histo->entries();
  for (int i = 0; i < nev; ++i) {
    std::vector<double> v;
    const double x = histo->valueX(i);
    const double y = histo->valueY(i);
    v.push_back(x);
    v.push_back(y);
    result.push_back(v);
  }
  return std::vector<std::vector<double> >(result);
}




std::vector<std::vector<double> >
GraceAIDAConverter::function1DAsDoubles(const AIDA::IFunction* f, 
					double min, double max, int n)
{
  if (f == 0 || f->dimension() != 1 || min >= max || n < 2) {
    if (f->dimension() != 1) {
      const TextMessage funNot1D("function is not 1D", "GraceAIDAConverter");
      funNot1D.show();
    }
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > v;
  const double range = max - min;
  const double step  = range / n;
  std::vector<double> xyvec;
  for (double x = min; x <= max; x += step) {
    xyvec.push_back(x);
    const double y = f->value(xyvec);
    xyvec.push_back(y);
    v.push_back(xyvec);
    xyvec.clear();
  }
  return std::vector<std::vector<double> >(v);
}




std::vector<std::vector<double> > 
GraceAIDAConverter::dataPointSet2DAsDoubles(const AIDA::IDataPointSet* d)
{
  if (d == 0 || d->dimension() != 2) {
    if (d->dimension() != 2) {
      const TextMessage dpsNot2D("data point set is not 2D", "GraceAIDAConverter");
      dpsNot2D.show();
    }
    return std::vector<std::vector<double> >();
  }
  std::vector<std::vector<double> > result;
  std::vector<double> v;
  const int np = d->size();
  for (int i = 0; i < np; ++i) {
    const AIDA::IDataPoint* point = d->point(i);
    if (point->dimension() != 2) continue;
    const AIDA::IMeasurement* mx = point->coordinate(0);
    const AIDA::IMeasurement* my = point->coordinate(1);
    if (!mx || !my) continue;
    const double centreX      = mx->value();
    const double centreY      = my->value();
    const double xErrorMinus  = mx->errorMinus();
    const double xErrorPlus   = mx->errorPlus();
    const double yErrorMinus  = my->errorMinus();
    const double yErrorPlus   = my->errorPlus();
    v.push_back(centreX);
    v.push_back(centreY);
    v.push_back(xErrorMinus);
    v.push_back(xErrorPlus);
    v.push_back(yErrorMinus);
    v.push_back(yErrorPlus);
    result.push_back(v);
    v.clear();
  }
  return std::vector<std::vector<double> >(result);
}



} // end of namespace AidaNative

