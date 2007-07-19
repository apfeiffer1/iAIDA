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
 * \file Header file for class AIDA_FillStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:40:06 2002
// 

#ifndef AIDANATIVE_AIDA_FILLSTYLE_H
#define AIDANATIVE_AIDA_FILLSTYLE_H



#include "AIDA/IFillStyle.h"



namespace AidaNative {
  // Concrete classes from GracePlotter
  class FillStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Proxy for AIDA::IFillStyle based on GracePlotter
 */

class AIDA_FillStyle : public AIDA::IFillStyle
{
public:
  AIDA_FillStyle();
  AIDA_FillStyle(FillStyle*, bool owner = true);
  virtual ~AIDA_FillStyle();

private:
  AIDA_FillStyle(const AIDA_FillStyle&);
  AIDA_FillStyle& operator=(const AIDA_FillStyle&);

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
  virtual std::vector<std::string> availablePatterns() const;
  virtual std::string pattern() const;
  virtual bool setPattern(const std::string& pattern);

  void setRep(FillStyle&, bool ownership);
  FillStyle* theRep();

private:
  void crisis() const; 

private:
  FillStyle* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // AIDANATIVE_AIDA_FILLSTYLE_H

