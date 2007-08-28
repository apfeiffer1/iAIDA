// Emacs -*- C++ -*-
#ifndef AIDA_IDEVHISTOGRAM2D
#define AIDA_IDEVHISTOGRAM2D 1

#include "AIDA/IHistogram2D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a 2-dimensional binned histogram

class IDevHistogram2D : virtual public IHistogram2D,
                        virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevHistogram2D() {/*nop*/;}
  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
  virtual bool setBinContents( int binIndexX,
                               int binIndexY,
                               int entries,
                               double height,
                               double error,
                               double centreX,
                               double centreY ) = 0;
  /**
   * Sets the rms of the histogram.
   * It should be called after all calls to setBinContents
   */
  virtual bool setRms( double rmsX, double rmsY ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVHISTOGRAM2D */
