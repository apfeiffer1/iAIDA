// Emacs -*- C++ -*-
#ifndef AIDA_IDEVMANAGEDOBJECT
#define AIDA_IDEVMANAGEDOBJECT 1

#include "AIDA/IManagedObject.h"

namespace AIDA {
  namespace Dev {

/// Developer-level interface for a manageable/storeable object

class IDevManagedObject : virtual public IManagedObject {

public:
  /// Destructor
  virtual ~IDevManagedObject() {/*nop*/;}

  /// Checks if the object is up-to-date with respect to its stored image
  virtual bool isUpToDate() const = 0;

  /// The up-to-date flag
  virtual void setUpToDate( bool isUpToDate ) = 0;

  /// Sets the name of the object
  virtual bool setName( const std::string& newName ) = 0;

  /// The AIDA user-level unterface leaf class type
  virtual const std::string& userLevelClassType() const = 0;
};

  }
}
#endif /* ifndef AIDA_IDEVMANAGEDOBJECT */
