#ifndef AIDA_DEV_IFUNCTIONFACTORYPROVIDER_H
#define AIDA_DEV_IFUNCTIONFACTORYPROVIDER_H

namespace AIDA {
  class IFunctionFactory;

  namespace Dev {
    class IDevFunctionFactory;
    class IDevTree;

/// Abstract interface class for a function factory provider

class IFunctionFactoryProvider {

public:
  /// Destructor
  virtual ~IFunctionFactoryProvider() {/*nop*/;}
  /// Creates a new function factory
  virtual IFunctionFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level function factory which creates unmanaged objects
  virtual IDevFunctionFactory& devFunctionFactory() = 0;
};

  }
}


#endif
