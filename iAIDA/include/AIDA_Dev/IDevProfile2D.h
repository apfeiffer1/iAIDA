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
