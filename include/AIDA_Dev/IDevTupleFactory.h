// Emacs -*- C++ -*-
#ifndef AIDA_IDEVTUPLEFACTORY
#define AIDA_IDEVTUPLEFACTORY 1

#include <string>
#include <vector>

namespace AIDA {
  class ITuple;

  namespace Dev {
    class IBackingStore;
    class IDevTuple;
    class ITupleVariableDescription;
    class ITupleHeader; 

/// Abstract interface class for a factory which creates tuple objects that are not attached to a tree

class IDevTupleFactory {

public:
  /// Destructor
  virtual ~IDevTupleFactory() {/*nop*/;}
  /// Creates an unmanaged tuple
  virtual IDevTuple* create( IBackingStore* backingStore,
			     const std::string& title,
			     std::string options = "") = 0;


  /// Creates a chained tuple
  virtual IDevTuple* createChained( const std::string& title,
				    const std::vector<IDevTuple*>& set ) = 0;
  /// Creates a variable description object
  virtual ITupleVariableDescription* createDescription() = 0;
  /// Creates a copy of a description object, with emtpy statistics
  virtual ITupleVariableDescription* createDescription( const ITupleVariableDescription& original ) = 0;

  ///Build a tuple header  from columns names and types and path in store  
  virtual bool buildTupleHeader(const std::vector<std::string>& columnNames, 
				const std::vector<std::string>& columnTypes,
				const std::string & pathInStore, 
				ITupleHeader & header) = 0;  

  ///Build a tuple header  from columns description and path in store  
  virtual bool buildTupleHeader(const std::string& columnDescr, 
				const std::string & pathInStore, 
				ITupleHeader & header) = 0;  
  
};
   


  }
}

#endif /* ifndef AIDA_IDEVTUPLEFACTORY */
