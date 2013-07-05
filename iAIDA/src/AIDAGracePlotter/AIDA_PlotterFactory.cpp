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
 * \file Implementation of class AIDA_PlotterFactory
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:25:21 2002
// 



#include "AIDA_PlotterFactory.h"

// proxies
#include "AIDA_Plotter.h"
#include "AIDA_AxisStyle.h"
#include "AIDA_PlotterStyle.h"
#include "AIDA_LineStyle.h"
#include "AIDA_FillStyle.h"
#include "AIDA_MarkerStyle.h"
#include "AIDA_TextStyle.h"
#include "AIDA_DataStyle.h"
#include "AIDA_TitleStyle.h"

// concrete implementations
#include "AIDAGracePlotter/Plotter.h"
#include "AIDAGracePlotter/AxisStyle.h"
#include "AIDAGracePlotter/Style.h"
#include "AIDAGracePlotter/LineStyle.h"
#include "AIDAGracePlotter/FillStyle.h"
#include "AIDAGracePlotter/MarkerStyle.h"
#include "AIDAGracePlotter/TextStyle.h"
#include "AIDAGracePlotter/PlotStyle.h"
#include "AIDAGracePlotter/TitleStyle.h"




namespace AidaNative {
namespace AIDA_Plotter_Grace {





////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_PlotterFactory::AIDA_PlotterFactory()
{
  // no-op
}




AIDA_PlotterFactory::~AIDA_PlotterFactory()
{
  // no-op
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

AIDA::IPlotter*
AIDA_PlotterFactory::create(const std::string& name)
{
  Plotter* gp = new Plotter;
  return new AIDA_Plotter(gp); // keeps ownership by default
}




AIDA::IMarkerStyle*
AIDA_PlotterFactory::createMarkerStyle()
{
  MarkerStyle* gms = new MarkerStyle;
  return new AIDA_MarkerStyle(gms); // keeps ownership by default
}




AIDA::ITextStyle*
AIDA_PlotterFactory::createTextStyle()
{
  TextStyle* gts = new TextStyle;
  return new AIDA_TextStyle(gts); // keeps ownership by default
}




AIDA::ILineStyle*
AIDA_PlotterFactory::createLineStyle()
{
  LineStyle* gls = new LineStyle;
  return new AIDA_LineStyle(gls); // keeps ownership by default
}




AIDA::IFillStyle*
AIDA_PlotterFactory::createFillStyle()
{
  FillStyle* gfs = new FillStyle;
  return new AIDA_FillStyle(gfs); // keeps ownership by default
}




AIDA::IDataStyle*
AIDA_PlotterFactory::createDataStyle()
{
  PlotStyle* gds = new PlotStyle;
  return new AIDA_DataStyle(gds); // keeps ownership by default
}




AIDA::IAxisStyle*
AIDA_PlotterFactory::createAxisStyle()
{
  AxisStyle* gas = new AxisStyle;
  return new AIDA_AxisStyle(gas); // keeps ownership by default
}




AIDA::IPlotterStyle*
AIDA_PlotterFactory::createPlotterStyle()
{
  Style* gps = new Style;
  return new AIDA_PlotterStyle(gps); // keeps ownership by default
}



AIDA::ITitleStyle*
AIDA_PlotterFactory::createTitleStyle()
{
  TitleStyle* gts = new TitleStyle;
  return new AIDA_TitleStyle(gts); // keeps ownership by default
}




} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

