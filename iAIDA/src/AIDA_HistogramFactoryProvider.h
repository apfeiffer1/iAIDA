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

// Emacs -*- C++ -*-
#ifndef IAIDA_AIDAHISTOFACTORYPROVIDER
#define IAIDA_AIDAHISTOFACTORYPROVIDER 1

#include "AIDA_Dev/IHistogramFactoryProvider.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Implementation of the AIDA::Dev::IDevHistogramFactory interface class

class AIDA_HistogramFactoryProvider : virtual public AIDA::Dev::IHistogramFactoryProvider
{
public:
  /// Constructor
  AIDA_HistogramFactoryProvider();
  /// Destructor
  ~AIDA_HistogramFactoryProvider();
  /// Creates a new histogram factory
  AIDA::IHistogramFactory* createFactory( AIDA::Dev::IDevTree & tree );
  /// Returns the developer-level histogram factory which creates unmanaged objects
  AIDA::Dev::IDevHistogramFactory& devHistogramFactory();

private:
  AIDA::Dev::IDevHistogramFactory* m_factory;
};

  }
}

#endif /* ifndef IAIDA_AIDAHISTOFACTORYPROVIDER */
