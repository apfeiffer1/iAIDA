// Emacs -*- C++ -*-
#ifndef AIDA_IDEVHISTOGRAM1D
#define AIDA_IDEVHISTOGRAM1D 1

#include "AIDA/IHistogram1D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for an 1-dimensional binned histogram

class IDevHistogram1D : virtual public IHistogram1D,
                        virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevHistogram1D() {/*nop*/;}
  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
  virtual bool setBinContents( int binIndex,
                               int entries,
                               double height,
                               double error,
                               double centre ) = 0;
  /**
   * Sets the rms of the histogram.
   * It should be called after all calls to setBinContents
   */
  virtual bool setRms( double rms ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVHISTOGRAM1D */
