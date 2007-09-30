// Emacs -*- C++ -*-
#ifndef IAIDA_AIDAPROFILE2D
#define IAIDA_AIDAPROFILE2D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevProfile2D.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class ProfileBin2D;

/**
 * Implementation of the AIDA IProfile2D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "MeanX", "RmsX", "MeanY", "RmsY", "Extra Entries"
 *
 */

class AIDA_Profile2D : public AIDA_BaseHistogram,
		       virtual public AIDA::Dev::IDevProfile2D
{

public:
  /// Constructor for fixed-sized bin histogram
  AIDA_Profile2D( const std::string& title,
		  int numberOfBinsX,
		  double lowEdgeX,
		  double highEdgeX,
		  int numberOfBinsY,
		  double lowEdgeY,
		  double highEdgeY );
  /// Constructor for variable-sized bin histogram
  AIDA_Profile2D( const std::string& title,
		  const std::vector< double >& edgesX,
		  const std::vector< double >& edgesY );
  /// Copy constructor
  AIDA_Profile2D( const AIDA_Profile2D& h );
  /// Constructor from another AIDA histogram
  AIDA_Profile2D( const AIDA::IProfile2D& h );
  /// Destructor
  virtual ~AIDA_Profile2D();
  /**
   * Methods still to be implemented from the AIDA_BaseHistogram inheritence tree :
   * (IBaseHistogram, IDevManagedObject, IManagedObject )
   */
protected:
  void updateAnnotation() const;
public:
  virtual bool reset();
  int entries() const;
  /// new from AIDA 3.2 
  void * cast(const std::string & className) const; 

  /// Methods to be implemented from IProfile
  int allEntries() const;
  virtual int extraEntries() const;
  double sumBinHeights() const;
  double sumAllBinHeights() const;
  double sumExtraBinHeights() const;
  double minBinHeight() const;
  double maxBinHeight() const;

  /// Methods to be implemented from IProfile2D
  virtual bool fill( double x, double y, double z, double weight = 1. );
  double binMeanX( int indexX, int indexY ) const;
  double binMeanY( int indexX, int indexY ) const;
  int binEntries( int indexX, int indexY ) const;
  int binEntriesX( int indexX ) const;
  int binEntriesY( int indexY ) const;
  double binHeight( int indexX, int indexY ) const;
  double binHeightX( int indexX ) const;
  double binHeightY( int indexY ) const;
  double binError( int indexX, int indexY ) const;
  double binRms( int indexX, int indexY ) const;
  double meanX() const;
  double meanY() const;
  double rmsX() const;
  double rmsY() const;
  const AIDA::IAxis& xAxis() const;
  const AIDA::IAxis& yAxis() const;
  int coordToIndexX( double coordX ) const;
  int coordToIndexY( double coordY ) const;
  virtual bool add( const AIDA::IProfile2D & h );

  /// Methods to be implemented from IDevProfile2D
  bool setBinContents( int binIndexX,
		       int binIndexY,
		       int entries,
		       double height,
		       double error,
		       double spread,
		       double centreX,
		       double centreY );

private:
  /// Private methods
  bool calculateStatistics() const;
  bool increment( const AIDA_Profile2D& h );

private:
  AIDA::IAxis*                  m_axisX;
  AIDA::IAxis*                  m_axisY;
  std::vector< std::vector< ProfileBin2D* > >  m_bins;
  mutable bool                  m_validStatistics;

  // Histogram statistics
  mutable double                m_meanX;
  mutable double                m_rmsX;
  mutable double                m_meanY;
  mutable double                m_rmsY;
  mutable int                   m_entries;
  mutable int                   m_extraEntries;
  mutable double                m_sumBinHeights;
  mutable double                m_sumBinWeights;
  mutable double                m_sumExtraBinHeights;
  mutable double                m_minHeight;
  mutable double                m_maxHeight;

  /// No asignment operator
  AIDA_Profile2D& operator=( const AIDA_Profile2D& );
};

  }
}

#endif /* ifndef IAIDA_AIDAPROFILE2D */
