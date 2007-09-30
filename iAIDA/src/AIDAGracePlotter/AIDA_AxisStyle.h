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
 * \file Header file for class IAxisStyle_Proxy
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:45:30 2002
// 

#ifndef IAIDA_AIDA_AXISSTYLE_H
#define IAIDA_AIDA_AXISSTYLE_H


#include "AIDA/IAxisStyle.h"



namespace AidaNative {
  // Concrete classes from GracePlotter
  class AxisStyle;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Implementation of AIDA::IAxisStyle based on GracePlotter
 */

class AIDA_AxisStyle : public AIDA::IAxisStyle
{
public:
  AIDA_AxisStyle();
  AIDA_AxisStyle(AxisStyle*, bool ownsership = true);
  virtual ~AIDA_AxisStyle();

private:
  AIDA_AxisStyle(const AIDA_AxisStyle&);
  AIDA_AxisStyle& operator=(const AIDA_AxisStyle&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::vector<std::string> availableParameters() const;
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string& paramName) const;
  virtual AIDA::ILineStyle& lineStyle();
  virtual AIDA::ITextStyle& tickLabelStyle();
  virtual AIDA::ITextStyle& labelStyle();
  virtual bool setlineStyle(const AIDA::ILineStyle& lineStyle);
  virtual bool setTickLabelStyle(const AIDA::ITextStyle& tickLabelStyle);
  virtual bool setLabelStyle(const AIDA::ITextStyle& labelStyle);
  virtual void setLabel(const std::string& label);

  AxisStyle* theRep();
  void setRep(AxisStyle&, bool ownership);

private:
  void crisis() const;

private:
  AxisStyle* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_AXISSTYLE_H

