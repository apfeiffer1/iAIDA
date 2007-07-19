// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDAPROFILE1D
#define AIDANATIVE_AIDAPROFILE1D 1

#include "AIDA_BaseHistogram.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class ProfileBin1D;

/**
 * Implementation of the AIDA IProfile1D class
 *
 * The Additional annotation items
 * defined on top of those from the AIDA_BaseHistogram class are:
 *
 *  "Mean", "Rms", "Extra Entries", "Overflow", "Underflow"
 *
 */

class AIDA_Profile1D : public AIDA_BaseHistogram,
		       virtual public AIDA::Dev::IDevProfile1D
{

public:
  /// Constructor for fixed-sized bin profile histogram
  AIDA_Profile1D( const std::string& title,
		  int numberOfBins,
		  double lowEdge,
		  double highEdge );
  /// Constructor for variable-sized bin profile histogram
  AIDA_Profile1D( const std::string& title,
		  const std::vector< double >& edges );
  /// Copy constructor
  AIDA_Profile1D( const AIDA_Profile1D& h );
  /// Constructor from another AIDA profile histogram
  AIDA_Profile1D( const AIDA::IProfile1D& h );
  /// Destructor
  virtual ~AIDA_Profile1D();
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

  /// Methods to be implemented from IProfile1D
  virtual bool fill( double x, double y, double weight = 1. ) ;
  double binMean( int index ) const;
  int binEntries( int index ) const;
  double binHeight( int index ) const;
  double binError( int index ) const;
  double binRms( int index ) const;
  double mean() const;
  double rms() const;
  const AIDA::IAxis& axis() const;
  int coordToIndex( double coord ) const;
  virtual bool add( const AIDA::IProfile1D & h );

  /// Methods to be implemented from IDevProfile1D
  bool setBinContents( int binIndex,
		       int entries,
		       double height,
		       double error,
		       double spread,
                       double centre );

private:
  /// Private methods
  bool calculateStatistics() const;
  bool increment( const AIDA_Profile1D& h );

private:
  AIDA::IAxis*                  m_axis;
  std::vector< ProfileBin1D* >  m_bins;
  mutable bool                  m_validStatistics;

  // Histogram statistics
  mutable double                m_mean;
  mutable double                m_rms;
  mutable int                   m_entries;
  mutable int                   m_extraEntries;
  mutable double                m_sumBinHeights;
  mutable double                m_sumExtraBinHeights;
  mutable double                m_sumBinWeights;
  mutable double                m_minHeight;
  mutable double                m_maxHeight;

  /// No asignment operator
  AIDA_Profile1D& operator=( const AIDA_Profile1D& );
};

  }
}

#endif /* ifndef AIDANATIVE_AIDAPROFILE1D */
