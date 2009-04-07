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
 * \file Header file for class AIDA_PlotterFactory
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:25:21 2002
// 

#ifndef IAIDA_AIDA_PLOTTERFACTORY_H
#define IAIDA_AIDA_PLOTTERFACTORY_H


#include "AIDA/IPlotterFactory.h"



namespace AidaNative {
namespace AIDA_Plotter_Grace {




/**
 * Creates plotters and styles based on GracePlotter
 */

class AIDA_PlotterFactory : public AIDA::IPlotterFactory
{
public:
  AIDA_PlotterFactory();
  virtual ~AIDA_PlotterFactory();

public:
  virtual AIDA::IPlotter* create(const std::string& name);
  virtual AIDA::IMarkerStyle* createMarkerStyle();
  virtual AIDA::ITextStyle* createTextStyle();
  virtual AIDA::ILineStyle* createLineStyle();
  virtual AIDA::IFillStyle* createFillStyle();
  virtual AIDA::IDataStyle* createDataStyle();
  virtual AIDA::IAxisStyle* createAxisStyle();
  virtual AIDA::IPlotterStyle* createPlotterStyle();
  virtual AIDA::ITitleStyle* createTitleStyle();
};




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative


#endif // IAIDA_AIDA_PLOTTERFACTORY_H

