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

// Implementation file for class RangeSet
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:54:59 2002
// 
// Last update: Fri Oct  4 16:54:59 2002
// 

# include <assert.h>

#include "FML/RangeSet.h"

#include <algorithm>

//#include "FML/Util/debug.h"


namespace fml  { 
  // namespace FML { 


    RangeSet::RangeSet() 
    {
      //default is including all 
      includeAll(); 
    }

    RangeSet::~RangeSet() 
    {
    }

    // implement copy constructor 
    RangeSet::RangeSet(const RangeSet & r) 
    {
      RangeSet * r2 = const_cast<RangeSet * > (&r); 
      m_lower = r2->lowerBounds();       
      m_upper = r2->upperBounds();       
      for (int i = 0; i < r2->size(); ++i) 
	m_ranges.push_back(std::make_pair(m_lower[i],m_upper[i]) ); 
    }

    RangeSet & RangeSet::operator = (const RangeSet &r) 
    {
      RangeSet * r2 = const_cast<RangeSet * > (&r); 
      m_lower = r2->lowerBounds();       
      m_upper = r2->upperBounds();       
      for (int i = 0; i < r2->size(); ++i) 
	m_ranges.push_back(std::make_pair(m_lower[i],m_upper[i]) ); 

      return *this;
    }


    const std::vector<double> & RangeSet::lowerBounds() 
    { 
      return m_lower; 
    }
      
    const std::vector<double> & RangeSet::upperBounds() 
    { 
      return m_upper; 
    }
    
    void RangeSet::include(double xMin, double xMax)
    { 
      // check if points are already included
      int iR1 = m_getInRange(xMin);
      int iR2 = m_getInRange(xMax); 

      // both points are in range 

      if ( iR1 >= 0 && iR2 >= 0) { 
	if (iR1 == iR2 ) 
	    return;   // do nothing since new values are already included
	else  
	  // add the new range and remove non-disjoint ones
	  m_addRange(xMin,xMax); 
      }
      else if ( iR1 >=0 && iR2 < 0) 
	  // add the new range and remove non-disjoint ones
	  m_addRange(m_ranges[iR1].first,xMax); 

      else if ( iR1 < 0 && iR2 >= 0) 
	  // add the new range and remove non-disjoint ones
	  m_addRange(xMin,m_ranges[iR2].second); 

      else  // both iR1 and iR2 are out 	 
	  // add the new range and remove non-disjoint ones
	  m_addRange(xMin,xMax); 
	
    }

    void RangeSet::exclude(double xMin, double xMax)
    { 
      // delete all ranges between xMin and xMax
      m_cleanRanges(xMin,xMax); 

      // check if points are already included
      int iR1 = m_getInRange(xMin);
      int iR2 = m_getInRange(xMax); 

      // case iR1 == iR2 && >=0 
      if (iR1 >=0 && iR1 == iR2) { 
	double x2 = m_ranges[iR1].second;
	// modify existing one 
	m_ranges[iR1].second = xMin;
	// and add a new range
	m_addRange(xMax,x2); 
      }
      // if xMin is in range iR1 reduced range up to xMin
      else if ( iR1 >= 0 ) 
	  m_ranges[iR1].second = xMin;  

	// if xMax is in range iR2 reduced range from xMax
      
      else if ( iR2 >= 0 ) 
	m_ranges[iR2].first = xMax;  

    }

    
    void RangeSet::includeAll() 
    { 
      m_clearRanges();   // clear ranges 
      include(MINUS_INF(),PLUS_INF());  
    }

    void RangeSet::excludeAll() 
    { 
      m_clearRanges();   // clear ranges 
    }

    bool RangeSet::isInRange(double point) 
    { 
      // check if point is in range
      if (m_getInRange(point) >= 0 ) return true; 
      return false; 
    }

    int RangeSet::size() 
    { 
      return m_ranges.size(); 
    }


    // return index of range which contains the point. 
    // return -1 if point is not in range
    int RangeSet::m_getInRange(double point) { 
    // look for all ranges 
      for (int i = 0; i < size() ; ++i) { 
	if (point >= m_ranges[i].first && point < m_ranges[i].second )
	  return i; 
      }
      return -1; 
    }
      

    // add a new range and clean up deleting already included ones
    void  RangeSet::m_addRange(double xMin, double xMax) { 

      // clean up : loop on existing ranges 
      m_cleanRanges(xMin,xMax); 
      // add the new range
      m_ranges.push_back(std::make_pair(xMin,xMax)); 

      // sort new ranges with increasing values  
      sort(m_ranges.begin(), m_ranges.end()); 

      m_updateLowerAndUpper();
    }

    // remove ranges between xmin and xmax
    void  RangeSet::m_cleanRanges(double xMin, double xMax) { 

      // clean up : loop on existing ranges 
      for (M_RangeSet::iterator itr = m_ranges.begin(); itr != m_ranges.end(); ++itr) { 
	// delete included ranges
	if ( itr->first >= xMin && itr->second <= xMax) { 
	  m_ranges.erase(itr);
	  // itr goes to next element, so go back before adding
	  --itr;
	}
      }
      m_updateLowerAndUpper();

    } 

    void RangeSet::m_clearRanges()
    { 
      m_lower.clear();
      m_upper.clear();
      m_ranges.clear();
    }

    void RangeSet::m_updateLowerAndUpper() 
    { 
      m_lower.clear(); 
      for (M_RangeSet::iterator itr = m_ranges.begin(); itr != m_ranges.end(); ++itr)
	m_lower.push_back(itr->first); 

      m_upper.clear(); 
      for (M_RangeSet::iterator itr = m_ranges.begin(); itr != m_ranges.end(); ++itr)
	m_upper.push_back(itr->second); 
    }

   // }    // end of namespace fml::FML
}
