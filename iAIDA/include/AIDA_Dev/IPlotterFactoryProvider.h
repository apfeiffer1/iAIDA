#ifndef AIDA_IPLOTTERFACTORYPROVIDER
#define AIDA_IPLOTTERFACTORYPROVIDER 1

namespace AIDA {
  class IPlotterFactory;

  namespace Dev {

/// Abstract interface class for a plotter factory provider

class IPlotterFactoryProvider {

public:
  /// Destructor
  virtual ~IPlotterFactoryProvider() {/*nop*/;}
  /// Creates a new plotter factory
  virtual IPlotterFactory* createFactory( int argc = 0, char** args = 0 ) = 0;
};

  }
}

#endif /* ifndef AIDA_IPLOTTERFACTORYPROVIDER */
