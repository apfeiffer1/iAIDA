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
 * \file Header file for class AIDA_TitleStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 20:43:17 2002
// 

#ifndef IAIDA_AIDA_TITLESTYLE_H
#define IAIDA_AIDA_TITLESTYLE_H


#include "AIDA/ITitleStyle.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class TitleStyle;
  class TextStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Implementation of AIDA::ITitleStyle in based on GracePlotter
 */

class AIDA_TitleStyle : public AIDA::ITitleStyle
{
public:
  AIDA_TitleStyle();
  AIDA_TitleStyle(TitleStyle*, bool ownership = true);
  virtual ~AIDA_TitleStyle();

private:
  AIDA_TitleStyle(const AIDA_TitleStyle&);
  AIDA_TitleStyle& operator=(const AIDA_TitleStyle&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameters() const;
  virtual std::vector<std::string> availableParameterOptions(const std::string& paramName) const;
  virtual AIDA::ITextStyle& textStyle();
  virtual bool setTextStyle(const AIDA::ITextStyle& textStyle);

  void setRep(TitleStyle&, bool ownership);
  TitleStyle* theRep();

private:
  void crisis() const;

private:
  TitleStyle* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_TITLESTYLE_H

