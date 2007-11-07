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

# ifndef IAIDA_AIDA_FITTER_IDevFitDataIteratorFMLBase_h__included__
# define IAIDA_AIDA_FITTER_IDevFitDataIteratorFMLBase_h__included__


# include "AIDA_Dev/IDevFitDataIterator.h"

/**
 * \file Implementation for AIDA fitting component
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

namespace fml
{
    class IFMLDataIterator;
}

namespace iAIDA 
{
  namespace AIDA_Fitter
  {
    /**
     * Base class for all iterators implemented in this package.
     * This interface to access IFMLDataIterator directly, speeding up the fitting!
     * If an IDevFitDataIterator is not derived from this class, 
     * AIDAFitter will internally create an adapter to wrap around such iterator which 
     * is much slower (but still required to interconnect different AIDA implementations).
     */
    class IDevFitDataIteratorFMLBase : virtual public AIDA::Dev::IDevFitDataIterator
    {
    public:
      virtual ~IDevFitDataIteratorFMLBase() {}

      virtual fml::IFMLDataIterator & getIFMLDataIterator() = 0;
    };
  }
}

#endif
