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
