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
#ifndef AIDA_IDEVDATAPOINTSET
#define AIDA_IDEVDATAPOINTSET 1

#include "AIDA/IDataPointSet.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a data point set

class IDevDataPointSet : virtual public IDataPointSet,
			 virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevDataPointSet() {/*nop*/;}
};

  }
}

#endif /* ifndef AIDA_IDEVDATAPOINTSET */
