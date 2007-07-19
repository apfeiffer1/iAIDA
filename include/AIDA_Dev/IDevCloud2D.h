// Emacs -*- C++ -*-
#ifndef AIDA_IDEVCLOUD2D
#define AIDA_IDEVCLOUD2D 1

#include "AIDA/ICloud2D.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a 2-dimensional unbinned histogram (cloud)

class IDevCloud2D : virtual public ICloud2D,
		    virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevCloud2D() {/*nop*/;}
  /// Sets the maximum number of elements, before conversion into a histogram
  virtual bool setCacheSize( unsigned int newCacheSize ) = 0;
  /// Retrieves the cache size
  virtual unsigned int cacheSize() const = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVCLOUD2D */
