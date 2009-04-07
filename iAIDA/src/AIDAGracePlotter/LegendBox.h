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

//
// (c) CERN, Geneva
//

//
// Header file for class LegendBox
// 
// Created by user sang on Mon Aug  5 12:23:42 2002
// 

#ifndef IAIDA_QPLOTTER_NEW_VIEWER_LEGENDBOX_H
#define IAIDA_QPLOTTER_NEW_VIEWER_LEGENDBOX_H


#include "MarkerStyle.h"
#include "FillStyle.h"
#include "LineStyle.h"



namespace AidaNative {



/**
 * Box containing text and/or legends (i.e. marker+text, line+text, 
 * fill+text pairs) VERY UNFINISHED!
 */

class LegendBox
{
public:
  LegendBox(const std::string& nameOfDefaultTable = "");
  virtual ~LegendBox();

public:
  virtual void resetParameters();
  std::string parameterValue(const std::string& parameterName) const;
  virtual bool setParameter(const std::string& paramName, const std::string& options);
  virtual std::vector<std::string> availableParameters() const;
  virtual std::vector<std::string> availableOptions(const std::string& paramName) const;
  virtual void clear();
  virtual bool addText(const std::string&);
  virtual bool addLegend(const std::string&, const MarkerStyle&);
  virtual bool addLegend(const std::string&, const LineStyle&);
  virtual bool addLegend(const std::string&, const FillStyle&);

private:
  /// Generates "default" table of parameters according to string argument
  static ParameterTable newDefaultPTable(const std::string& nameOfDefaultTable = "");

private:
  typedef std::pair<MarkerStyle,std::string> MarkerTextPair;
  typedef std::pair<LineStyle,std::string> LineTextPair;
  typedef std::pair<FillStyle,std::string> FillTextPair;
  typedef std::vector<std::string> TextList;
  typedef std::vector<MarkerTextPair> MarkerList;
  typedef std::vector<FillTextPair> FillList;
  typedef std::vector<LineTextPair> LineList;

private:
  ParameterHandler parameterHandler;
  TextList textList;
  MarkerList markerList;
  LineList lineList;
  FillList fillList;

};



} // end of namespace AidaNative


#endif //  IAIDA_QPLOTTER_NEW_VIEWER_LEGENDBOX_H

