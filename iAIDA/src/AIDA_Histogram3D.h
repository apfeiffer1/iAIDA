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
#ifndef IAIDA_AIDAHISTOGRAM3D
#define IAIDA_AIDAHISTOGRAM3D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class GravityBin3D;

/**
 * Implementation of the AIDA IHistogram3D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "MeanX", "RmsX", "MeanY", "RmsY", "MeanZ", "RmsZ", "Extra Entries"
 *
 */

class AIDA_Histogram3D : public AIDA_BaseHistogram,
			 virtual public AIDA::Dev::IDevHistogram3D
{

public:
  /// Constructor for fixed-sized bin histogram
  AIDA_Histogram3D( const std::string& title,
		    int numberOfBinsX,
		    double lowEdgeX,
		    double highEdgeX,
		    int numberOfBinsY,
		    double lowEdgeY,
		    double highEdgeY,
		    int numberOfBinsZ,
		    double lowEdgeZ,
		    double highEdgeZ );
  /// Constructor for variable-sized bin histogram
  AIDA_Histogram3D( const std::string& title,
		    const std::vector< double >& edgesX,
		    const std::vector< double >& edgesY,
		    const std::vector< double >& edgesZ );
  /// Copy constructor
  AIDA_Histogram3D( const AIDA_Histogram3D& h );
  /// Constructor from another AIDA histogram
  AIDA_Histogram3D( const AIDA::IHistogram3D& h );
  /// Destructor
  ~AIDA_Histogram3D();
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

  /// Methods to be implemented from IHistogram3D
  bool fill( double x, double y, double z, double weight = 1. );
  double binMeanX( int indexX, int indexY, int indexZ ) const;
  double binMeanY( int indexX, int indexY, int indexZ ) const;
  double binMeanZ( int indexX, int indexY, int indexZ ) const;
  int binEntries( int indexX, int indexY, int indexZ ) const;
  int binEntriesX( int indexX ) const;
  int binEntriesY( int indexY ) const;
  int binEntriesZ( int indexZ ) const;
  double binHeight( int indexX, int indexY, int indexZ ) const;
  double binHeightX( int indexX ) const;
  double binHeightY( int indexY ) const;
  double binHeightZ( int indexZ ) const;
  double binError( int indexX, int indexY, int indexZ ) const;
  double meanX() const;
  double meanY() const;
  double meanZ() const;
  double rmsX() const;
  double rmsY() const;
  double rmsZ() const;
  const AIDA::IAxis& xAxis() const;
  const AIDA::IAxis& yAxis() const;
  const AIDA::IAxis& zAxis() const;
  int coordToIndexX( double coordX ) const;
  int coordToIndexY( double coordY ) const;
  int coordToIndexZ( double coordZ ) const;
  bool add( const AIDA::IHistogram3D & h );

  /// Methods to be implemented from IDevHistogram3D
  bool setBinContents( int binIndexX,
		       int binIndexY,
		       int binIndexZ,
		       int entries,
		       double height,
		       double error,
		       double centreX,
		       double centreY,
		       double centreZ );
  bool setRms( double rmsX, double rmsY, double rmsZ );

private:
  /// Private methods
  bool calculateStatistics() const;
  bool increment( const AIDA_Histogram3D& h );

private:
  AIDA::IAxis*                  m_axisX;
  AIDA::IAxis*                  m_axisY;
  AIDA::IAxis*                  m_axisZ;
  std::vector< std::vector< std::vector< GravityBin3D* > > > m_bins;
  mutable bool                  m_validStatistics;

  // Histogram statistics
  mutable double                m_meanX;
  mutable double                m_rmsX;
  mutable double                m_meanY;
  mutable double                m_rmsY;
  mutable double                m_meanZ;
  mutable double                m_rmsZ;
  mutable int                   m_entries;
  mutable int                   m_extraEntries;
  mutable double                m_ebe;
  mutable double                m_sumBinHeights;
  mutable double                m_sumExtraBinHeights;
  mutable double                m_minHeight;
  mutable double                m_maxHeight;
  double                        m_sumWeightTimesSquaredX;
  double                        m_sumWeightTimesSquaredY;
  double                        m_sumWeightTimesSquaredZ;

  /// No asignment operator
  AIDA_Histogram3D& operator=( const AIDA_Histogram3D& );
};

  }
}

#endif /* ifndef IAIDA_AIDAHISTOGRAM3D */
