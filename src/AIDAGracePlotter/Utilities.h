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
 * \file Header file for class Utilities
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Oct  2 21:58:18 2002
// 

#ifndef IAIDA_GRACE_AIDA_PLOTTER_UTILITIES_H
#define IAIDA_GRACE_AIDA_PLOTTER_UTILITIES_H


#include "Grace.h"



namespace AidaNative {



class TextStyle;



/**
 * Collection of useful stuff for the Grace-based AIDA 3.x Plotter
 */

class Utilities
{
private:
  Utilities();
  Utilities(const Utilities&);
  virtual ~Utilities();
  Utilities& operator=(const Utilities&);

public:
  /// returns romanNormal if unknown
  static Grace::Font resolveFont(const TextStyle&);
  /// returns an approximation of the font size as a double for Grace
  static double fontSizeAsDouble(int fontSizeAsPoints);

};



} // end of namespace AidaNative


#endif // IAIDA_GRACE_AIDA_PLOTTER_UTILITIES_H

