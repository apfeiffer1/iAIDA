// Emacs -*- C++ -*-
#ifndef AIDA_IDEVTUPLE
#define AIDA_IDEVTUPLE 1

#include "AIDA/ITuple.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace AIDA {
  namespace Dev {
    class IBackingStore;
    class ITupleHeader;

/// Developer-level abstact interface for a tuple

class IDevTuple : virtual public ITuple,
		  virtual public IDevManagedObject {

public:
  /// Destructor
  virtual ~IDevTuple() {/*nop*/;}

  /// Connects the tuple to a backing store
  virtual bool connectToStore( IBackingStore* store ) = 0;

  /// Returns the pointer to the supporting backing store
  virtual IBackingStore* store() = 0;
  virtual const IBackingStore* store() const = 0;

  /// Returns the header object
  virtual ITupleHeader& header() = 0;
  virtual const ITupleHeader& header() const = 0;

  /// Returns the address of a given variable
  virtual void* variableAddress( const std::string& variableName ) = 0;
  virtual const void* variableAddress( const std::string& variableName ) const = 0;

  /// set address for generic objects
  virtual void setVariableAddress(int column, void * p) = 0;  

};

  }
}

#endif /* ifndef AIDA_IDEVTUPLE */
