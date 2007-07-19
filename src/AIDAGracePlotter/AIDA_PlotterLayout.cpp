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
 * \file Implementation of class AIDA_PlotterLayout
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:27:52 2002
// 



#include "AIDA_PlotterLayout.h"
#include "GracePlotter/Layout.h"

#include <iostream>


namespace AidaNative {
namespace AIDA_Plotter_Grace {



////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_PlotterLayout::AIDA_PlotterLayout()
  : rep(0),
    ownRep(false)
{
  // no-op
}



AIDA_PlotterLayout::AIDA_PlotterLayout(Layout* x, bool b)
  : rep(x),
    ownRep(b)
{
  // no-op
}




AIDA_PlotterLayout::~AIDA_PlotterLayout()
{
  if (ownRep) delete rep;
}





////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_PlotterLayout::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_PlotterLayout::setParameter(const std::string& paramName, 
					double paramValue)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameterValue(paramName, paramValue);
}




double AIDA_PlotterLayout::parameterValue(const std::string& paramName)
{
  if (!rep) { 
    crisis();
    return -1.0;
  }
  return rep->parameterValue(paramName);
}




std::vector<std::string> AIDA_PlotterLayout::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}



Layout* AIDA_PlotterLayout::theRep()
{
  return rep;
}



void AIDA_PlotterLayout::setRep(Layout& newRep, bool ownership)
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

void AIDA_PlotterLayout::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_PlotterLayout" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

