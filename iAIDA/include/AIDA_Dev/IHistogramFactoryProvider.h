// Emacs -*- C++ -*-
#ifndef AIDA_IHISTOGRAMFACTORYPROVIDER
#define AIDA_IHISTOGRAMFACTORYPROVIDER 1

namespace AIDA {
  class IHistogramFactory;

  namespace Dev {
    class IDevHistogramFactory;
    class IDevTree;

/// Abstract interface class for a histogram factory provider

class IHistogramFactoryProvider {

public:
  /// Destructor
  virtual ~IHistogramFactoryProvider() {/*nop*/;}
  /// Creates a new histogram factory
  virtual IHistogramFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level histogram factory which creates unmanaged objects
  virtual IDevHistogramFactory& devHistogramFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IHISTOGRAMFACTORYPROVIDER */
