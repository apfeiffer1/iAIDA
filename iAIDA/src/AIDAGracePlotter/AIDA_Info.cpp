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
 * \file Implementation of class AIDA_Info
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:29:38 2002
// 



#include "AIDA_Info.h"
#include "AIDA_MarkerStyle.h"
#include "AIDA_LineStyle.h"
#include "AIDA_FillStyle.h"
#include "AIDAGracePlotter/LegendBox.h"

#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_Info::AIDA_Info()
  : rep(0),
    ownRep(false)
{
  // no-op
}




AIDA_Info::AIDA_Info(LegendBox* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}




AIDA_Info::~AIDA_Info()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_Info::clear()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->clear();
}




void AIDA_Info::addText(const std::string& text)
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->addText(text);
}




void AIDA_Info::addLegend(const AIDA::IMarkerStyle& aistyle, 
			    const std::string& description)
{
  if (!rep) { 
    crisis();
    return;
  }
  const AIDA_MarkerStyle* pr = dynamic_cast<const AIDA_MarkerStyle*>(&aistyle);
  if (pr == 0) return;
  AIDA_MarkerStyle* ncpr = const_cast<AIDA_MarkerStyle*>(pr);
  MarkerStyle* theRealStyle = ncpr->theRep();
  if (theRealStyle == 0) return;
  rep->addLegend(description,*theRealStyle);
}




void AIDA_Info::addLegend(const AIDA::ILineStyle& aistyle, 
			    const std::string& description)
{
  if (!rep) { 
    crisis();
    return;
  }
  const AIDA_LineStyle* pr = dynamic_cast<const AIDA_LineStyle*>(&aistyle);
  if (pr == 0) return;
  AIDA_LineStyle* ncpr = const_cast<AIDA_LineStyle*>(pr);
  LineStyle* theRealStyle = ncpr->theRep();
  if (theRealStyle == 0) return;
  rep->addLegend(description,*theRealStyle);
}




void AIDA_Info::addLegend(const AIDA::IFillStyle& aistyle, 
			    const std::string& description)
{
  if (!rep) { 
    crisis();
    return;
  }
  const AIDA_FillStyle* pr = dynamic_cast<const AIDA_FillStyle*>(&aistyle);
  if (pr == 0) return;
  AIDA_FillStyle* ncpr = const_cast<AIDA_FillStyle*>(pr);
  FillStyle* theRealStyle = ncpr->theRep();
  if (theRealStyle == 0) return;
  rep->addLegend(description,*theRealStyle);
}




LegendBox* AIDA_Info::theRep()
{
  return rep;
}




void AIDA_Info::setRep(LegendBox& newRep, bool ownership)
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

void AIDA_Info::crisis()
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_Info" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

