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
 * \file Implementation of class AIDA_FillStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:40:06 2002
// 



#include "AIDAGracePlotter/FillStyle.h"
#include "AIDA_FillStyle.h"
#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_FillStyle::AIDA_FillStyle()
  : rep   (0),
    ownRep(false)
{
  // no-op
}




AIDA_FillStyle::AIDA_FillStyle(FillStyle* x, bool owner)
  : rep   (x),
    ownRep(owner)
{
  // no-op
}




AIDA_FillStyle::~AIDA_FillStyle()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_FillStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_FillStyle::setParameter(const std::string& paramName, 
				    const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName, options);
}




std::vector<std::string> AIDA_FillStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::string AIDA_FillStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> 
AIDA_FillStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




std::vector<std::string>  AIDA_FillStyle::availableColors() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableColors();
}




std::string AIDA_FillStyle::color() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->color();
}




double AIDA_FillStyle::opacity() const
{
  if (!rep) { 
    crisis();
    return -1.0;
  }
  return rep->opacity();
}




bool AIDA_FillStyle::setColor(const std::string & newColor)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setColor(newColor);
}




bool AIDA_FillStyle::setOpacity(double newOpacity)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setOpacity(newOpacity);
}




std::vector<std::string> AIDA_FillStyle::availablePatterns() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availablePatterns();
}




std::string AIDA_FillStyle::pattern() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->pattern();
}




bool AIDA_FillStyle::setPattern(const std::string& pattern)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setPattern(pattern);
}





void AIDA_FillStyle::setRep(FillStyle& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



FillStyle* AIDA_FillStyle::theRep()
{
  return rep;
}



//
// private methods
//

void AIDA_FillStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_FillStyle" << std::endl; 
}




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

