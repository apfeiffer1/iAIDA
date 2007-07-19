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
 * \file Implementation of class AIDA_DataStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:49:35 2002
// 



#include "AIDA_DataStyle.h"
#include "AIDA_LineStyle.h"
#include "AIDA_FillStyle.h"
#include "AIDA_MarkerStyle.h"
#include "GracePlotter/PlotStyle.h"
#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_DataStyle::AIDA_DataStyle()
  : rep(0),
    ownRep(false)
{
  // no-op
}



AIDA_DataStyle::AIDA_DataStyle(PlotStyle* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}




AIDA_DataStyle::~AIDA_DataStyle()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_DataStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_DataStyle::setParameter(const std::string& paramName, 
				    const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::string AIDA_DataStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> AIDA_DataStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> AIDA_DataStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




AIDA::ILineStyle & AIDA_DataStyle::lineStyle()
{
  static AIDA_LineStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  LineStyle& gls = rep->lineStyle();
  pr.setRep(gls,false);
  return pr;
}




AIDA::IMarkerStyle & AIDA_DataStyle::markerStyle()
{
  static AIDA_MarkerStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  MarkerStyle& gms = rep->markerStyle();
  pr.setRep(gms,false);
  return pr;
}




AIDA::IFillStyle & AIDA_DataStyle::fillStyle()
{
  static AIDA_FillStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  FillStyle& gfs = rep->fillStyle();
  pr.setRep(gfs,false);
  return pr; 
}




bool AIDA_DataStyle::setLineStyle(const AIDA::ILineStyle & lineStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_LineStyle* pr = dynamic_cast<const AIDA_LineStyle*>(&lineStyle);
  if (!pr) return false;
  AIDA_LineStyle* ncpr = const_cast<AIDA_LineStyle*>(pr);
  LineStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setLineStyle(*tsrep);
    return true;
  }
}




bool AIDA_DataStyle::setMarkerStyle(const AIDA::IMarkerStyle & markerStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_MarkerStyle* pr = dynamic_cast<const AIDA_MarkerStyle*>(&markerStyle);
  if (!pr) return false;
  AIDA_MarkerStyle* ncpr = const_cast<AIDA_MarkerStyle*>(pr);
  MarkerStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setMarkerStyle(*tsrep);
    return true;
  }
}




bool AIDA_DataStyle::setFillStyle(const AIDA::IFillStyle & fillStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_FillStyle* pr = dynamic_cast<const AIDA_FillStyle*>(&fillStyle);
  if (!pr) return false;
  AIDA_FillStyle* ncpr = const_cast<AIDA_FillStyle*>(pr);
  FillStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setFillStyle(*tsrep);
    return true;
  }
}




PlotStyle* AIDA_DataStyle::theRep()
{
  return rep;
}



void AIDA_DataStyle::setRep(PlotStyle& newRep, bool ownership)
{
  if (ownRep) delete rep;
  rep = &newRep;
  ownRep = ownership;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void AIDA_DataStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace ERROR: proxy failure in AIDA_DataStyle" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

