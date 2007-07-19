// Emacs -*- C++ -*-
#ifndef AIDA_ISTOREFACTORY
#define AIDA_ISTOREFACTORY 1

#include <string>

namespace AIDA {
  namespace Dev {
    class IStore;

/// Abstract interface class for a store factory

class IStoreFactory {

public:
  /// Destructor
  virtual ~IStoreFactory() {/*nop*/;}
  /// Creates a new store
  virtual IStore* createStore( const std::string & storeName,
                               bool readOnly,
			       bool createNew,
                               const std::string & options ) = 0;
  /// Deletes a store object
  virtual bool destroyStore( IStore * store ) = 0;
};

  }
}

#endif /* ifndef AIDA_ISTOREFACTORY */
