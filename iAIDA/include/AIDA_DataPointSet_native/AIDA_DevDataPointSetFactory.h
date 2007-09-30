// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_DATAPOINTSET_AIDADEVDATAPOINTSETFACTORY
#define IAIDA_AIDA_DATAPOINTSET_AIDADEVDATAPOINTSETFACTORY 1

#include "AIDA_Dev/IDevDataPointSetFactory.h"

namespace AIDA {
  class IAnnotation;
}

namespace iAIDA {
  namespace AIDA_DataPointSet_native {

/// Implementation of the AIDA::Dev::IevDataPointSetFactory interface

class AIDA_DevDataPointSetFactory : virtual public AIDA::Dev::IDevDataPointSetFactory
{
public:
  /// Constructor
  AIDA_DevDataPointSetFactory() { /*nop*/; }
  /// Destructor
  ~AIDA_DevDataPointSetFactory() { /*nop*/; }

  /// Creates an empty set
  AIDA::Dev::IDevDataPointSet* create( const std::string& title, int dimensionOfPoint );
  AIDA::Dev::IDevDataPointSet* create( int dimensionOfPoint );
  AIDA::Dev::IDevDataPointSet* createCopy( const AIDA::IDataPointSet & original );
  /// Destroys a set
  bool destroy ( AIDA::IDataPointSet * dataPointSet );

  /// Creates a data point set from a histogram
  AIDA::Dev::IDevDataPointSet* create( const AIDA::IHistogram1D & h, const std::string & options = "" );
  AIDA::Dev::IDevDataPointSet* create( const AIDA::IHistogram2D & h, const std::string & options = "" );
  AIDA::Dev::IDevDataPointSet* create( const AIDA::IHistogram3D & h, const std::string & options = "" );
  /// Creates a data point set from a cloud
  AIDA::Dev::IDevDataPointSet* create( const AIDA::ICloud1D & c, const std::string & options = "" );
  AIDA::Dev::IDevDataPointSet* create( const AIDA::ICloud2D & c, const std::string & options = "" );
  AIDA::Dev::IDevDataPointSet* create( const AIDA::ICloud3D & c, const std::string & options = "" );
  /// Create a data point set from a profile
  AIDA::Dev::IDevDataPointSet* create( const AIDA::IProfile1D & p, const std::string & options = "" );
  AIDA::Dev::IDevDataPointSet* create( const AIDA::IProfile2D & p, const std::string & options = "" );
  // Operations on the data points given two sets, producing a third.
  // strategy = "" means default (uncorrelated Gaussian error propagation)
  // Only works if n points and dimensionality identical.
  // a = b/c
  /// Point by point and measurement by measurement addition
  AIDA::Dev::IDevDataPointSet* add( const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b, std::string strategy = "" );
  /// Point by point subtraction
  AIDA::Dev::IDevDataPointSet* subtract( const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b, std::string strategy = "" );
  /// Point by point and measurement by measurement multiplication
  AIDA::Dev::IDevDataPointSet* multiply( const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b, std::string strategy = "" );
  /// Point by point and measurement by measurement division
  AIDA::Dev::IDevDataPointSet* divide( const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b, std::string strategy = "" );
  /// Point by point and measurement by measurement calculation of the weighted means
  AIDA::Dev::IDevDataPointSet* weightedMean( const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b, std::string strategy = "" );

  /// new method from AIDA 3.2  (only needed ones) 
  AIDA::Dev::IDevDataPointSet* createY(const std::string & title, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym);
  AIDA::Dev::IDevDataPointSet* createX(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm);
  AIDA::Dev::IDevDataPointSet* createXY(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym);
  AIDA::Dev::IDevDataPointSet* createXYZ(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm);

private:
  AIDA::IAnnotation* createAnnotation();
};

  }
}

#endif /* ifndef IAIDA_AIDA_DATAPOINTSET_AIDADEVDATAPOINTSETFACTORY */
