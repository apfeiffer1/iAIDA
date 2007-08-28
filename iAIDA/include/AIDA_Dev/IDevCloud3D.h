// Emacs -*- C++ -*-
#ifndef AIDA_IDEVCLOUD3D
#define AIDA_IDEVCLOUD3D 1

#include "AIDA/ICloud3D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a 3-dimensional unbinned histogram (cloud)

class IDevCloud3D : virtual public ICloud3D,
		    virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevCloud3D() {/*nop*/;}
  /// Sets the maximum number of elements, before conversion into a histogram
  virtual bool setCacheSize( unsigned int newCacheSize ) = 0;
  /// Retrieves the cache size
  virtual unsigned int cacheSize() const = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVCLOUD3D */
