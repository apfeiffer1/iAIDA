// Emacs -*- C++ -*-
#ifndef AIDA_IDATAPOINTSETFACTORYPROVIDER
#define AIDA_IDATAPOINTSETFACTORYPROVIDER 1

namespace AIDA {
  class IDataPointSetFactory;

  namespace Dev {
    class IDevDataPointSetFactory;
    class IDevTree;

/// Abstract interface class for a datapointset factory provider

class IDataPointSetFactoryProvider {

public:
  /// Destructor
  virtual ~IDataPointSetFactoryProvider() {/*nop*/;}
  /// Creates a new dataPointSet factory
  virtual IDataPointSetFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level dataPointSet factory which creates unmanaged objects
  virtual IDevDataPointSetFactory& devDataPointSetFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IDATAPOINTSETFACTORYPROVIDER */
