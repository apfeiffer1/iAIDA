// Emacs -*- C++ -*-
#ifndef IAIDA_AIDACLOUD1D
#define IAIDA_AIDACLOUD1D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevCloud1D.h"
#include <vector>

// Forward declatations
namespace AIDA {
  class IHistogram1D;
}

namespace iAIDA {
  namespace AIDA_Histogram_native {

/**
 * Implementation of the AIDA ICloud1D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "Mean", "Rms"
 *
 */

class AIDA_Cloud1D : public AIDA_BaseHistogram,
		     virtual public AIDA::Dev::IDevCloud1D
{
public:
  /// The default cache size
  static const unsigned int defaultCacheSize = 10000;
  /// The default number of bins for the supporting histogram
  static const unsigned int defaultNumberOfBins = 100;

  /// Constructor
  AIDA_Cloud1D( const std::string& title  );
  /// Copy constructor
  AIDA_Cloud1D( const AIDA_Cloud1D& h );
  /// Constructor from another AIDA histogram
  AIDA_Cloud1D( const AIDA::ICloud1D& h );
  /// Destructor
  ~AIDA_Cloud1D();
  /**
   * Methods still to be implemented from the AIDA_BaseHistogram inheritence tree :
   * (IBaseHistogram, IDevManagedObject, IManagedObject )
   */
protected:
  void updateAnnotation() const;
public:
  bool reset();
  int entries() const;
  /// new from AIDA 3.2 
  void * cast(const std::string & className) const; 

  /// Methods to be implemented from ICloud
  double sumOfWeights() const;
  bool convertToHistogram();
  bool isConverted() const;
  int maxEntries() const;
  bool scale( double scaleFactor );

  /// Methods to be implemented from ICloud1D
  bool fill( double x, double weight = 1. );
  double lowerEdge() const;
  double upperEdge() const;
  double value( int index ) const;
  double weight( int index ) const;
  double mean() const;
  double rms() const;
  bool convert( int nBins, double lowerEdge, double upperEdge );
  bool convert( const std::vector< double >& binEdge );
  const AIDA::IHistogram1D & histogram() const;
  bool fillHistogram( AIDA::IHistogram1D& hist ) const;

  /// Methods to be implemented from IDevCloud1D
  bool setCacheSize( unsigned int newCacheSize );
  /// Retrieves the cache size
  unsigned int cacheSize() const;
  

private:
  /// A cloud element consists of two values : x, w
  class Cloud1DElement {
  public:
    Cloud1DElement( double newX = 0,
		    double newW = 1.0 ):
      x( newX ), w( newW ) {}
    ~Cloud1DElement(){}
    double x;
    double w;
  };

  /// The cache size
  unsigned int                   m_cacheSize;
  /// The vector holding the cloud elements
  std::vector< Cloud1DElement >  m_elements;
  /// The running sum of weights
  double                         m_sumOfWeights;
  /// The running sum of squared weights
  double                         m_sumOfWeightTimesValue;
  /// The running sum of the weight times the value
  double                         m_sumOfWeightTimesSquaredValue;
  /// The low edge
  double                         m_lowEdge;
  /// The high edge
  double                         m_highEdge;
  /// The supporting histogram
  AIDA::IHistogram1D*            m_histo;

  /// No asignment operator
  AIDA_Cloud1D& operator=( const AIDA_Cloud1D& );
};

  }
}

#endif /* ifndef IAIDA_AIDACLOUD1D */
