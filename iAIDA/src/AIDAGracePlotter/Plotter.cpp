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
 * \file Implementation of class Plotter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Mon Aug 26 17:10:31 2002
// 

#ifdef __sun
# include <stdlib.h>
#endif

#include <iostream>

#include <unistd.h>
#include "Plotter.h"
#include "PlotterRegion.h"
#include "GraceAIDAConverter.h"
#include "Utilities.h"



//namespace AIDA {
class IHistogram1D;
//}



namespace AidaNative {



/////////////
//         //
// Statics //
//         //
/////////////

// Available parameters - if you change these update the tests :-)
ParameterTable
Plotter::newDefaultPTable(const std::string& nameOfDefaultTable)
{
  // (Example) Parameter1: 
  // typedef std::vector<std::string> OptionList;
  // OptionList someOptions;
  // someOptions.push_back("on");
  // someOptions.push_back("off");
  // t.addParameter("CoolParameter", someOptions, "on");
  //
  // Parameter 2: Something else...
  // ..of course behaviour can be modified according to nameOfDefT...
  ParameterTable t;
  return t;
}

   // const TextMessage 
   // Plotter::unknownParameter("parameter '$1' is not known ",
   // 			  "Plotter", TextMessage::INFO);
   // 
   // const TextMessage 
   // Plotter::badRegionParameters("cannot create region with those parameters ",
   // 			     "Plotter");
   // 
   // const TextMessage 
   // Plotter::regionOutOfRange("region number $1 is not a valid index ",
   // 			  "Plotter");
   // 
   // const TextMessage
   // Plotter::fileTypeConflict("filename '$1' and actual type '$2' may confuse! ",
   // 			  "Plotter", TextMessage::WARNING);
   // 
   // const TextMessage
   // Plotter::fileTypeUnknown("requested type '$1' not known",
   // 			 "Plotter");
   // 
   // const TextMessage
   // Plotter::singleRegionOnly("sorry, only single regions in this version ",
   // 			  "Plotter", TextMessage::INFO);


////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

Plotter::Plotter(const std::string& defaultName)
  : grace           (Grace::theInstance()),
    parameterHandler(newDefaultPTable(defaultName)),
    m_currentRegion (0)
{
  resetToSingleRegion();
  refresh();
}




Plotter::~Plotter()
{
  destroyAllRegions();
  if (grace.isRunning()) grace.stop();
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

PlotterRegion* Plotter::createRegion(double bottomLeftX,
				     double bottomLeftY,
				     double width,
				     double height)
{
//-ap    /////////////////////////////////////////////////
//-ap    //ND RESTRICTED TO A SINGLE REGION FOR NOW!
//-ap    if (numberOfRegions() < 1) {
//-ap      resetToSingleRegion();
//-ap      return region(0);
//-ap    }
//-ap    else {
//-ap      const TextMessage singleRegionOnly("sorry, only single regions in this version ",
//-ap 	  			              "Plotter", TextMessage::INFO);
//-ap      singleRegionOnly.show();
//-ap      return 0;
//-ap    }
//-ap    /////////////////////////////////////////////////

  // some sanity checks ...
  if (width < 0.0 || height < 0.0) {
    const TextMessage badRegionParameters("cannot create region with those parameters ",
					  "Plotter");
    badRegionParameters.show();
    return 0;
  }
  // smarter checks?

  // we simply create a new region according to user specs,
  // add it to the list and set it the new currentRegion
  PlotterRegion* newRegion = new PlotterRegion(theRegions.size(),
 					       this,
 					       bottomLeftX,
 					       bottomLeftY,
 					       width, height); 
   theRegions.push_back(newRegion);
   m_currentRegion = theRegions.size();
   return newRegion;
}




bool Plotter::createRegions(int columns, int rows, int index)
{
  destroyAllRegions();

  double dx = 1./double(columns);
  double dy = 1./double(rows);

  double width  = dx * 0.9;
  double height = dy * 0.9;

  double startLLX = 0.;
  double startLLY = 0.;
  int regionNr = 0;
  for (int j=0; j<rows; j++) {
    startLLY += dy;
    for (int i=0; i<columns; i++ ) {
      startLLX += dx;
//-ap       std::cerr << "creating region " << regionNr 
//-ap 		<< " at X/Y=" << startLLX << "/" << startLLY
//-ap 		<< " w/h = " << width << "/" << height
//-ap 		<< std::endl;
      theRegions.push_back(new PlotterRegion(regionNr,
					     this,
					     startLLX,
					     startLLY,
					     width, height) );
      regionNr++;
    }
    startLLX = 0.; // reset starting value for x at each new row ...
  }

  grace.arrange(rows, columns);

  if (index < 0 || index > regionNr) {
    std::cerr << "Plotter::createRegions> illegal index " << index << " for regionNr " << regionNr << std::endl;
    return false;
  } else {
    m_currentRegion = index;
    return true;
  }
}




PlotterRegion& Plotter::currentRegion() const
{
  return *region(m_currentRegion);
}




PlotterRegion& Plotter::next() const
{
  unsigned int nextRegionNumber = m_currentRegion + 1;
  if (nextRegionNumber >= theRegions.size()) nextRegionNumber = 0;
  const int rn = nextRegionNumber;
  return *region(rn);
}




PlotterRegion* Plotter::region(int regionNumber) const
{
  unsigned int rn = abs(regionNumber);
  if (rn >= theRegions.size() || regionNumber < 0) {
    const TextMessage regionOutOfRange("region number $1 is not a valid index ", "Plotter");
    regionOutOfRange.show(regionNumber);
    return 0;
  }
  else {
    return theRegions[regionNumber];
  }
}




bool Plotter::setCurrentRegionNumber(int newRegionNumber)
{
  unsigned int rn = abs(newRegionNumber);
  if (rn >= theRegions.size() || newRegionNumber < 0) {
    const TextMessage regionOutOfRange("region number $1 is not a valid index ", "Plotter");
    regionOutOfRange.show(newRegionNumber);
    return false;
  }
  else {
    m_currentRegion = newRegionNumber;
    return true;
  }
}




int Plotter::currentRegionNumber() const
{
  return m_currentRegion;
}




int Plotter::numberOfRegions() const
{
  return theRegions.size();
}




bool Plotter::destroyAllRegions()
{
  //ND Only single regions in the version!
  //-ap return clearAllRegions();
  // /////////////////////////
  for (RegionMapIt i = theRegions.begin(); i != theRegions.end(); ++i) {
    delete *i; // clears the region first ...
    *i = 0;
  }
  theRegions.clear();
  m_currentRegion = 0;
  return true;
}




bool Plotter::clearAllRegions()
{
  if (theRegions.empty()) return false;
  else {
    for (RegionMapIt i = theRegions.begin(); i != theRegions.end(); ++i) {
      (*i)->clear();
    }
    return true;
  }
}




bool Plotter::setParameter(const std::string& par,
			   const std::string& opt)
{
  return parameterHandler.setParameter(par, opt);
}




std::string Plotter::parameterValue(const std::string& param) const
{
  std::string s;
  bool OK = parameterHandler.getParameterValue(param, s);
  if (!OK) {
    const TextMessage unknownParameter("parameter '$1' is not known ",
				       "Plotter", TextMessage::INFO);
    unknownParameter.show(param);
  }
  return s;
}



std::vector<std::string> Plotter::availableParameters() const
{
  return std::vector<std::string>(parameterHandler.availableParameters());
}




std::vector<std::string> 
Plotter::availableOptions(const std::string& param) const
{
  std::vector<std::string> vec;
  parameterHandler.getAvailableOptions(param, vec);
  return vec;
}




bool Plotter::writeToFile(const std::string& filename, 
			  const std::string& fileType)
{
  FileFormat format = UNKNOWN;
  FileFormat implicitType = getFileFormatFromSuffix(filename);
  FileFormat explicitType = getFileFormatFromDescription(fileType);
  if (explicitType == UNKNOWN && implicitType == UNKNOWN) {
    const TextMessage fileTypeUnknown("requested type '$1' not known",
				      "Plotter");
    fileTypeUnknown.show();
    return false;
  }
  if (explicitType != implicitType) {
    format = (explicitType != UNKNOWN ? explicitType : implicitType);
    if (explicitType != UNKNOWN) {
      const TextMessage fileTypeConflict("filename '$1' and actual type '$2' may confuse! ",
					 "Plotter", TextMessage::WARNING);
      fileTypeConflict.show(filename, fileType);
    }
  }
  else format = explicitType;
  return uncheckedWrite(filename, format);
}




bool Plotter::refresh()
{
  applyTitleFontChanges();
  for (RegionMapIt i = theRegions.begin(); i != theRegions.end(); ++i) {
    (*i)->refresh();
  }
  return grace.redraw();
}




bool Plotter::show()
{
  return refresh();
}




bool Plotter::hide()
{
  //ND
  return false;
}




bool Plotter::interact()
{
  //ND
  return true;
}




bool Plotter::setTitle(const std::string& s)
{
  m_title = s;
  //ND convert from MATHML first?
  grace.setGlobalTitle(s);
  return true;
}



TitleStyle& Plotter::titleStyle()
{
  return m_titleStyle;
}



bool Plotter::setTitleStyle(const TitleStyle& x)
{
  m_titleStyle = x;
  return true;
}



/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////


Plotter::FileFormat 
Plotter::getFileFormatFromSuffix(const std::string& filename)
{
  FileFormat f;
  if      (filename.find(".ps")  == filename.length() - 3) f = PS;
  else if (filename.find(".eps") == filename.length() - 4) f = EPS;
  else if (filename.find(".png") == filename.length() - 4) f = PNG;
  else if (filename.find(".svg") == filename.length() - 4) f = SVG;
  else if (filename.find(".jpg") == filename.length() - 4) f = JPG;
  else f = UNKNOWN;
  return f;
}




Plotter::FileFormat 
Plotter::getFileFormatFromDescription(const std::string& desc)
{
  std::string ldesc = desc;
  for (unsigned int i = 0; i < ldesc.length(); ++i) tolower(ldesc[i]);
  FileFormat f;
  if      (ldesc == "png") 
    f = PNG;
  else if (ldesc == "ps" || ldesc == "postscript")
    f = PS;
  else if (ldesc == "eps") 
    f = EPS;
  else if (ldesc == "svg") 
    f = SVG;
  else if (ldesc == "jpg") 
    f = JPG;
  else 
    f = UNKNOWN;
  return f;
}



bool Plotter::uncheckedWrite(const std::string& filename, 
			     Plotter::FileFormat format)
{
  if (format == PS) return grace.printToPostscript(filename);
  else return false;
}



void Plotter::resetToSingleRegion()
{
  destroyAllRegions();
  theRegions.push_back(new PlotterRegion(0,this,0,0,1,1));
}





void Plotter::applyTitleFontChanges()
{
  const Grace::Font f = Utilities::resolveFont(titleStyle().textStyle());
  grace.setGlobalTitleFont(f);
}



} // end of namespace AidaNative

