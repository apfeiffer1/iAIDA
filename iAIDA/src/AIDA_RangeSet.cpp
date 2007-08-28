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

// Implementation file for class AIDA_RangeSet
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:54:59 2002
// 
// Last update: Fri Oct  4 16:54:59 2002
// 

#include "AIDA_Function/AIDA_RangeSet.h"
#include "FML/RangeSet.h"

namespace iAIDA  { 
  namespace AIDA_Function { 


    AIDA_RangeSet::AIDA_RangeSet(fml::RangeSet * rs) : 
      m_impl(rs), m_own(false)
    {
    }

    AIDA_RangeSet::AIDA_RangeSet() : m_own(true)
    {
      m_impl = new fml::RangeSet(); 
    }

    AIDA_RangeSet::~AIDA_RangeSet() 
    {
      // here need to delete if I create ? 
      if (m_own) delete m_impl; 
    }

    AIDA_RangeSet::AIDA_RangeSet(const AIDA_RangeSet &) 
    {
    }

    AIDA_RangeSet & AIDA_RangeSet::operator = (const AIDA_RangeSet &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test

      return *this;
    }


    const std::vector<double> & AIDA_RangeSet::lowerBounds() 
    { 
      return m_impl->lowerBounds(); 
    }
      
    const std::vector<double> & AIDA_RangeSet::upperBounds() 
    { 
      return m_impl->upperBounds(); 
    }
    
    void AIDA_RangeSet::include(double xMin, double xMax)
    { 
      m_impl->include(xMin, xMax);
    }

    void AIDA_RangeSet::exclude(double xMin, double xMax)
    { 
      m_impl->exclude(xMin, xMax);
    }
    
    void AIDA_RangeSet::includeAll() 
    { 
      m_impl->includeAll();
    }

    void AIDA_RangeSet::excludeAll() 
    { 
      m_impl->excludeAll();
    }

    bool AIDA_RangeSet::isInRange(double point) 
    { 
      return  m_impl->isInRange(point);
    }

    int AIDA_RangeSet::size() 
    { 
      return  m_impl->size(); 
    }

    double AIDA_RangeSet::PLUS_INF() {
      return m_impl->PLUS_INF(); 
    }

    double AIDA_RangeSet::MINUS_INF() { 
      return m_impl->MINUS_INF(); 
    }

    fml::RangeSet &  AIDA_RangeSet::getFMLRangeSet() { 
      return *m_impl; 
    }
      
  }     // end of namespace iAIDA::AIDA_Function
} 



