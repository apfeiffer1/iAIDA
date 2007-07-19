// Emacs -*- C++ -*-
#ifndef AIDA_ITUPLEFACTORYPROVIDER
#define AIDA_ITUPLEFACTORYPROVIDER 1

namespace AIDA {
  class ITupleFactory;

  namespace Dev {
    class IDevTupleFactory;
    class IDevTree;

/// Abstract interface class for a tuple factory provider

class ITupleFactoryProvider {

public:
  /// Destructor
  virtual ~ITupleFactoryProvider() {/*nop*/;}
  /// Creates a new tuple factory
  virtual ITupleFactory* createFactory( IDevTree & tree ) = 0;
  /// Returns the developer-level tuple factory
  virtual IDevTupleFactory& devTupleFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_ITUPLEFACTORYPROVIDER */
