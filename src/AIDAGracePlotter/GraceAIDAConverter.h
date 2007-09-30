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
 * \file Header file for class GraceAIDAConverter
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Mon Sep 30 19:27:10 2002
// 

#ifndef IAIDA_GRACEAIDACONVERTER_H
#define IAIDA_GRACEAIDACONVERTER_H


#include <vector>
#include <string>
#include "TextMessage.h"



namespace AIDA {
  class IHistogram1D;
  class IHistogram2D;
  class IProfile1D;
  class ICloud2D;
  class IFunction;
  class IDataPointSet;
}


namespace AidaNative {



  /*
  using AIDA::IHistogram1D;
  using AIDA::IHistogram2D;
  using AIDA::IAxis;
  */



/**
 * Converts AIDA histograms into vector of doubles suitable for parsing by Grace
 * (one point per vector).
 */

class GraceAIDAConverter
{
private:
  GraceAIDAConverter();
  virtual ~GraceAIDAConverter();

public:
  static std::vector<std::vector<double> > 
    histo1DAsDoubles(const AIDA::IHistogram1D*);
  static std::vector<std::vector<double> > 
    histo2DAsDoubles(const AIDA::IHistogram2D*);
  static std::vector<std::vector<double> >
    profile1DAsDoubles(const AIDA::IProfile1D*);
  static std::vector<std::vector<double> >
    cloud2DAsDoubles(const AIDA::ICloud2D*);
  static std::vector<std::vector<double> >
    function1DAsDoubles(const AIDA::IFunction*, 
			double min, double max, int n);
  static std::vector<std::vector<double> > 
    dataPointSet2DAsDoubles(const AIDA::IDataPointSet*);

private:
  static const TextMessage functionIsNot1D;
  static const TextMessage dataPointSetIsNot2D;
};



} // end of namespace AidaNative


#endif // IAIDA_GRACEAIDACONVERTER_H
