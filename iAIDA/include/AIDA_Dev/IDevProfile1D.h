// Emacs -*- C++ -*-
#ifndef AIDA_IDEVPROFILE1D
#define AIDA_IDEVPROFILE1D 1

#include "AIDA/IProfile1D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for an 1-dimensional profile histogram

class IDevProfile1D : virtual public IProfile1D,
		      virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevProfile1D() {/*nop*/;}
  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
  virtual bool setBinContents( int binIndex,
                               int entries,
                               double height,
                               double error,
                               double spread,
                               double centre ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVPROFILE1D */
