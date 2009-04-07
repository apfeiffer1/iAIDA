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
 * \file Implementation of class AIDA_PlotterStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:48:39 2002
// 



#include "AIDA_PlotterStyle.h"
#include "AIDA_AxisStyle.h"
#include "AIDA_DataStyle.h"
#include "AIDA_InfoStyle.h"
#include "AIDA_TitleStyle.h"
#include "AIDA_DataStyle.h"
#include "GracePlotter/Style.h"

#include <iostream>


namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_PlotterStyle::AIDA_PlotterStyle()
  : rep(0),
    ownRep(false)
{
  // no-op
}




AIDA_PlotterStyle::AIDA_PlotterStyle(Style* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}




AIDA_PlotterStyle::~AIDA_PlotterStyle()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_PlotterStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_PlotterStyle::setParameter(const std::string& paramName, const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::string AIDA_PlotterStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> AIDA_PlotterStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> 
AIDA_PlotterStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




AIDA::IDataStyle& AIDA_PlotterStyle::dataStyle()
{
  static AIDA_DataStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  PlotStyle& gds = rep->plotStyle();
  pr.setRep(gds,false);
  return pr;
}




AIDA::IAxisStyle& AIDA_PlotterStyle::xAxisStyle()
{
  static AIDA_AxisStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  AxisStyle& gas = rep->xAxisStyle();
  pr.setRep(gas,false);
  return pr;
}




AIDA::IAxisStyle& AIDA_PlotterStyle::yAxisStyle()
{
  static AIDA_AxisStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  AxisStyle& gas = rep->yAxisStyle();
  pr.setRep(gas,false);
  return pr;
}




AIDA::IAxisStyle& AIDA_PlotterStyle::zAxisStyle()
{
  static AIDA_AxisStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  AxisStyle& gas = rep->zAxisStyle();
  pr.setRep(gas,false);
  return pr;
}




AIDA::ITitleStyle& AIDA_PlotterStyle::titleStyle()
{
  static AIDA_TitleStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  TitleStyle& gts = rep->titleStyle();
  pr.setRep(gts,false);
  return pr;
}




AIDA::IInfoStyle& AIDA_PlotterStyle::infoStyle()
{
  static AIDA_InfoStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  LegendBoxStyle& gis = rep->legendBoxStyle();
  pr.setRep(gis,false);
  return pr;
}




bool AIDA_PlotterStyle::setDataStyle(const AIDA::IDataStyle& dataStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_DataStyle* pr = dynamic_cast<const AIDA_DataStyle*>(&dataStyle);
  if (!pr) return false;
  AIDA_DataStyle* ncpr = const_cast<AIDA_DataStyle*>(pr);
  PlotStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setPlotStyle(*tsrep);
    return true;
  }
}




bool AIDA_PlotterStyle::setAxisStyleX(const AIDA::IAxisStyle& xAxisStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_AxisStyle* pr = dynamic_cast<const AIDA_AxisStyle*>(&xAxisStyle);
  if (!pr) return false;
  AIDA_AxisStyle* ncpr = const_cast<AIDA_AxisStyle*>(pr);
  AxisStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setXAxisStyle(*tsrep);
    return true;
  }
}




bool AIDA_PlotterStyle::setAxisStyleY(const AIDA::IAxisStyle& yAxisStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_AxisStyle* pr = dynamic_cast<const AIDA_AxisStyle*>(&yAxisStyle);
  if (!pr) return false;
  AIDA_AxisStyle* ncpr = const_cast<AIDA_AxisStyle*>(pr);
  AxisStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setYAxisStyle(*tsrep);
    return true;
  }
}




bool AIDA_PlotterStyle::setAxisStyleZ(const AIDA::IAxisStyle& zAxisStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_AxisStyle* pr = dynamic_cast<const AIDA_AxisStyle*>(&zAxisStyle);
  if (!pr) return false;
  AIDA_AxisStyle* ncpr = const_cast<AIDA_AxisStyle*>(pr);
  AxisStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setZAxisStyle(*tsrep);
    return true;
  }
}




bool AIDA_PlotterStyle::setTitleStyle(const AIDA::ITitleStyle& titleStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_TitleStyle* pr = dynamic_cast<const AIDA_TitleStyle*>(&titleStyle);
  if (!pr) return false;
  AIDA_TitleStyle* ncpr = const_cast<AIDA_TitleStyle*>(pr);
  TitleStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setTitleStyle(*tsrep);
    return true;
  }
}




bool AIDA_PlotterStyle::setInfoStyle(const AIDA::IInfoStyle& infoStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_InfoStyle* pr = dynamic_cast<const AIDA_InfoStyle*>(&infoStyle);
  if (!pr) return false;
  AIDA_InfoStyle* ncpr = const_cast<AIDA_InfoStyle*>(pr);
  LegendBoxStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setLegendBoxStyle(*tsrep);
    return true;
  }
}




void AIDA_PlotterStyle::setRep(Style& newRep, bool own)
{
  if (ownRep) delete rep;
  rep = &newRep;
  ownRep = own;
}




Style* AIDA_PlotterStyle::theRep()
{
  return rep;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void AIDA_PlotterStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_PlotterStyle" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

