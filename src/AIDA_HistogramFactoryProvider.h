// Emacs -*- C++ -*-
#ifndef IAIDA_AIDAHISTOFACTORYPROVIDER
#define IAIDA_AIDAHISTOFACTORYPROVIDER 1

#include "AIDA_Dev/IHistogramFactoryProvider.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Implementation of the AIDA::Dev::IDevHistogramFactory interface class

class AIDA_HistogramFactoryProvider : virtual public AIDA::Dev::IHistogramFactoryProvider
{
public:
  /// Constructor
  AIDA_HistogramFactoryProvider();
  /// Destructor
  ~AIDA_HistogramFactoryProvider();
  /// Creates a new histogram factory
  AIDA::IHistogramFactory* createFactory( AIDA::Dev::IDevTree & tree );
  /// Returns the developer-level histogram factory which creates unmanaged objects
  AIDA::Dev::IDevHistogramFactory& devHistogramFactory();

private:
  AIDA::Dev::IDevHistogramFactory* m_factory;
};

  }
}

#endif /* ifndef IAIDA_AIDAHISTOFACTORYPROVIDER */
