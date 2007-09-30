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
 * \file Header file for class AIDA_DataStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:49:35 2002
// 

#ifndef IAIDA_AIDA_DATASTYLE_H
#define IAIDA_AIDA_DATASTYLE_H


#include "AIDA/IDataStyle.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class PlotStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {



/**
 * Implementation of AIDA::IDataStyle based on GracePlotter
 */

class AIDA_DataStyle : public AIDA::IDataStyle
{
public:
  AIDA_DataStyle();
  AIDA_DataStyle(PlotStyle*, bool ownership = true);
  virtual ~AIDA_DataStyle();

private:
  AIDA_DataStyle(const AIDA_DataStyle&);
  AIDA_DataStyle& operator=(const AIDA_DataStyle&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::vector<std::string> availableParameters() const;
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string& paramName) const;
  virtual AIDA::ILineStyle & lineStyle();
  virtual AIDA::IMarkerStyle & markerStyle();
  virtual AIDA::IFillStyle & fillStyle();
  virtual bool setLineStyle(const AIDA::ILineStyle & lineStyle);
  virtual bool setMarkerStyle(const AIDA::IMarkerStyle & markerStyle);
  virtual bool setFillStyle(const AIDA::IFillStyle & fillStyle);

  PlotStyle* theRep();
  void setRep(PlotStyle&, bool owns);

private:
  void crisis() const;

private:
  PlotStyle* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_DATASTYLE_H

