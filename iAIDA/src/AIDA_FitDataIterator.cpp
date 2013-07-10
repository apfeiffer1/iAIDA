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

// Implementation file for class AIDA_FitDataIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:44:49 2002
// 
// Last update: Fri Oct  4 16:44:49 2002
// 

#include "AIDA_FitDataIterator.h"
#include "AIDA_Function/AIDA_RangeSet.h"
#include "FML/Util/debug.h"

#include <assert.h>


namespace iAIDA  { 
  namespace AIDA_Fitter { 


    AIDA_FitDataIterator::AIDA_FitDataIterator() 
    {
    }
    
    AIDA_FitDataIterator::~AIDA_FitDataIterator() 
    {
    }



     void AIDA_FitDataIterator::start()
    {
      assert(m_it.get());
      m_it->start();
    }

    bool AIDA_FitDataIterator::next()
    {
      assert(m_it.get());
      m_it->next();
      return m_it->current();
    }

    int AIDA_FitDataIterator::entries() const
    {
      assert(m_it.get());
      return m_it->numberOfEntries();
    }

    double AIDA_FitDataIterator::value() const
    {
      assert(m_it.get());
      return m_it->value();
    }

    const std::vector<double>& AIDA_FitDataIterator::vars() const
    {
      assert(m_it.get());
      return m_it->coords();
    }

    double AIDA_FitDataIterator::error() const
    {
      assert(m_it.get());
      return m_it->vErrorPlus();
    }
    
    double AIDA_FitDataIterator::minusError() const
    {
      assert(m_it.get());
      return m_it->vErrorMinus();
    }
  
    fml::IFMLDataIterator & AIDA_FitDataIterator::getIFMLDataIterator()
    {
      return *m_it.get();
    }

    void AIDA_FitDataIterator::setVector(std::auto_ptr<fml::DataVector> vptr)
    {
      m_vector = vptr;
      std::auto_ptr<fml::DataVectorIterator> it(new fml::DataVectorIterator(*m_vector.get()));
      m_it = it;						
    }

    void AIDA_FitDataIterator::fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & /* ranges */ ) 
    { 
      INFO_MSG(" AIDA_FitDataIterator::fillVector() should be called by derived classes ! ");  
    }

  }     // end of namespace iAIDA::AIDA_Fitter
} 
