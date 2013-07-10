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

# ifndef IAIDA_AIDA_FITTER_FMLFitDataIteratorAdapter_h__included__
# define IAIDA_AIDA_FITTER_FMLAFitDataIteratorAdapter_h__included__

/**
 * \file Implementation for AIDA fitting component
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

# include "AIDA_Dev/IDevFitDataIterator.h"
# include "FML/IFMLDataIterator.h"

# include <memory>
# include <vector>

namespace iAIDA 
{
  namespace AIDA_Fitter
  {

    /**
     * Adapt the AIDA data iterator to IDevFitDataIterator
     *
     */
    
    class FMLFitDataIteratorAdapter : virtual public fml::IFMLDataIterator
    {
    public:

      FMLFitDataIteratorAdapter(AIDA::Dev::IDevFitDataIterator *  it);

      void start();
      bool current() const;
      void next();
      int numberOfEntries() const;
      const std::vector<double> &coords() const;
      double value() const;
      double vErrorPlus() const;
      double vErrorMinus() const;

    private:
      AIDA::Dev::IDevFitDataIterator *  m_it;
      bool m_curr;
    };
 
  }
}


# endif
