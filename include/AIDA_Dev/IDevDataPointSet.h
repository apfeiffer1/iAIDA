// Emacs -*- C++ -*-
#ifndef AIDA_IDEVDATAPOINTSET
#define AIDA_IDEVDATAPOINTSET 1

#include "AIDA/IDataPointSet.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a data point set

class IDevDataPointSet : virtual public IDataPointSet,
			 virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevDataPointSet() {/*nop*/;}
};

  }
}

#endif /* ifndef AIDA_IDEVDATAPOINTSET */
