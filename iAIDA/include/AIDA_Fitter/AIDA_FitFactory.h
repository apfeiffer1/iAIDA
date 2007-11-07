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
#ifndef IAIDA_FITFACTORY
#define IAIDA_FITFACTORY 1

#include <string>
#include "AIDA/IFitFactory.h"

// type ahead declarations 
namespace AIDA {
  class IFitter;
}

namespace iAIDA {
  namespace AIDA_Fitter {

/// Implementation of the AIDA::IFitFactory abstract interface

class AIDA_FitFactory : public virtual AIDA::IFitFactory
{
public:
  /// Constructor
  AIDA_FitFactory();
  /// Destructor
  ~AIDA_FitFactory();
  /// Methods inherited from the IFitFactory class
  AIDA::IFitData* createFitData();
  AIDA::IFitter* createFitter( const std::string &fitterType = "Chi2",
			       const std::string &engineType = "",
			       const std::string &options = "");
};

  }
}

#endif /* ifndef IAIDA_FITFACTORY */
