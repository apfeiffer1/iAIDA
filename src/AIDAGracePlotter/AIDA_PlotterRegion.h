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
 * \file Header file for class AIDA_PlotterRegion
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:22:41 2002
// 

#ifndef IAIDA_AIDA_PLOTTERREGION_H
#define IAIDA_AIDA_PLOTTERREGION_H


#include "AIDA/IPlotterRegion.h"


namespace AidaNative {
  // Concrete classes from GracePlotter
  class PlotterRegion;
}


namespace AidaNative {
namespace AIDA_Plotter_Grace {



/**
 * Implementation of AIDA::IPlotterRegion based on GracePlotter
 */

class AIDA_PlotterRegion : public AIDA::IPlotterRegion
{
public:
  AIDA_PlotterRegion();
  AIDA_PlotterRegion(AidaNative::PlotterRegion*, bool owner = true);
  virtual ~AIDA_PlotterRegion();

private:
  AIDA_PlotterRegion(const AIDA_PlotterRegion&);
  AIDA_PlotterRegion& operator=(const AIDA_PlotterRegion&);

public:
  virtual bool plot(const AIDA::IBaseHistogram& histogram, const std::string& options);
  virtual bool plot(const AIDA::IBaseHistogram& histogram, const AIDA::IPlotterStyle& style, const std::string& options);
  virtual bool plot(const AIDA::IFunction& function, const std::string& options);
  virtual bool plot(const AIDA::IFunction& function, const AIDA::IPlotterStyle& style, 
		    const std::string& options);
  virtual bool plot(const AIDA::IDataPointSet& dataPointSet, const std::string& options);
  virtual bool plot(const AIDA::IDataPointSet& dataPointSet, const AIDA::IPlotterStyle& style,
		    const std::string& options);
  virtual bool remove(const AIDA::IBaseHistogram& histogram);
  virtual bool remove(const AIDA::IFunction& function);
  virtual bool remove(const AIDA::IDataPointSet& dataPointSet);
  virtual void clear();
  virtual bool setParameter(const std::string& parameter, const std::string& options);
  virtual std::vector<std::string> availableParameterOptions(const std::string& parameter) const;
  virtual std::string parameterValue(const std::string& param) const;
  virtual std::vector<std::string> availableParameters() const;
  virtual AIDA::IPlotterStyle& style();
  virtual bool setStyle(const AIDA::IPlotterStyle& style);
  virtual bool applyStyle(const AIDA::IPlotterStyle& style);
  virtual void setTitle(const std::string& title);
  virtual bool setXLimits(double min, double max);
  virtual bool setYLimits(double min, double max);
  virtual bool setZLimits(double min, double max);
  virtual AIDA::IPlotterLayout& layout();
  virtual bool setLayout(const AIDA::IPlotterLayout& layout);
  virtual AIDA::IInfo& info();
  
  PlotterRegion* theRep();
  void setRep(PlotterRegion&, bool owner);
  
private:
  void crisis() const;
  bool plot(const AIDA::IBaseHistogram& histogram, 
	    const AIDA::IPlotterStyle* style, const std::string& options);
  bool plot(const AIDA::IFunction& function, 
	    const AIDA::IPlotterStyle* style, const std::string& options);
  bool plot(const AIDA::IDataPointSet& dataPointSet, 
	    const AIDA::IPlotterStyle* style, const std::string& options);

private:
  PlotterRegion* rep;
  bool ownRep;

};



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_PLOTTERREGION_H

