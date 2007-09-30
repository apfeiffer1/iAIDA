 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Header file for class AIDA_RangeSet
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:54:59 2002
// 
// Last update: Fri Oct  4 16:54:59 2002
// 
#ifndef IAIDA_AIDA_FUNCTION_AIDA_RANGESET
#define IAIDA_AIDA_FUNCTION_AIDA_RANGESET 1

#include "AIDA/IRangeSet.h"

/** 
     wrapper to FML Range
*/
   


namespace fml  { 
     class RangeSet; 
 }

namespace iAIDA { 
   namespace AIDA_Function { 


class AIDA_RangeSet : virtual public AIDA::IRangeSet {

 public: 
  // create a new range from scratch 
  AIDA_RangeSet();   
  // create a new range from an existing FML range 
  AIDA_RangeSet(fml::RangeSet * r);

  virtual ~AIDA_RangeSet(); 

 private:
  // usually copying is non trivial, so we make this unaccessible
  AIDA_RangeSet(const AIDA_RangeSet &); 
  AIDA_RangeSet & operator = (const AIDA_RangeSet &); 
  
 public: 

  /// from AIDA interface

   const std::vector<double>  & lowerBounds();

    /** 
     * Return array of upper Bounds for the current set of ranges
     * @return Array of upper bounds for all valid ranges
     */
   const std::vector<double>  & upperBounds();

    /**
     * Add [xMin, xMax] interval to existing set of valid ranges
     * @param xMin - lower bound of a new valid range
     * @param xMax - upper bound of a new valid range
     */  
   void include(double xMin, double xMax);

    /**
     * Exclude [xMin, xMax] interval from the existing set of valid ranges
     * @param xMin - lower bound of range to be excluded 
     * @param xMax - upper bound of range to be excluded
     */  
   void exclude(double xMin, double xMax);

    /**
     * Set full range (from MINUS_INF to PLUS_INF ).
     */
   void includeAll();

    /**
     * Set empty range.
     */
   void excludeAll();

    /*
     *return Return true if the point is in range 
     */
   bool isInRange(double point);

    /// Return current number of disjoint ranges (non-overlapping intervals).

   int size();

    
    /// Define +/-infinity according to specific implementation
    
   double PLUS_INF(); 

   double MINUS_INF(); 

   /// implemented methods (not in AIDA) 
   fml::RangeSet &  getFMLRangeSet(); 

protected: 


private: 

   // implemented range set
   fml::RangeSet * m_impl; 
   // ownership of RangeSet
   bool m_own;
}; 

  }     // end of namespace iAIDA::AIDA_Function
} 
#endif /* AIDA_RANGESET */
