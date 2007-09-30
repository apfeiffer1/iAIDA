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
 * \file Implementation of class AIDA_Plotter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Sun Oct  6 17:16:54 2002
// 



#include "AIDA_Plotter.h"
#include "GracePlotter/Plotter.h"
#include "AIDA_TitleStyle.h"
#include "AIDA_PlotterRegion.h"

#include <iostream>



namespace AidaNative {
namespace AIDA_Plotter_Grace {




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

AIDA_Plotter::AIDA_Plotter()
  : rep   (0),
    ownRep(false)
{
  // no-op
}




AIDA_Plotter::AIDA_Plotter(Plotter* p, bool own)
  : rep   (p),
    ownRep(own)
{
  // no-op
}




AIDA_Plotter::~AIDA_Plotter()
{
  if (ownRep) delete rep;
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

AIDA::IPlotterRegion* AIDA_Plotter::createRegion(double x, double y, double w, double h)
{
  if (rep == 0) {
    crisis();
    return 0;
  }
  PlotterRegion* newRegion = rep->createRegion(x, y, w, h); // keeps ownership!
  if (newRegion == 0) return 0;
  static AIDA_PlotterRegion pr;
  pr.setRep(*newRegion, false);
  return &pr;
}




bool AIDA_Plotter::createRegions(int columns, int rows, int index)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->createRegions(columns, rows, index);
}




AIDA::IPlotterRegion& AIDA_Plotter::currentRegion() const
{
  static AIDA_PlotterRegion pr;  
  if (rep == 0) {
    crisis();
    return pr;
  }
  PlotterRegion& realRegion = rep->currentRegion();
  pr.setRep(realRegion, false);
  return pr;
}




int AIDA_Plotter::currentRegionNumber() const
{  
  if (rep == 0) {
    crisis();
    return -1;
  }
  return rep->currentRegionNumber();
}




int AIDA_Plotter::numberOfRegions() const
{
  if (rep == 0) {
    crisis();
    return 0;
  }
  return rep->numberOfRegions();
}




bool AIDA_Plotter::setCurrentRegionNumber(int index)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->setCurrentRegionNumber(index);
}




AIDA::IPlotterRegion& AIDA_Plotter::next()
{
  static AIDA_PlotterRegion pr;
  if (rep == 0) {
    crisis();
    return pr;
  }
  PlotterRegion& realRegion = rep->next();
  pr.setRep(realRegion, false);
  return pr;
}




AIDA::IPlotterRegion* AIDA_Plotter::region(int index) const
{  
  if (rep == 0) {
    crisis();
    return 0;
  }
  PlotterRegion* realRegion = rep->region(index);
  if (realRegion == 0) return 0;
  static AIDA_PlotterRegion pr;
  pr.setRep(*realRegion, false);
  return &pr;
}




void AIDA_Plotter::destroyRegions()
{  
  if (rep == 0) {
    crisis();
    return;
  }
  rep->destroyAllRegions();
}




void AIDA_Plotter::clearRegions()
{  
  if (rep == 0) {
    crisis();
    return;
  }
  rep->clearAllRegions();
}




bool AIDA_Plotter::setParameter(const std::string& parameter, 
				const std::string& options)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->setParameter(parameter, options);
}




std::string AIDA_Plotter::parameterValue(const std::string& param) const
{
  if (!rep) { 
    crisis();
    return "";
  }
  return rep->parameterValue(param);
}




std::vector<std::string> 
AIDA_Plotter::availableParameterOptions(const std::string& parameter) const
{  
  if (rep == 0) {
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableOptions(parameter);
}




std::vector<std::string> AIDA_Plotter::availableParameters() const
{ 
  if (rep == 0) {
    crisis();
    return std::vector<std::string>();
  }
  return rep->availableParameters();
}




bool AIDA_Plotter::show()
{
  //ND ?
  return true;
}




bool AIDA_Plotter::refresh()
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->refresh();
}




bool AIDA_Plotter::hide()
{
  //ND ?
  return false;
}




bool AIDA_Plotter::interact()
{
  //ND ?
  return false;
}




bool AIDA_Plotter::writeToFile(const std::string& filename, 
			       const std::string& type)
{
  if (rep == 0) {
    crisis();
    return false;
  }
  return rep->writeToFile(filename, type);
}




void AIDA_Plotter::setTitle(const std::string& title)
{  
  if (rep == 0) {
    crisis();
    return;
  }
  rep->setTitle(title);
}




AIDA::ITitleStyle& AIDA_Plotter::titleStyle()
{  
  static AIDA_TitleStyle pr;
  if (rep == 0) {
    crisis();
    return pr;
  }
  TitleStyle& realTitleStyle = rep->titleStyle();
  pr.setRep(realTitleStyle, false);
  return pr;
}




void AIDA_Plotter::setTitleStyle(const AIDA::ITitleStyle& style)
{  
  if (rep == 0) {
    crisis();
    return;
  }
  const AIDA_TitleStyle* pr = dynamic_cast<const AIDA_TitleStyle*>(&style);
  if (pr == 0) return; //ND plugnplay message?
  AIDA_TitleStyle* ncpr = const_cast<AIDA_TitleStyle*>(pr);
  TitleStyle* gts = ncpr->theRep();
  if (gts == 0) return;
  rep->setTitleStyle(*gts);
}




void AIDA_Plotter::setRep(Plotter& s, bool ownership)
{
  if (ownRep) delete rep;
  rep = &s;
  ownRep = ownership;
}



Plotter* AIDA_Plotter::theRep()
{
  return rep;
}





/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void AIDA_Plotter::crisis() const
{
  std::cout << "*** AIDA_Plotter_Grace SEVERE ERROR: proxy failure in AIDA_Plotter" << std::endl; 
}



} // end of namespace AIDA_Plotter_Grace
} // end of namespace AidaNative

