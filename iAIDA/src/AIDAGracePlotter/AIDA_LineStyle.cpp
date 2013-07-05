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
 * \file Implementation of class AIDA_LineStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:43:51 2002
// 



#include "AIDA_LineStyle.h"
#include "AIDAGracePlotter/LineStyle.h"

#include <iostream>


namespace AidaNative {
namespace AIDA_Plotter_Grace {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_LineStyle::AIDA_LineStyle()
  : rep   (0),
    ownRep(false)
{
  // no-op
}



AIDA_LineStyle::AIDA_LineStyle(LineStyle* x, bool b)
  : rep   (x),
    ownRep(b)
{
  // no-op
}




AIDA_LineStyle::~AIDA_LineStyle()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_LineStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_LineStyle::setParameter(const std::string& paramName, 
				    const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::vector<std::string> AIDA_LineStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::string AIDA_LineStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> 
AIDA_LineStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




std::vector<std::string> AIDA_LineStyle::availableColors() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableColors();
}




std::string AIDA_LineStyle::color() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->color();
}




double AIDA_LineStyle::opacity() const
{
  if (!rep) { 
    crisis();
    return -1.0;
  }
  return rep->opacity();
}




bool AIDA_LineStyle::setColor(const std::string & newColor)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setColor(newColor);
}




bool AIDA_LineStyle::setOpacity(double newOpacity)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setOpacity(newOpacity);
}




std::vector<std::string> AIDA_LineStyle::availableLineTypes() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableLineTypes();
}




std::string AIDA_LineStyle::lineType() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->lineType();
}




int AIDA_LineStyle::thickness() const
{
  if (!rep) { 
    crisis();
    return -1;
  }
  return rep->thickness();
}




bool AIDA_LineStyle::setLineType(const std::string& newLineType)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setLineType(newLineType);
}




bool AIDA_LineStyle::setThickness(int newThickness)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setThickness(newThickness);
}




void AIDA_LineStyle::setRep(LineStyle& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



LineStyle* AIDA_LineStyle::theRep()
{
  return rep;
}




//
// private methods
//

void AIDA_LineStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_LineStyle" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

