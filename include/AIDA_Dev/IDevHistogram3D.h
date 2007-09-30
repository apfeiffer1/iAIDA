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
#ifndef AIDA_IDEVHISTOGRAM3D
#define AIDA_IDEVHISTOGRAM3D 1

#include "AIDA/IHistogram3D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a 3-dimensional binned histogram

class IDevHistogram3D : virtual public IHistogram3D,
                        virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevHistogram3D() {/*nop*/;}
  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
  virtual bool setBinContents( int binIndexX,
                               int binIndexY,
                               int binIndexZ,
                               int entries,
                               double height,
                               double error,
                               double centreX,
                               double centreY,
                               double centreZ ) = 0;
  /**
   * Sets the rms of the histogram.
   * It should be called after all calls to setBinContents
   */
  virtual bool setRms( double rmsX, double rmsY, double rmsZ ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVHISTOGRAM3D */
