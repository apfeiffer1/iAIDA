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
 * \file Header file for class AIDA_Plotter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:16:54 2002
// 

#ifndef IAIDA_AIDA_PLOTTER_H
#define IAIDA_AIDA_PLOTTER_H


#include "AIDA/IPlotter.h"



namespace AidaNative {
  // Concrete classes from GracePlotter
  class Plotter;
}



namespace AidaNative {
namespace AIDA_Plotter_Grace {



/**
 * Proxy for AIDA::IPlotter based on GracePlotter::Plotter
 */

class AIDA_Plotter : public AIDA::IPlotter
{
public:
  AIDA_Plotter();
  AIDA_Plotter(AidaNative::Plotter*, bool ownership = true);
  virtual ~AIDA_Plotter();

private:
  /// not permitted
  AIDA_Plotter(const AIDA_Plotter&);
  /// not permitted
  AIDA_Plotter& operator=(const AIDA_Plotter&);

public:
  virtual AIDA::IPlotterRegion* createRegion(double x, double y, double w, double h);
  virtual bool createRegions(int columns, int rows, int index);
  virtual AIDA::IPlotterRegion& currentRegion() const;
  virtual int currentRegionNumber() const;
  virtual int numberOfRegions() const;
  virtual bool setCurrentRegionNumber(int index);
  virtual AIDA::IPlotterRegion& next();
  virtual AIDA::IPlotterRegion* region(int index) const;
  virtual void destroyRegions();
  virtual void clearRegions();
  virtual bool setParameter(const std::string& parameter, const std::string& options);
  virtual std::vector<std::string> availableParameterOptions(const std::string& parameter) const;
  virtual std::string parameterValue(const std::string& paramName) const;
  virtual std::vector<std::string> availableParameters() const;
  virtual bool show();
  virtual bool refresh();
  virtual bool hide();
  virtual bool interact();
  virtual bool writeToFile(const std::string& filename, const std::string& type);
  virtual void setTitle(const std::string& title);
  virtual AIDA::ITitleStyle& titleStyle();
  virtual void setTitleStyle(const AIDA::ITitleStyle& style);

  void setRep(AidaNative::Plotter&, bool ownership);
  AidaNative::Plotter* theRep();

private:
  void crisis() const;

private:
  AidaNative::Plotter* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_PLOTTER_H

