// Emacs -*- C++ -*-
#ifndef AIDA_IBACKINGSTORE
#define AIDA_IBACKINGSTORE 1

#include <string>
#include <vector>

namespace AIDA {

  namespace Dev {
  class ITupleHeader;

/// Abstract interface class for a backing store.

class IBackingStore
{
public:
  /// Destructor
  virtual ~IBackingStore() {/*nop*/;}

  /// Writes the header
  virtual bool writeTupleHeader( ITupleHeader& header ) = 0;

  /// Reads (fills) a tuple header
  virtual bool readTupleHeader( ITupleHeader& header ) = 0;

  /// Binds a variable
  virtual bool bindVariable( ITupleHeader& header, int variableIndex ) = 0;

  /// Returns the address of a variable. It performs automatic binding if needed
  virtual void* variableAddress( ITupleHeader& header, int variableIndex ) = 0;
  virtual const void* variableAddress( const ITupleHeader& header, int variableIndex ) const = 0;

  /// set the address of a variable. Needed when type of object is not known 
  virtual void setVariableAddress( const ITupleHeader& header, int variableIndex, void * p ) = 0;

  /// Clears the existing bindings
  virtual bool clearBindings( const ITupleHeader& header ) = 0;

  /// Writes a tuple row into the underlying persistency solution
  virtual bool writeTupleRow( ITupleHeader& header ) = 0;

  /// Reads (fills) a tuple row from the underlying persistency solution
  virtual bool readTupleRow( ITupleHeader& header ) = 0;  

  /// Clears all the rows in the tuple
  virtual bool resetTuple( ITupleHeader& header ) = 0;
};

  }
}

#endif /* ifndef AIDA_IBACKINGSTORE */
