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
 * \file Implementation of class IAxisStyle_Proxy
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:45:30 2002
// 



#include "AIDA_AxisStyle.h"
#include "AIDA_TextStyle.h"
#include "AIDA_LineStyle.h"
#include "AIDAGracePlotter/AxisStyle.h"



#include <iostream>

namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_AxisStyle::AIDA_AxisStyle()
  : rep(0),
    ownRep(false)
{
  // no-op
}




AIDA_AxisStyle::AIDA_AxisStyle(AxisStyle* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}



AIDA_AxisStyle::~AIDA_AxisStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_AxisStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_AxisStyle::setParameter(const std::string& paramName, const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::string AIDA_AxisStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> AIDA_AxisStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> 
AIDA_AxisStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




AIDA::ILineStyle& AIDA_AxisStyle::lineStyle()
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




AIDA::ITextStyle& AIDA_AxisStyle::tickLabelStyle()
{
  static AIDA_TextStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  TextStyle& gts = rep->tickLabelStyle();
  pr.setRep(gts,false);
  return pr;
}




AIDA::ITextStyle& AIDA_AxisStyle::labelStyle()
{
  static AIDA_TextStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  TextStyle& gts = rep->labelStyle();
  pr.setRep(gts,false);
  return pr;
}




bool AIDA_AxisStyle::setlineStyle(const AIDA::ILineStyle& lineStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_LineStyle* pr = dynamic_cast<const AIDA_LineStyle*>(&lineStyle);
  if (!pr) return false; //ND no plugnplay message?
  AIDA_LineStyle* ncpr = const_cast<AIDA_LineStyle*>(pr);
  LineStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setLineStyle(*tsrep);
    return true;
  }
}




bool AIDA_AxisStyle::setTickLabelStyle(const AIDA::ITextStyle& tickLabelStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_TextStyle* pr = dynamic_cast<const AIDA_TextStyle*>(&tickLabelStyle);
  if (!pr) return false;
  AIDA_TextStyle* ncpr = const_cast<AIDA_TextStyle*>(pr);
  TextStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setTickLabelStyle(*tsrep);
    return true;
  }
}




bool AIDA_AxisStyle::setLabelStyle(const AIDA::ITextStyle& labelStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_TextStyle* pr = dynamic_cast<const AIDA_TextStyle*>(&labelStyle);
  if (!pr) return false;
  AIDA_TextStyle* ncpr = const_cast<AIDA_TextStyle*>(pr);
  TextStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setLabelStyle(*tsrep);
    return true;
  }
}




void AIDA_AxisStyle::setLabel(const std::string& label)
{
  if (!rep) { 
    crisis();
    return;
  }
  return rep->setLabel(label);
}




AxisStyle* AIDA_AxisStyle::theRep()
{
  return rep;
}




void AIDA_AxisStyle::setRep(AxisStyle& newRep, bool ownership)
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
void AIDA_AxisStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace ERROR: proxy failure in AIDA_AxisStyle" << std::endl;
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

