//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

// Emacs -*- C++ -*-
#ifndef IAIDA_AIDACLOUD2D
#define IAIDA_AIDACLOUD2D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include <vector>

// Forward declatations
namespace AIDA {
  class IHistogram2D;
}

namespace iAIDA {
  namespace AIDA_Histogram_native {

/**
 * Implementation of the AIDA ICloud2D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "MeanX", "RmsX", "MeanY", "RmsY"
 *
 */

class AIDA_Cloud2D : public AIDA_BaseHistogram,
		     virtual public AIDA::Dev::IDevCloud2D
{
public:
  /// The default cache size
  static const unsigned int defaultCacheSize = 10000;
  /// The default number of bins for the supporting histogram
  static const unsigned int defaultNumberOfBins = 50;

  /// Constructor
  AIDA_Cloud2D( const std::string& title  );
  /// Copy constructor
  AIDA_Cloud2D( const AIDA_Cloud2D& h );
  /// Constructor from another AIDA histogram
  AIDA_Cloud2D( const AIDA::ICloud2D& h );
  /// Destructor
  ~AIDA_Cloud2D();
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

  /// Methods to be implemented from ICloud2D
  bool fill( double x, double y, double weight = 1. );
  double lowerEdgeX() const;
  double lowerEdgeY() const;
  double upperEdgeX() const;
  double upperEdgeY() const;
  double valueX( int index ) const;
  double valueY( int index ) const;
  double weight( int index ) const;
  double meanX() const;
  double meanY() const;
  double rmsX() const;
  double rmsY() const;
  bool convert( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  bool convert( const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY );
  const AIDA::IHistogram2D & histogram() const;
  bool fillHistogram(  AIDA::IHistogram2D & hist ) const;

  /// Methods to be implemented from IDevCloud2D
  bool setCacheSize( unsigned int newCacheSize );
  /// Retrieves the cache size
  unsigned int cacheSize() const;
  

private:
  /// A cloud element consists of three values : x, y, w
  class Cloud2DElement {
  public:
    Cloud2DElement( double newX = 0,
		    double newY = 0,
		    double newW = 1.0 ):
      x( newX ), y( newY ), w( newW ) {}
    ~Cloud2DElement(){}
    double x;
    double y;
    double w;
  };

  /// The cache size
  unsigned int                   m_cacheSize;
  /// The vector holding the cloud elements
  std::vector< Cloud2DElement >  m_elements;
  /// The running sum of weights
  double                         m_sumOfWeights;
  /// The running sum of squared weights
  double                         m_sumOfWeightTimesValueX;
  double                         m_sumOfWeightTimesValueY;
  /// The running sum of the weight times the value
  double                         m_sumOfWeightTimesSquaredValueX;
  double                         m_sumOfWeightTimesSquaredValueY;
  /// The low edge
  double                         m_lowEdgeX;
  double                         m_lowEdgeY;
  /// The high edge
  double                         m_highEdgeX;
  double                         m_highEdgeY;
  /// The supporting histogram
  AIDA::IHistogram2D*            m_histo;

  /// No asignment operator
  AIDA_Cloud2D& operator=( const AIDA_Cloud2D& );
};

  }
}

#endif /* ifndef IAIDA_AIDACLOUD2D */
