// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDAHISTOGRAM2D
#define AIDANATIVE_AIDAHISTOGRAM2D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class GravityBin2D;

/**
 * Implementation of the AIDA IHistogram2D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "MeanX", "RmsX", "MeanY", "RmsY", "Extra Entries"
 *
 */

class AIDA_Histogram2D : public AIDA_BaseHistogram,
			 virtual public AIDA::Dev::IDevHistogram2D
{

public:
  /// Constructor for fixed-sized bin histogram
  AIDA_Histogram2D( const std::string& title,
		    int numberOfBinsX,
		    double lowEdgeX,
		    double highEdgeX,
		    int numberOfBinsY,
		    double lowEdgeY,
		    double highEdgeY );
  /// Constructor for variable-sized bin histogram
  AIDA_Histogram2D( const std::string& title,
		    const std::vector< double >& edgesX,
		    const std::vector< double >& edgesY );
  /// Copy constructor
  AIDA_Histogram2D( const AIDA_Histogram2D& h );
  /// Constructor from another AIDA histogram
  AIDA_Histogram2D( const AIDA::IHistogram2D& h );
  /// Destructor
  ~AIDA_Histogram2D();
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

  /// Methods to be implemented from IHistogram
  int allEntries() const;
  int extraEntries() const;
  double equivalentBinEntries() const;
  double sumBinHeights() const;
  double sumAllBinHeights() const;
  double sumExtraBinHeights() const;
  double minBinHeight() const;
  double maxBinHeight() const;
  bool scale( double scaleFactor );

  /// Methods to be implemented from IHistogram2D
  bool fill( double x, double y, double weight = 1. );
  double binMeanX( int indexX, int indexY ) const;
  double binMeanY( int indexX, int indexY ) const;
  int binEntries( int indexX, int indexY ) const;
  int binEntriesX( int indexX ) const;
  int binEntriesY( int indexY ) const;
  double binHeight( int indexX, int indexY ) const;
  double binHeightX( int indexX ) const;
  double binHeightY( int indexY ) const;
  double binError( int indexX, int indexY ) const;
  double meanX() const;
  double meanY() const;
  double rmsX() const;
  double rmsY() const;
  const AIDA::IAxis& xAxis() const;
  const AIDA::IAxis& yAxis() const;
  int coordToIndexX( double coordX ) const;
  int coordToIndexY( double coordY ) const;
  bool add( const AIDA::IHistogram2D & h );

  /// Methods to be implemented from IDevHistogram2D
  bool setBinContents( int binIndexX,
		       int binIndexY,
		       int entries,
		       double height,
		       double error,
		       double centreX,
		       double centreY );
  bool setRms( double rmsX, double rmsY );

private:
  /// Private methods
  bool calculateStatistics() const;
  bool increment( const AIDA_Histogram2D& h );

private:
  AIDA::IAxis*                  m_axisX;
  AIDA::IAxis*                  m_axisY;
  std::vector< std::vector< GravityBin2D* > >  m_bins;
  mutable bool                  m_validStatistics;

  // Histogram statistics
  mutable double                m_meanX;
  mutable double                m_rmsX;
  mutable double                m_meanY;
  mutable double                m_rmsY;
  mutable int                   m_entries;
  mutable int                   m_extraEntries;
  mutable double                m_ebe;
  mutable double                m_sumBinHeights;
  mutable double                m_sumExtraBinHeights;
  mutable double                m_minHeight;
  mutable double                m_maxHeight;
  double                        m_sumWeightTimesSquaredX;
  double                        m_sumWeightTimesSquaredY;

  /// No asignment operator
  AIDA_Histogram2D& operator=( const AIDA_Histogram2D& );
};

  }
}

#endif /* ifndef AIDANATIVE_AIDAHISTOGRAM2D */
