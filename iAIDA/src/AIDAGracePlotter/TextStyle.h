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
 * \file Header file for class TextStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Tue Aug  6 16:38:24 2002
// 

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_TEXTSTYLE_H
#define IAIDA_QPLOTTER_NEW_VIEWER_TEXTSTYLE_H



#include "BrushStyle.h"



namespace AidaNative {



/**
 * Style for all text (e.g. axis labels, legend box text etc.)
 */

class TextStyle
{
public:
  TextStyle(const std::string& nameOfDefaultTable = "");
  virtual ~TextStyle();

public:
  /// Return to original (construction time) state.
  virtual void reset();
  /// Returns false if parameter unknown or options are invalid
  virtual bool setParameter(const std::string&, const std::string& options = "");
  /// Get current 'value' (option string) of a named parameter.
  virtual std::string parameterValue(const std::string& parameterName) const;
  /// Get list of the available parameters
  virtual std::vector<std::string> availableParameters() const;
  /// Get list of the available options for a given parameter 
  virtual std::vector<std::string> availableOptions(const std::string& param) const;
  /// Get a list of the colours supported
  virtual std::vector<std::string> availableColors() const;
  /// Current colour of the text
  virtual std::string color() const;
  /// Current opacity (alpha)
  virtual double opacity() const;
  /// Set current text colour, e.g. "darkGreen"
  virtual bool setColor(const std::string&);
  /// Set opacity (alpha), e.g. 0 (transparent), 1 (opaque), 0.5 (halfway).
  virtual bool setOpacity(double);
  /// Get list of available font names
  virtual std::vector<std::string> availableFonts() const;
  /// Get current font size
  virtual int fontSize() const;
  /// Set font size
  virtual bool setFontSize(int);
  /// Get current font name
  virtual std::string font() const;
  /// Returns true if current font is bold
  virtual bool isBold() const;
  /// Returns true if current font is italic
  virtual bool isItalic() const;
  /// Returns true if current font is underlined  
  virtual bool isUnderlined() const;
  /// Set new font name
  virtual bool setFont(const std::string&);
  /// Toggle bold font (returns false if not available in the current font)
  virtual bool setBold(bool);
  /// Toggle italic font (returns false if not available in the current font)
  virtual bool setItalic(bool);
  /// Toggle underlined font (returns false if not available in the current font)
  virtual bool setUnderlined(bool);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" table of fonts according to string argument
  static ParameterTable newDefaultFontTable(const std::string& nameOfDefaultTable = "");
  /// Generates "default" font size according to input string
  static int newDefaultFontSize(const std::string& nameOfDefaultTable = "");
  /// Generates "default" bold flag according to input string
  static bool newDefaultBold(const std::string& nameOfDefaultTable = "");
  /// Generates "default" italic flag according to input string
  static bool newDefaultItalic(const std::string& nameOfDefaultTable = "");
  /// Generates "default" underlined flag according to input string
  static bool newDefaultUnderlined(const std::string& nameOfDefaultTable = "");
  /// Maximum permitted font size
  static const int MAX_FONT_SIZE;

private:
  /// Available fonts
  const ParameterTable& defaultFontTable() const;
  /// Font name is understood?
  bool fontIsOK(const std::string& fontName) const;
  /// Get name of default font 
  const std::string defaultFont() const;
  /// Getr default font size
  const int defaultFontSize() const;
  /// Get default bold flag
  const bool defaultBold() const;
  /// Get default italic flag
  const bool defaultItalic() const;
  /// Get default underlined flag
  const bool defaultUnderlined() const;

private:
  ParameterTable   m_defaultFontTable;
  int              m_defaultFontSize;
  bool             m_defaultBold;
  bool             m_defaultItalic;
  bool             m_defaultUnderlined;
  ParameterHandler parameterHandler;
  BrushStyle       brushStyle;
  std::string      m_font;
  int              m_fontSize;
  bool             m_isBold;
  bool             m_isItalic;
  bool             m_isUnderlined;
  bool             fontHasBeenSet;
  bool             fontSizeHasBeenSet;
  bool             boldHasBeenSet;
  bool             italicHasBeenSet;
  bool             underlinedHasBeenSet;

};



} // end of namespace AidaNative


#endif // IAIDA_QPLOTTER_NEW_VIEWER_TEXTSTYLE_H

