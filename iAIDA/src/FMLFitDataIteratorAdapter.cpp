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

# include "FMLFitDataIteratorAdapter.h"

#include <assert.h>


namespace iAIDA  
{

  namespace AIDA_Fitter
  {

    FMLFitDataIteratorAdapter::FMLFitDataIteratorAdapter(AIDA::Dev::IDevFitDataIterator* it) : m_it(it), m_curr(false) 
    {
      assert(m_it);
    }

    void FMLFitDataIteratorAdapter::start()
    {
      m_it->start();
      m_curr = m_it->next();
    }
    
    bool FMLFitDataIteratorAdapter::current() const
    {
      return m_curr;
    }
    
    void FMLFitDataIteratorAdapter::next()
    {
      m_curr = m_it->next();      
    }

    int FMLFitDataIteratorAdapter::numberOfEntries() const
    {
      return m_it->entries();
    }
    
    const std::vector<double> &FMLFitDataIteratorAdapter::coords() const
    {
      return m_it->vars();
    }
    
    double FMLFitDataIteratorAdapter::value() const
    {
      return m_it->value();
    }
    
    double FMLFitDataIteratorAdapter::vErrorPlus() const
    {      
      return m_it->error();
    }
    
    double FMLFitDataIteratorAdapter::vErrorMinus() const
    {
      return m_it->minusError();
    }

  }
}
