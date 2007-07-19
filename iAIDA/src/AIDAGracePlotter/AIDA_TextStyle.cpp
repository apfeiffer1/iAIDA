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
 * \file Implementation of class AIDA_TextStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:38:26 2002
// 



#include "AIDA_TextStyle.h"
#include "GracePlotter/TextStyle.h"
#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_TextStyle::AIDA_TextStyle()
  : rep   (0),
    ownRep(false)
{
  // no-op
}




AIDA_TextStyle::AIDA_TextStyle(TextStyle* x, bool b)
  : rep   (x),
    ownRep(b)
{
  // no-op
}




AIDA_TextStyle::~AIDA_TextStyle()
{
  if (ownRep) delete rep;
}





////////////////////
//                //
// Public methods //
//                //
////////////////////

void AIDA_TextStyle::reset()
{
  if (!rep) { 
    crisis();
    return;
  }
  rep->reset();
}




bool AIDA_TextStyle::setParameter(const std::string& paramName, 
				    const std::string& options)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setParameter(paramName,options);
}




std::string AIDA_TextStyle::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> AIDA_TextStyle::availableParameters() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




std::vector<std::string> 
AIDA_TextStyle::availableParameterOptions(const std::string& paramName) const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(paramName);
}




std::vector<std::string>  AIDA_TextStyle::availableColors() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableColors();
}




std::string AIDA_TextStyle::color() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->color();
}




double AIDA_TextStyle::opacity() const
{
  if (!rep) { 
    crisis();
    return -1.0;
  }
  return rep->opacity();
}




bool AIDA_TextStyle::setColor(const std::string & newColor)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setColor(newColor);
}




bool AIDA_TextStyle::setOpacity(double newOpacity)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setOpacity(newOpacity);
}




std::vector<std::string> AIDA_TextStyle::availableFonts() const
{
  if (!rep) { 
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableFonts();
}




double AIDA_TextStyle::fontSize() const
{
  if (!rep) { 
    crisis();
    return -1.0;
  }
  return rep->fontSize();
}




bool AIDA_TextStyle::setFontSize(double size)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setFontSize( static_cast<int>( size ) );
}




std::string AIDA_TextStyle::font() const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->font();
}




bool AIDA_TextStyle::setFont(const std::string& font)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setFont(font);
}




bool AIDA_TextStyle::isBold() const
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->isBold();
}




bool AIDA_TextStyle::isItalic() const
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->isItalic();
}




bool AIDA_TextStyle::isUnderlined() const
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->isUnderlined();
}




bool AIDA_TextStyle::setBold(bool bold)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setBold(bold);
}




bool AIDA_TextStyle::setItalic(bool italic)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setItalic(italic);
}




bool AIDA_TextStyle::setUnderlined(bool underlined)
{
  if (!rep) { 
    crisis();
    return false;
  }
  return rep->setUnderlined(underlined);
}



void AIDA_TextStyle::setRep(TextStyle& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



TextStyle* AIDA_TextStyle::theRep()
{
  return rep;
}


//
// private methods
//

void AIDA_TextStyle::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_TextStyle" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

