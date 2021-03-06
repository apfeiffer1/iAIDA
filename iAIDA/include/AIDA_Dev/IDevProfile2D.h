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
#ifndef AIDA_IDEVPROFILE2D
#define AIDA_IDEVPROFILE2D 1

#include "AIDA/IProfile2D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a 2-dimensional profile histogram

class IDevProfile2D : virtual public IProfile2D,
		      virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevProfile2D() {/*nop*/;}
  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
  virtual bool setBinContents( int binIndexX,
                               int binIndexY,
                               int entries,
                               double height,
                               double error,
                               double spread,
                               double centreX,
                               double centreY ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVPROFILE1D */
