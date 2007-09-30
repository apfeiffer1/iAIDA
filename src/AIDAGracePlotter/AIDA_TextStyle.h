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
 * \file Header file for class AIDA_TextStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:38:26 2002
// 

#ifndef IAIDA_AIDA_TEXTSTYLE_H
#define IAIDA_AIDA_TEXTSTYLE_H


#include "AIDA/ITextStyle.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class TextStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {



/**
 * Implementation of AIDA::ITextStyle in based on GracePlotter
 */

class AIDA_TextStyle : public AIDA::ITextStyle
{
public:
  AIDA_TextStyle();
  AIDA_TextStyle(TextStyle*, bool ownership = true);
  virtual ~AIDA_TextStyle();

private:
  AIDA_TextStyle(const AIDA_TextStyle&);
  AIDA_TextStyle& operator=(const AIDA_TextStyle&);

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
  virtual std::vector<std::string> availableFonts() const;
  virtual double fontSize() const;
  virtual bool setFontSize(double size);
  virtual std::string font() const;
  virtual bool setFont(const std::string& font);
  virtual bool isBold() const; virtual bool isItalic() const;
  virtual bool isUnderlined() const;
  virtual bool setBold(bool bold);
  virtual bool setItalic(bool italic);
  virtual bool setUnderlined(bool underlined);

  void setRep(TextStyle&, bool ownership);
  TextStyle* theRep();

private:
  void crisis() const;

private:
  TextStyle* rep;
  bool ownRep;

};




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_TEXTSTYLE_H

