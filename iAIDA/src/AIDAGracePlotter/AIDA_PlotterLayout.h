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
 * \file Header file for class AIDA_PlotterLayout
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:27:52 2002
// 

#ifndef AIDANATIVE_AIDA_PLOTTERLAYOUT_H
#define AIDANATIVE_AIDA_PLOTTERLAYOUT_H



#include "AIDA/IPlotterLayout.h"



namespace AidaNative {
  // Concrete classes from GracePlotter
  class Layout;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {


/**
 * Implementation of AIDA::IPlotterLayout based on GracePlotter
 */

class AIDA_PlotterLayout : public AIDA::IPlotterLayout
{
public:
  AIDA_PlotterLayout();
  AIDA_PlotterLayout(Layout*, bool ownership = true);
  virtual ~AIDA_PlotterLayout();

private:
  AIDA_PlotterLayout(const AIDA_PlotterLayout&);
  AIDA_PlotterLayout& operator=(const AIDA_PlotterLayout&);

public:
  virtual void reset();
  virtual bool setParameter(const std::string& paramName, double paramValue);
  virtual double parameterValue(const std::string& paramName);
  virtual std::vector<std::string> availableParameters() const;

  void setRep(Layout&, bool ownership);
  Layout* theRep();

private:
  void crisis() const;

private:
  Layout* rep;
  bool ownRep;

};




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // AIDANATIVE_AIDA_PLOTTERLAYOUT_H

