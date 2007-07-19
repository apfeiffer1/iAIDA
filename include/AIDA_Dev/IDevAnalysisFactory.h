// Emacs -*- C++ -*-
#ifndef AIDA_IDEVANALYSISFACTORY
#define AIDA_IDEVANALYSISFACTORY 1

#include <string>
#include "AIDA/IAnalysisFactory.h"

namespace AIDA {

  namespace Dev {
    class IStoreFactory;
    class IAnnotationFactory;
    class IHistogramFactoryProvider;
    class IDataPointSetFactoryProvider;
    class ITreeFactoryProvider;
    class ITupleFactoryProvider;
    class IFunctionFactoryProvider;
    class IFitFactoryProvider;
    class IPlotterFactoryProvider;

/// Developer-level interface for an analysis factory

class IDevAnalysisFactory : public virtual IAnalysisFactory {

public:
  /// Destructor
  virtual ~IDevAnalysisFactory() {/*nop*/;}

  /// Registers a store factory
  virtual bool registerStoreFactory( IStoreFactory* factory,
				     const std::string & storeType ) = 0;
  /// Returns the address of a store factory
  virtual IStoreFactory* storeFactory( const std::string & storeType ) = 0;

  /// Registers an annotation factory
  virtual bool registerAnnotationFactory( IAnnotationFactory* factory ) = 0;
  /// Returns the address of an annotation factory
  virtual IAnnotationFactory* annotationFactory() = 0;

  /// Registers a histogram factory provider
  virtual bool registerHistogramFactoryProvider( IHistogramFactoryProvider* provider ) = 0;
  /// Returns the address of a histogram factory provider
  virtual IHistogramFactoryProvider* histogramFactoryProvider() = 0;

  /// Registers a dataPointSet factory provider
  virtual bool registerDataPointSetFactoryProvider( IDataPointSetFactoryProvider* provider ) = 0;
  /// Returns the address of a dataPointSet factory provider
  virtual IDataPointSetFactoryProvider* dataPointSetFactoryProvider() = 0;

  /// Registers a tree factory provider
  virtual bool registerTreeFactoryProvider( ITreeFactoryProvider* provider ) = 0;
  /// Returns the address of a tree factory provider
  virtual ITreeFactoryProvider* treeFactoryProvider() = 0;

  /// Returns the address of a tuple factory provider
  virtual bool registerTupleFactoryProvider( ITupleFactoryProvider* provider ) = 0;
  virtual ITupleFactoryProvider* tupleFactoryProvider() = 0;

  /// Returns the address of a function factory provider
  virtual bool registerFunctionFactoryProvider( IFunctionFactoryProvider* provider ) = 0;
  virtual IFunctionFactoryProvider* functionFactoryProvider() = 0;

  /// Returns the address of a fit factory provider
  virtual bool registerFitFactoryProvider( IFitFactoryProvider* provider ) = 0;
  virtual IFitFactoryProvider* fitFactoryProvider() = 0;

  /// Returns the address of a plotter factory provider
  virtual bool registerPlotterFactoryProvider( IPlotterFactoryProvider* provider ) = 0;
  virtual IPlotterFactoryProvider* plotterFactoryProvider() = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVANALYSISFACTORY */
