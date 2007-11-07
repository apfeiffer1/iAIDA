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
 * \file Implementation of class AIDA_InfoStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:47:12 2002
// 



#include "AIDA_InfoStyle.h"
#include "AIDA_TextStyle.h"
#include "GracePlotter/LegendBoxStyle.h"

#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_InfoStyle::AIDA_InfoStyle()
  : rep   (0),
    ownRep(false)
{
  // no-op
}



AIDA_InfoStyle::AIDA_InfoStyle(LegendBoxStyle* x, bool b)
  : rep   (x),
    ownRep(b)
{
  // no-op
}



AIDA_InfoStyle::~AIDA_InfoStyle()
{
  if (ownRep) delete rep;
}





////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_InfoStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_InfoStyle::setParameter(const std::string& paramName, 
				    const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::string AIDA_InfoStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> AIDA_InfoStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> 
AIDA_InfoStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




AIDA::ITextStyle& AIDA_InfoStyle::textStyle()
{
  static AIDA_TextStyle pr;
  if (!rep) { 
    crisis();
    return pr;
  }
  TextStyle& gts = rep->textStyle();
  pr.setRep(gts,false);
  return pr;
}




bool AIDA_InfoStyle::setTextStyle(const AIDA::ITextStyle& textStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_TextStyle* pr = dynamic_cast<const AIDA_TextStyle*>(&textStyle);
  if (!pr) return false;
  AIDA_TextStyle* ncpr = const_cast<AIDA_TextStyle*>(pr);
  const TextStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else {
    rep->setTextStyle(*tsrep);
    return true;
  }
}




void AIDA_InfoStyle::setRep(LegendBoxStyle& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



LegendBoxStyle* AIDA_InfoStyle::theRep()
{
  return rep;
}



/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void AIDA_InfoStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_InfoStyle" << std::endl; 
}




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

