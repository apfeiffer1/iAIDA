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
 * \file Implementation of class Utilities
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Oct  2 21:58:18 2002
// 



#include "Utilities.h"
#include "TextStyle.h"



namespace AidaNative {


////////////////////
//                //
// Public methods //
//                //
////////////////////

Grace::Font Utilities::resolveFont(const TextStyle& ts)
{
  const std::string fontName = ts.font();
  const bool bold   = ts.isBold();
  const bool italic = ts.isItalic();
  Grace::Font f = Grace::timesNormal;
  if (fontName == "times" || fontName == "roman") {
    if (!bold && !italic) f = Grace::timesNormal;
    if ( bold && !italic) f = Grace::timesBold;
    if (!bold &&  italic) f = Grace::timesItalic;
    if ( bold &&  italic) f = Grace::timesItalicBold;
  }
  else if (fontName == "helvetica") {
    if (!bold && !italic) f = Grace::helveticaNormal;
    if ( bold && !italic) f = Grace::helveticaBold;
    if (!bold &&  italic) f = Grace::helveticaItalic;
    if ( bold &&  italic) f = Grace::helveticaItalicBold;
  }
  else if (fontName == "courier") {
    if (!bold && !italic) f = Grace::courierNormal;
    if ( bold && !italic) f = Grace::courierBold;
    if (!bold &&  italic) f = Grace::courierItalic;
    if ( bold &&  italic) f = Grace::courierItalicBold;
  }
  else if (fontName == "symbol") {
    f = Grace::symbol;
  }
  else if (fontName == "zapfDingbats") {
    f = Grace::zapfDingbats;
  }
  return f;
}




double Utilities::fontSizeAsDouble(int n)
{
  const double def = 1.20;
  if (n > 1 && n < 35) { // kurwa mac, what a hack
    return double(n)/10.0;
  }
  else return def;
}



/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////



} // end of namespace AidaNative

