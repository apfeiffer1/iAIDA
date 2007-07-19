// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDACLOUD3D
#define AIDANATIVE_AIDACLOUD3D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include <vector>

// Forward declatations
namespace AIDA {
  class IHistogram3D;
}

namespace iAIDA {
  namespace AIDA_Histogram_native {

/**
 * Implementation of the AIDA ICloud3D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "MeanX", "RmsX", "MeanY", "RmsY", "MeanZ", "RmsZ"
 *
 */

class AIDA_Cloud3D : public AIDA_BaseHistogram,
		     virtual public AIDA::Dev::IDevCloud3D
{
public:
  /// The default cache size
  static const unsigned int defaultCacheSize = 10000;
  /// The default number of bins for the supporting histogram
  static const unsigned int defaultNumberOfBins = 25;

  /// Constructor
  AIDA_Cloud3D( const std::string& title  );
  /// Copy constructor
  AIDA_Cloud3D( const AIDA_Cloud3D& h );
  /// Constructor from another AIDA histogram
  AIDA_Cloud3D( const AIDA::ICloud3D& h );
  /// Destructor
  ~AIDA_Cloud3D();
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

  /// Methods to be implemented from ICloud3D
  bool fill( double x, double y,  double z, double weight = 1. );
  double lowerEdgeX() const;
  double lowerEdgeY() const;
  double lowerEdgeZ() const;
  double upperEdgeX() const;
  double upperEdgeY() const;
  double upperEdgeZ() const;
  double valueX( int index ) const;
  double valueY( int index ) const;
  double valueZ( int index ) const;
  double weight( int index ) const;
  double meanX() const;
  double meanY() const;
  double meanZ() const;
  double rmsX() const;
  double rmsY() const;
  double rmsZ() const;
  bool convert( int nBinsX, double lowerEdgeX, double upperEdgeX,
                int nBinsY, double lowerEdgeY, double upperEdgeY,
                int nBinsZ, double lowerEdgeZ, double upperEdgeZ );
  bool convert( const std::vector< double >& binEdgeX,
		const std::vector< double >& binEdgeY,
		const std::vector< double >& binEdgeZ );
  const AIDA::IHistogram3D & histogram() const;
  bool fillHistogram(  AIDA::IHistogram3D & hist ) const;

  /// Methods to be implemented from IDevCloud3D
  bool setCacheSize( unsigned int newCacheSize );
  /// Retrieves the cache size
  unsigned int cacheSize() const;
  

private:
  /// A cloud element consists of four values : x, y, z, w
  class Cloud3DElement {
  public:
    Cloud3DElement( double newX = 0,
		    double newY = 0,
		    double newZ = 0,
		    double newW = 1.0 ):
      x( newX ), y( newY ), z( newZ ), w( newW ) {}
    ~Cloud3DElement(){}
    double x;
    double y;
    double z;
    double w;
  };

  /// The cache size
  unsigned int                   m_cacheSize;
  /// The vector holding the cloud elements
  std::vector< Cloud3DElement >  m_elements;
  /// The running sum of weights
  double                         m_sumOfWeights;
  /// The running sum of squared weights
  double                         m_sumOfWeightTimesValueX;
  double                         m_sumOfWeightTimesValueY;
  double                         m_sumOfWeightTimesValueZ;
  /// The running sum of the weight times the value
  double                         m_sumOfWeightTimesSquaredValueX;
  double                         m_sumOfWeightTimesSquaredValueY;
  double                         m_sumOfWeightTimesSquaredValueZ;
  /// The low edge
  double                         m_lowEdgeX;
  double                         m_lowEdgeY;
  double                         m_lowEdgeZ;
  /// The high edge
  double                         m_highEdgeX;
  double                         m_highEdgeY;
  double                         m_highEdgeZ;
  /// The supporting histogram
  AIDA::IHistogram3D*            m_histo;

  /// No asignment operator
  AIDA_Cloud3D& operator=( const AIDA_Cloud3D& );
};

  }
}

#endif /* ifndef AIDANATIVE_AIDACLOUD3D */
