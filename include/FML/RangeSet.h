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

// Header file for class RangeSet
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:54:59 2002
// 
// Last update: Fri Oct  4 16:54:59 2002
// 
#ifndef AIDANATIVE_FML_RANGESET
#define AIDANATIVE_FML_RANGESET 1

#include <vector>

#include <float.h>

/** 
   implement ranges to be applied to the data or functions 

*/


namespace fml  { 
  // namespace FML { 


    class RangeSet {

    public: 
      RangeSet(); 
      virtual ~RangeSet(); 
      
      // implement copy constructors 
      RangeSet(const RangeSet &); 
      RangeSet & operator = (const RangeSet &); 
      
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
    
      inline double PLUS_INF() { return FLT_MAX; }

      inline double MINUS_INF() { return -FLT_MAX; } 


    protected: 

      // get index of range containing point
      int m_getInRange(double point); 
      // add a new range [xmin,xmax] deleting previous ranges full contained
      void m_addRange(double xMin, double xMax);
      // clean ranges present between xmin and xmax 
      void m_cleanRanges(double xMin, double xMax);
      // clear all internal  range vectors
      void m_clearRanges(); 
      // update lower and upper bound vectors
      void m_updateLowerAndUpper(); 

    private: 

   // vector of pair containing ranges 
      typedef std::vector<std::pair<double,double> > M_RangeSet;
      M_RangeSet m_ranges; 
      
      // cache uppper and lower bounds
      mutable std::vector<double> m_lower; 
      mutable std::vector<double> m_upper; 

    }; 

  // }     // end of namespace FML
} // end namespace fml
#endif /* RANGESET */
