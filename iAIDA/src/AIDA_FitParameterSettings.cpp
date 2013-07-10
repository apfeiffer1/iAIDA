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

// Implementation file for class AIDA_FitParameterSettings
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:14:29 2002
// 
// Last update: Fri Oct  4 16:14:29 2002
// 

#include "AIDA_FitParameterSettings.h"
#include "FML/FitParameterSettings.h"

namespace iAIDA  { 
  namespace AIDA_Fitter { 

    // constructor taking an FML Fit parameter setting pointer 

    AIDA_FitParameterSettings::AIDA_FitParameterSettings(fml::FitParameterSettings* ptr) : m_settings(ptr)
    {}

    AIDA_FitParameterSettings::~AIDA_FitParameterSettings() 
    {
    }

    AIDA_FitParameterSettings::AIDA_FitParameterSettings(const AIDA_FitParameterSettings &) 
    {
    }

    AIDA_FitParameterSettings & AIDA_FitParameterSettings::operator = (const AIDA_FitParameterSettings &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }


    std::string AIDA_FitParameterSettings::name() const
    {
      return m_settings->name();
    }
    
    double AIDA_FitParameterSettings::stepSize() const
    {
      return m_settings->stepSize();
    }

    double AIDA_FitParameterSettings::upperBound() const
    {
      return m_settings->bound().upperBound();
    }
    
    double AIDA_FitParameterSettings::lowerBound() const
    {
      return m_settings->bound().lowerBound();
    }
    
    bool AIDA_FitParameterSettings::isBound() const
    {
      return m_settings->bound().isBound();
    }
    
    bool AIDA_FitParameterSettings::isFixed() const
    {
      return m_settings->bound().isFixed();
    }
    
    void AIDA_FitParameterSettings::setStepSize(double step)
    {
      m_settings->setStepSize(step);
    }
    
    void AIDA_FitParameterSettings::setBounds(double lo, double up)
    {
      m_settings->bound() = fml::Bound(lo,up);
    }

    void AIDA_FitParameterSettings::removeBounds()
    {
      m_settings->removeBounds();
    }

    void AIDA_FitParameterSettings::setFixed(bool isFixed)
    {
      m_settings->bound().setFixed(isFixed);
    }
    
    void AIDA_FitParameterSettings::setLowerBound(double lowerBound)
    {
      m_settings->bound() = fml::LowerBound(lowerBound);
    }
    
    void AIDA_FitParameterSettings::setUpperBound(double upperBound)
    {
      m_settings->bound() = fml::UpperBound(upperBound);
    }
    
    void AIDA_FitParameterSettings::reset()
    {
      m_settings->reset();
    }


  }     // end of namespace iAIDA::AIDA_Fitter
} 
