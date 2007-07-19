#ifndef AIDA_IFITFACTORYPROVIDER
#define AIDA_IFITFACTORYPROVIDER 1

namespace AIDA {
  class IFitFactory;

  namespace Dev {

/// Abstract interface class for a fit factory provider

class IFitFactoryProvider {

public:
  /// Destructor
  virtual ~IFitFactoryProvider() {/*nop*/;}
  /// Creates a new fit factory
  virtual IFitFactory* createFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_IFITFACTORYPROVIDER */
