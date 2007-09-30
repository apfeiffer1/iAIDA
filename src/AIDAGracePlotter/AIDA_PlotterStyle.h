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
 * \file Header file for class AIDA_PlotterStyle
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:48:39 2002
// 

#ifndef IAIDA_AIDA_PLOTTERSTYLE_H
#define IAIDA_AIDA_PLOTTERSTYLE_H


#include "AIDA/IPlotterStyle.h"



namespace AidaNative {
  // Concrete classes from GracePlotter
  class Style;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Implementation of AIDA::IPlotterStyle based on GracePlotter
 */

class AIDA_PlotterStyle : public AIDA::IPlotterStyle
{
public:
  AIDA_PlotterStyle();
  AIDA_PlotterStyle(Style*, bool ownership = true);
  virtual ~AIDA_PlotterStyle();

private:
  AIDA_PlotterStyle(const AIDA_PlotterStyle&);
  AIDA_PlotterStyle& operator=(const AIDA_PlotterStyle&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::vector<std::string> availableParameters() const;
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string& paramName) const;
  virtual AIDA::IDataStyle& dataStyle();
  virtual AIDA::IAxisStyle& xAxisStyle();
  virtual AIDA::IAxisStyle& yAxisStyle();
  virtual AIDA::IAxisStyle& zAxisStyle();
  virtual AIDA::ITitleStyle& titleStyle();
  virtual AIDA::IInfoStyle& infoStyle();
  virtual bool setDataStyle(const AIDA::IDataStyle& dataStyle);
  virtual bool setAxisStyleX(const AIDA::IAxisStyle& xAxisStyle);
  virtual bool setAxisStyleY(const AIDA::IAxisStyle& yAxisStyle);
  virtual bool setAxisStyleZ(const AIDA::IAxisStyle& zAxisStyle);
  virtual bool setTitleStyle(const AIDA::ITitleStyle& textStyle);
  virtual bool setInfoStyle(const AIDA::IInfoStyle& infoStyle);

  Style* theRep();
  void setRep(Style&, bool owns);

private:
  void crisis() const;

private:
  Style* rep;
  bool ownRep;

};


} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_PLOTTERSTYLE_H

