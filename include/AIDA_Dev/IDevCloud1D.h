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
#ifndef AIDA_IDEVCLOUD1D
#define AIDA_IDEVCLOUD1D 1

#include "AIDA/ICloud1D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for an 1-dimensional unbinned histogram (cloud)

class IDevCloud1D : virtual public ICloud1D,
		    virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevCloud1D() {/*nop*/;}
  /// Sets the maximum number of elements, before conversion into a histogram
  virtual bool setCacheSize( unsigned int newCacheSize ) = 0;
  /// Retrieves the cache size
  virtual unsigned int cacheSize() const = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVCLOUD1D */
