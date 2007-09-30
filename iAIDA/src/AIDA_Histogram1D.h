// Emacs -*- C++ -*-
#ifndef IAIDA_AIDAHISTOGRAM1D
#define IAIDA_AIDAHISTOGRAM1D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class GravityBin1D;

/**
 * Implementation of the AIDA IHistogram1D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "Mean", "Rms", "Extra Entries", "Overflow", "Underflow"
 *
 */

class AIDA_Histogram1D : public AIDA_BaseHistogram,
			 virtual public AIDA::Dev::IDevHistogram1D
{

public:
  /// Constructor for fixed-sized bin histogram
  AIDA_Histogram1D( const std::string& title,
		    int numberOfBins,
		    double lowEdge,
		    double highEdge );
  /// Constructor for variable-sized bin histogram
  AIDA_Histogram1D( const std::string& title,
		    const std::vector< double >& edges );
  /// Copy constructor
  AIDA_Histogram1D( const AIDA_Histogram1D& h );
  /// Constructor from another AIDA histogram
  AIDA_Histogram1D( const AIDA::IHistogram1D& h );
  /// Destructor
  ~AIDA_Histogram1D();
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

  /// Methods to be implemented from IHistogram1D
  bool fill( double x, double weight = 1. ) ;
  double binMean( int index ) const;
  int binEntries( int index ) const;
  double binHeight( int index ) const;
  double binError( int index ) const;
  double mean() const;
  double rms() const;
  const AIDA::IAxis& axis() const;
  int coordToIndex( double coord ) const;
  bool add( const AIDA::IHistogram1D & h );  

  /// Methods to be implemented from IDevHistogram1D
  bool setBinContents( int binIndex,
		       int entries,
		       double height,
		       double error,
		       double centre );
  bool setRms( double rms );

private:
  /// Private methods
  bool calculateStatistics() const;
  bool increment( const AIDA_Histogram1D& h );

private:
  AIDA::IAxis*                  m_axis;
  std::vector< GravityBin1D* >  m_bins;
  mutable bool                  m_validStatistics;

  // Histogram statistics
  mutable double                m_mean;
  mutable double                m_rms;
  mutable int                   m_entries;
  mutable int                   m_extraEntries;
  mutable double                m_ebe;
  mutable double                m_sumBinHeights;
  mutable double                m_sumExtraBinHeights;
  mutable double                m_minHeight;
  mutable double                m_maxHeight;
  double                        m_sumWeightTimesSquaredX;

  /// No asignment operator
  AIDA_Histogram1D& operator=( const AIDA_Histogram1D& );
};

  }
}

#endif /* ifndef IAIDA_AIDAHISTOGRAM1D */
