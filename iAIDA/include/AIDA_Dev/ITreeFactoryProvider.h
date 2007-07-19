// Emacs -*- C++ -*-
#ifndef AIDA_ITREEFACTORYPROVIDER
#define AIDA_ITREEFACTORYPROVIDER 1

namespace AIDA {
  class ITreeFactory;

  namespace Dev {

/// Abstract interface class for a tree factory provider

class ITreeFactoryProvider {

public:
  /// Destructor
  virtual ~ITreeFactoryProvider() {/*nop*/;}
  /// Creates a new tree factory
  virtual ITreeFactory* createFactory() = 0;
};

  }
}

#endif /* ifndef AIDA_ITREEFACTORYPROVIDER */
