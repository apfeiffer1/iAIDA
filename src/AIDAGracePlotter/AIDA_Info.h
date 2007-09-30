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
 * \file Header file for class AIDA_Info
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:29:38 2002
// 

#ifndef IAIDA_AIDA_INFO_H
#define IAIDA_AIDA_INFO_H


#include "AIDA/IInfo.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class LegendBox;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Implementation of AIDA::IInfo based on GracePlotter
 */

class AIDA_Info : public AIDA::IInfo
{
public:
  AIDA_Info();
  AIDA_Info(LegendBox*, bool ownership = true);
  virtual ~AIDA_Info();

private:
  AIDA_Info(const AIDA_Info&);
  AIDA_Info& operator=(const AIDA_Info&);

public:
  virtual void clear();
  virtual void addText(const std::string& text);
  virtual void addLegend(const AIDA::IMarkerStyle& style, const std::string & description);
  virtual void addLegend(const AIDA::ILineStyle& style, const std::string & description);
  virtual void addLegend(const AIDA::IFillStyle& style, const std::string & description);

  LegendBox* theRep();
  void setRep(LegendBox&, bool ownership);
 
private:
  void crisis();

private:
  LegendBox* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_INFO_H

