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
 * \file Implementation of class TextStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:38:24 2002
// 


#include <assert.h>



#include "TextStyle.h"


namespace AidaNative {




////////////////////
//                //
// Static methods //
//                //
////////////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
TextStyle::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  // (Example) Parameter1: 
  //
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "on");
  //
  // Parameter 2: Something else...
  // ...
  OptionList opts;
  //opts.push_back("on");
  //opts.push_back("off");
  //t.addParameter("shadow", opts, "off");
  return t;
}




// Available fonts - if you change these update the tests :-)
ParameterTable
TextStyle::newDefaultFontTable(const std::string& nameOfDefaultTable)
{
  typedef std::vector<std::string> OptionList;
  ParameterTable t;
  OptionList availableFonts;
  availableFonts.push_back("roman");
  availableFonts.push_back("courier");
  availableFonts.push_back("helvetica");
  availableFonts.push_back("symbol");
  t.addParameter("font", availableFonts, "roman");
  return t;
}




int TextStyle::newDefaultFontSize(const std::string& nameOfDefaultTable)
{
  int val = 12;
  if      (nameOfDefaultTable == "globalTitle") val = 16;
  else if (nameOfDefaultTable == "axisValue")   val = 10;
  return val;
}




bool TextStyle::newDefaultBold(const std::string& nameOfDefaultTable)
{
  bool val = false;
  if (nameOfDefaultTable == "globalTitle") val = true;
  return val;
}




bool TextStyle::newDefaultItalic(const std::string& nameOfDefaultTable)
{
  bool val = false;
  return val;
}




bool TextStyle::newDefaultUnderlined(const std::string& nameOfDefaultTable)
{
  bool val = false;
  if (nameOfDefaultTable == "globalTitle") val = true;
  return val;
}




const int TextStyle::MAX_FONT_SIZE = 20;




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

TextStyle::TextStyle(const std::string& nameOfDefaultTable)
  : m_defaultFontTable  (newDefaultFontTable (nameOfDefaultTable)),
    m_defaultFontSize   (newDefaultFontSize  (nameOfDefaultTable)),
    m_defaultBold       (newDefaultBold      (nameOfDefaultTable)),
    m_defaultItalic     (newDefaultItalic    (nameOfDefaultTable)),
    m_defaultUnderlined (newDefaultUnderlined(nameOfDefaultTable)),
    parameterHandler    (ParameterHandler(newDefaultPTable(nameOfDefaultTable))),
    brushStyle          (BrushStyle          (nameOfDefaultTable)),
    m_font              (defaultFont()),
    m_fontSize          (defaultFontSize()),
    m_isBold            (defaultBold()),
    m_isItalic          (defaultItalic()),
    m_isUnderlined      (defaultUnderlined()),
    fontHasBeenSet      (false),
    fontSizeHasBeenSet  (false),
    boldHasBeenSet      (false),
    italicHasBeenSet    (false),
    underlinedHasBeenSet(false)
{
  // no-op
}




TextStyle::~TextStyle()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void TextStyle::reset()
{
  m_font               = defaultFont();
  m_fontSize           = defaultFontSize();
  m_isBold             = defaultBold();
  m_isItalic           = defaultItalic();
  m_isUnderlined       = defaultUnderlined();
  fontHasBeenSet       = false;
  fontSizeHasBeenSet   = false;
  boldHasBeenSet       = false;
  italicHasBeenSet     = false;
  underlinedHasBeenSet = false;
  parameterHandler.reset();
  brushStyle.reset();
}




bool TextStyle::setParameter(const std::string& par, const std::string& opt)
{
  const bool OK = parameterHandler.setParameter(par, opt);
  if (OK) return true;
  else return brushStyle.setParameter(par, opt);
}




std::vector<std::string> TextStyle::availableParameters() const
{
  // get parameters from BrushStyle and append our own
  std::vector<std::string> vec = brushStyle.availableParameters();
  const std::vector<std::string> tmpVec = parameterHandler.availableParameters();
  vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
  return vec;
}




std::string TextStyle::parameterValue(const std::string& parameterName) const
{
  std::string s;
  const bool OK = parameterHandler.getParameterValue(parameterName, s);
  if (OK) return s;
  else return brushStyle.parameterValue(parameterName);
}




std::vector<std::string> TextStyle::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  const bool OK = parameterHandler.getAvailableOptions(param, vec);
  if (!OK) vec = brushStyle.availableOptions(param);
  return vec; 
}




std::vector<std::string> TextStyle::availableFonts() const
{
  std::vector<std::string> vec;
  const bool OK = defaultFontTable().availableOptions("font", vec);
  assert (OK);
  return vec;
}




std::vector<std::string> TextStyle::availableColors() const
{
  return brushStyle.availableColors();
}




std::string TextStyle::color() const
{
  return brushStyle.color();
}




double TextStyle::opacity() const
{
  return brushStyle.opacity();
}




bool TextStyle::setColor(const std::string& s)
{
  return brushStyle.setColor(s);
}




bool TextStyle::setOpacity(double d)
{
  return brushStyle.setOpacity(d);
}




int TextStyle::fontSize() const
{
  return m_fontSize;
}




bool TextStyle::setFontSize(int newSize)
{
  if (newSize <= 0 || newSize > MAX_FONT_SIZE) return false;
  else {
    m_fontSize = newSize;
    fontSizeHasBeenSet = true;
    return true;
  }
}




std::string TextStyle::font() const
{
  return m_font;
}




bool TextStyle::isBold() const
{
  return m_isBold;
}




bool TextStyle::isItalic() const
{
  return m_isItalic;
}




bool TextStyle::isUnderlined() const
{
  return m_isUnderlined;
}




bool TextStyle::setFont(const std::string& newFont)
{
  if (!fontIsOK(newFont)) return false;
  else {
    m_font = newFont;
    fontHasBeenSet = true;
    return true;
  }    
}




bool TextStyle::setBold(bool b)
{
  m_isBold = b;
  boldHasBeenSet = true;
  return true;
}




bool TextStyle::setItalic(bool b)
{
  m_isItalic = b;
  italicHasBeenSet = true;
  return true;
}




bool TextStyle::setUnderlined(bool b)
{
  m_isUnderlined = b;
  underlinedHasBeenSet = true;
  return true;
}




/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

const ParameterTable& TextStyle::defaultFontTable() const
{
  return m_defaultFontTable;
}




const int TextStyle::defaultFontSize() const
{
  return m_defaultFontSize;
}




const bool TextStyle::defaultBold() const
{
  return m_defaultBold;
}




const bool TextStyle::defaultItalic() const
{
  return m_defaultItalic;
}




const bool TextStyle::defaultUnderlined() const
{
  return m_defaultUnderlined;
}




bool TextStyle::fontIsOK(const std::string& shapeName) const
{
  std::vector<std::string> vec;
  const bool OK = defaultFontTable().availableOptions("font", vec);
  assert (OK);
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (shapeName == vec[i]) return true;
  }
  return false;
}




const std::string TextStyle::defaultFont() const
{
  std::string result;
  const bool OK = defaultFontTable().getDefaultValue("font", result);
  assert (OK);
  return result;  
}




} // end of namespace AidaNative

