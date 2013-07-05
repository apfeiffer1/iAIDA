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
 * \file Implementation of class AIDA_PlotterRegion
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:22:41 2002
// 



#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IDataPointSet.h"
#include "AIDA_PlotterRegion.h"
#include "AIDA_PlotterStyle.h"
#include "AIDA_Info.h"
#include "AIDA_PlotterLayout.h"
#include "AIDAGracePlotter/PlotterRegion.h"


#include <iostream>


namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_PlotterRegion::AIDA_PlotterRegion()
  : rep(0),
    ownRep(false)
{
  // no-op
}





AIDA_PlotterRegion::AIDA_PlotterRegion(PlotterRegion* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}




AIDA_PlotterRegion::~AIDA_PlotterRegion()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

bool AIDA_PlotterRegion::plot(const AIDA::IBaseHistogram& histogram, 
			      const std::string& options)
{
  return plot(histogram, 0, options);
}




bool AIDA_PlotterRegion::plot(const AIDA::IBaseHistogram& histogram, 
			      const AIDA::IPlotterStyle& style, 
			      const std::string& options)
{
  return plot(histogram, &style, options);
}




bool AIDA_PlotterRegion::plot(const AIDA::IFunction& function, 
			      const std::string& options)
{
  return plot(function, 0, options);
}




bool AIDA_PlotterRegion::plot(const AIDA::IFunction& function, 
			      const AIDA::IPlotterStyle& style, 
			      const std::string& options)
{
  return plot(function, &style, options);
}




bool AIDA_PlotterRegion::plot(const AIDA::IDataPointSet& dataPointSet, 
			      const std::string& options)
{
  return plot(dataPointSet, 0, options);
}




bool AIDA_PlotterRegion::plot(const AIDA::IDataPointSet& dataPointSet, 
			      const AIDA::IPlotterStyle& style, 
			      const std::string& options)
{
  return plot(dataPointSet, &style, options);
}




bool AIDA_PlotterRegion::remove(const AIDA::IBaseHistogram& histogram)
{
  //ND
  return 0;
}




bool AIDA_PlotterRegion::remove(const AIDA::IFunction& function)
{
  //ND
  return 0;
}




bool AIDA_PlotterRegion::remove(const AIDA::IDataPointSet& dataPointSet)
{
  //ND
  return 0;
}




void AIDA_PlotterRegion::clear()
{
  if (rep == 0) {
    crisis();
    return;
  }
  rep->clear();
}




bool AIDA_PlotterRegion::setParameter(const std::string& parameter, 
					const std::string& options)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->setParameter(parameter, options);
}




std::vector<std::string>
AIDA_PlotterRegion::availableParameterOptions(const std::string& parameter) const
{  
  if (rep == 0) {
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(parameter);
}




std::vector<std::string> AIDA_PlotterRegion::availableParameters() const
{ 
  if (rep == 0) {
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::string AIDA_PlotterRegion::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




AIDA::IPlotterStyle& AIDA_PlotterRegion::style()
{
  static AIDA_PlotterStyle pr;
  if (rep == 0) {
    crisis();
    return pr;
  }
  Style& realStyle = rep->style();
  pr.setRep(realStyle, false);
  return pr;  
}




bool AIDA_PlotterRegion::setStyle(const AIDA::IPlotterStyle& style)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  const AIDA_PlotterStyle* pr = dynamic_cast<const AIDA_PlotterStyle*>(&style);
  if (pr == 0) return false; //ND plugnplay message?
  AIDA_PlotterStyle* ncpr = const_cast<AIDA_PlotterStyle*>(pr);
  Style* gs = ncpr->theRep();
  if (gs == 0) return false;
  else {
    rep->setStyle(*gs);  
    return true;
  }
}




bool AIDA_PlotterRegion::applyStyle(const AIDA::IPlotterStyle& style)
{
  //ND no difference yet :-(
  return setStyle(style);
}




void AIDA_PlotterRegion::setTitle(const std::string& title)
{
  //ND not possible in grace plotter yet
  return;
}




bool AIDA_PlotterRegion::setXLimits(double min, double max)
{  
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->setXLimits(min, max);
}




bool AIDA_PlotterRegion::setYLimits(double min, double max)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->setYLimits(min, max);
}




bool AIDA_PlotterRegion::setZLimits(double min, double max)
{
  return false; // no Z limits in Grace
}




AIDA::IPlotterLayout& AIDA_PlotterRegion::layout()
{
  static AIDA_PlotterLayout pr;
  if (rep == 0) {
    crisis();
    return pr;
  }
  Layout& realLayout = rep->layout();
  pr.setRep(realLayout, false);
  return pr;  
}




bool AIDA_PlotterRegion::setLayout(const AIDA::IPlotterLayout& layout)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  const AIDA_PlotterLayout* pr = dynamic_cast<const AIDA_PlotterLayout*>(&layout);
  if (pr == 0) return false; //ND plugnplay message?
  AIDA_PlotterLayout* ncpr = const_cast<AIDA_PlotterLayout*>(pr);
  Layout* gl = ncpr->theRep();
  if (gl == 0) return false;
  else {
    return rep->setLayout(*gl);
  }
}




AIDA::IInfo& AIDA_PlotterRegion::info()
{
  static AIDA_Info pr;
  if (rep == 0) {
    crisis();
    return pr;
  }
  LegendBox& realInfo = rep->legendBox();
  pr.setRep(realInfo, false);
  return pr;  
}





PlotterRegion* AIDA_PlotterRegion::theRep()
{
  return rep;
}



void AIDA_PlotterRegion::setRep(PlotterRegion& newRep, bool own)
{
  if (ownRep) delete rep;
  rep = &newRep;
  ownRep = own;
}





/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

bool AIDA_PlotterRegion::plot(const AIDA::IBaseHistogram& histogram, 
			      const AIDA::IPlotterStyle* style, 
			      const std::string& options)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  // convert "AIDA::IPlotterStyle* style" to
  //         "AidaNative::Style* realStyle"   using lots of horrible casts
  AIDA_PlotterStyle* ncpr = 0;
  if (style != 0) {
    const AIDA_PlotterStyle* pr = dynamic_cast<const AIDA_PlotterStyle*>(style);
    if (pr != 0) ncpr = const_cast<AIDA_PlotterStyle*>(ncpr);
  }
  Style* realStyle = (ncpr == 0 ? 0 : ncpr->theRep());
  const AIDA::IBaseHistogram* bh = &histogram;
  // dynamic cast in turn to all the AIDA leaf types which GracePlotter is able
  // to handle, When I find the correct one, plot it with or without the style.
  const AIDA::IHistogram1D* h1 = dynamic_cast<const AIDA::IHistogram1D*>(bh);
  if (h1 != 0) {
    if (realStyle == 0) return rep->plot(*h1, options);
    else                return rep->plot(*h1, *realStyle, options);
  }
  const AIDA::IHistogram2D* h2 = dynamic_cast<const AIDA::IHistogram2D*>(bh);
  if (h2 != 0) {
    if (realStyle == 0) return rep->plot(*h2, options);
    else                return rep->plot(*h2, *realStyle, options);
  }
  const AIDA::ICloud2D* c2 = dynamic_cast<const AIDA::ICloud2D*>(bh);
  if (c2 != 0) {
    if (realStyle == 0) return rep->plot(*c2, options);
    else                return rep->plot(*c2, *realStyle, options);
  }
  const AIDA::IProfile1D*   pr = dynamic_cast<const AIDA::IProfile1D*>(bh);
  if (pr != 0) {
    if (realStyle == 0) return rep->plot(*pr, options);
    else                return rep->plot(*pr, *realStyle, options);
  }
  // OK, I couldn't find a reasonable leaf type. bail out.
  return false;
}




bool AIDA_PlotterRegion::plot(const AIDA::IFunction& function, 
			      const AIDA::IPlotterStyle* style, 
			      const std::string& options)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  // convert "AIDA::IPlotterStyle* style" to
  //         "AidaNative::Style* realStyle"   using lots of horrible casts
  AIDA_PlotterStyle* ncpr = 0;
  if (style != 0) {
    const AIDA_PlotterStyle* pr = dynamic_cast<const AIDA_PlotterStyle*>(style);
    if (pr != 0) ncpr = const_cast<AIDA_PlotterStyle*>(ncpr);
  }
  Style* realStyle = (ncpr == 0 ? 0 : ncpr->theRep());
  // OK now plot it 
  if (realStyle == 0) return rep->plot(function, options);
  else                return rep->plot(function, *realStyle, options);
}





bool AIDA_PlotterRegion::plot(const AIDA::IDataPointSet& dataPointSet, 
			      const AIDA::IPlotterStyle* style, 
			      const std::string& options)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  // convert "AIDA::IPlotterStyle* style" to
  //         "AidaNative::Style* realStyle"   using lots of horrible casts
  AIDA_PlotterStyle* ncpr = 0;
  if (style != 0) {
    const AIDA_PlotterStyle* pr = dynamic_cast<const AIDA_PlotterStyle*>(style);
    if (pr != 0) ncpr = const_cast<AIDA_PlotterStyle*>(ncpr);
  }
  Style* realStyle = (ncpr == 0 ? 0 : ncpr->theRep());
  // OK now plot it 
  if (realStyle == 0) return rep->plot(dataPointSet, options);
  else                return rep->plot(dataPointSet, *realStyle, options);
}






//
// private methods
//

void AIDA_PlotterRegion::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_PlotterRegion" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

