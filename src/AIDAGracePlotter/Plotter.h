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
 * \file Header file for class Plotter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Mon Aug 26 17:10:31 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTTER_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTTER_H


#include <list>
#include "TextMessage.h"
#include "TitleStyle.h"



/// Global error trapping function
extern "C" {
void formattedGraceError(const char*);
}



namespace AidaNative {



class PlotterRegion;
class ParameterTable;
class Grace;



/**
 * Implementation of Plotter based on Grace
 */

class Plotter
{
public:
  Plotter(const std::string& nameOfDefaultPTable = "");
  virtual ~Plotter();

private:
  /// not permitted
  Plotter(const Plotter&);
  /// not permitted
  Plotter& operator=(const Plotter&);

public:
  virtual PlotterRegion* createRegion(double bottomLeftX,
				      double bottomLeftY,
				      double width,
				      double height);
  virtual bool createRegions(int columns, int rows, int index=0);
  virtual PlotterRegion& currentRegion() const;
  virtual PlotterRegion& next() const;
  virtual PlotterRegion* region(int regionNumber) const;
  virtual bool setCurrentRegionNumber(int newRegionNumber);
  virtual int  currentRegionNumber() const;
  virtual int numberOfRegions() const;
  virtual bool destroyAllRegions();
  virtual bool clearAllRegions();
  virtual bool setParameter(const std::string& parameterName, 
			    const std::string& options="");
  virtual std::string parameterValue(const std::string& parameterName) const;
  virtual std::vector<std::string> availableParameters() const;
  virtual std::vector<std::string> 
    availableOptions(const std::string& parameterName) const;
  virtual bool writeToFile(const std::string& filename, 
			   const std::string& fileType="");
  virtual bool refresh();
  virtual bool show();
  virtual bool hide();
  virtual bool interact();
  virtual bool setTitle(const std::string&);
  virtual TitleStyle& titleStyle();
  virtual bool setTitleStyle(const TitleStyle&);

private:
  enum FileFormat {UNKNOWN = -1, PS = 1, EPS, PNG, JPG, SVG};
  typedef std::vector<PlotterRegion*> RegionMap;
  typedef RegionMap::iterator RegionMapIt;
  typedef RegionMap::const_iterator RegionMapCIt;

private:
  static const TextMessage unknownParameter;
  static const TextMessage badRegionParameters;
  static const TextMessage regionOutOfRange;
  static const TextMessage fileTypeConflict;
  static const TextMessage fileTypeUnknown;
  static const TextMessage singleRegionOnly;

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");
  /// e.g. "myfile.ps" returns PS. returns UNKNOWN if, er.. unknown.
  static FileFormat getFileFormatFromSuffix(const std::string& filename);
  /// e.g. "postscript" returns PS. returns UNKNOWN if, er.. unknown.
  static FileFormat getFileFormatFromDescription(const std::string& desc);

private:
  bool uncheckedWrite(const std::string& filename, FileFormat);
  void resetToSingleRegion();
  void applyTitleFontChanges();

private:
  Grace& grace;
  ParameterHandler parameterHandler;
  RegionMap theRegions;
  int m_currentRegion;
  std::string m_title;
  TitleStyle m_titleStyle;

};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_PLOTTER_H

