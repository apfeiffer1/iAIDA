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
 * \file Header file for class AIDA_MarkerStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:41:58 2002
// 

#ifndef IAIDA_AIDA_MARKERSTYLE_H
#define IAIDA_AIDA_MARKERSTYLE_H


#include "AIDA/IMarkerStyle.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class MarkerStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {



/** 
 * Implementation of AIDA::IMarkerStyle in based on GracePlotter
 */

class AIDA_MarkerStyle : public AIDA::IMarkerStyle
{
public:
  AIDA_MarkerStyle();
  AIDA_MarkerStyle(MarkerStyle*, bool owner = true);
  virtual ~AIDA_MarkerStyle();

private:
  AIDA_MarkerStyle(const AIDA_MarkerStyle&);
  AIDA_MarkerStyle& operator=(const AIDA_MarkerStyle&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::vector<std::string> availableParameters() const;
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string& paramName) const;
  virtual std::vector<std::string>  availableColors() const;
  virtual std::string color() const;
  virtual double opacity() const;
  virtual bool setColor(const std::string & newColor);
  virtual bool setOpacity(double newOpacity);
  virtual std::vector<std::string> availableShapes() const;
  virtual std::string shape() const;
  virtual bool setShape(const std::string& shape);

  void setRep(MarkerStyle&, bool ownership);
  MarkerStyle* theRep();

private:
  void crisis() const;

private:
  MarkerStyle* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_MARKERSTYLE_H

