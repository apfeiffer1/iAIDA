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
 * \file Implementation of class AIDA_TitleStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 20:43:17 2002
// 



#include "AIDA_TitleStyle.h"
#include "AIDA_TextStyle.h"
#include "GracePlotter/TitleStyle.h"

#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_TitleStyle::AIDA_TitleStyle()
  : rep   (0),
    ownRep(false)
{
  // no-op
}




AIDA_TitleStyle::AIDA_TitleStyle(TitleStyle* x, bool b)
  : rep   (x),
    ownRep(b)
{
  // no-op
}




AIDA_TitleStyle::~AIDA_TitleStyle()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_TitleStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_TitleStyle::setParameter(const std::string& paramName, 
				     const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::vector<std::string> AIDA_TitleStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> AIDA_TitleStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




std::string AIDA_TitleStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




AIDA::ITextStyle& AIDA_TitleStyle::textStyle()
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




bool AIDA_TitleStyle::setTextStyle(const AIDA::ITextStyle& textStyle)
{
  if (!rep) { 
    crisis();
    return false;
  }
  const AIDA_TextStyle* pr = dynamic_cast<const AIDA_TextStyle*>(&textStyle);
  if (!pr) return false;
  AIDA_TextStyle* ncpr = const_cast<AIDA_TextStyle*>(pr);
  TextStyle* tsrep = ncpr->theRep();
  if (tsrep == 0) return false;
  else return rep->setTextStyle(*tsrep);
}




void AIDA_TitleStyle::setRep(TitleStyle& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



TitleStyle* AIDA_TitleStyle::theRep()
{
  return rep;
}



void AIDA_TitleStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_TitleStyle" << std::endl; 
}





} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

