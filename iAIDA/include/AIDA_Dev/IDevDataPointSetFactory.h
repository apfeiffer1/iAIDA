// Emacs -*- C++ -*-
#ifndef AIDA_IDEVDATAPOINTSETFACTORY
#define AIDA_IDEVDATAPOINTSETFACTORY 1

#include <string>
#include <vector>

namespace AIDA {
  class IDataPointSet;
  class IHistogram1D;
  class IHistogram2D;
  class IHistogram3D;
  class ICloud1D;
  class ICloud2D;
  class ICloud3D;
  class IProfile1D;
  class IProfile2D;

  namespace Dev {
    class IDevDataPointSet;

/// DataPointSet factory creating objects that are not attached to a tree

class IDevDataPointSetFactory {

public:
  /// Virtual destructor
  virtual ~IDevDataPointSetFactory ( ) { /*nop*/; }
  /// Creates an empty set
  virtual IDevDataPointSet * create ( const std::string& title, int dimensionOfPoint ) = 0;
  virtual IDevDataPointSet * create ( int dimensionOfPoint ) = 0;
  virtual IDevDataPointSet * createCopy ( const IDataPointSet & original ) = 0;
  /// Destroys a set
  virtual bool destroy ( IDataPointSet * dataPointSet ) = 0;

  /// Creates a data point set from a histogram
  virtual IDevDataPointSet* create ( const IHistogram1D & h, const std::string & options = "" ) = 0;
  virtual IDevDataPointSet* create ( const IHistogram2D & h, const std::string & options = "" ) = 0;
  virtual IDevDataPointSet* create ( const IHistogram3D & h, const std::string & options = "" ) = 0;
  /// Creates a data point set from a cloud
  virtual IDevDataPointSet* create ( const ICloud1D & c, const std::string & options = "" ) = 0;
  virtual IDevDataPointSet* create ( const ICloud2D & c, const std::string & options = "" ) = 0;
  virtual IDevDataPointSet* create ( const ICloud3D & c, const std::string & options = "" ) = 0;
  /// Create a data point set from a profile
  virtual IDevDataPointSet* create ( const IProfile1D & p, const std::string & options = "" ) = 0;
  virtual IDevDataPointSet* create ( const IProfile2D & p, const std::string & options = "" ) = 0;
  // Operations on the data points given two sets, producing a third.
  // strategy = "" means default (uncorrelated Gaussian error propagation)
  // Only works if n points and dimensionality identical.
  // a = b/c
  /// Point by point and measurement by measurement addition
  virtual IDevDataPointSet* add ( const IDataPointSet& a, const IDataPointSet& b, std::string strategy = "" ) = 0;
  /// Point by point subtraction
  virtual IDevDataPointSet* subtract ( const IDataPointSet& a, const IDataPointSet& b, std::string strategy = "" ) = 0;
  /// Point by point and measurement by measurement multiplication
  virtual IDevDataPointSet* multiply ( const IDataPointSet& a, const IDataPointSet& b, std::string strategy = "" ) = 0;
  /// Point by point and measurement by measurement division
  virtual IDevDataPointSet* divide ( const IDataPointSet& a, const IDataPointSet& b, std::string strategy = "" ) = 0;
  /// Point by point and measurement by measurement calculation of the weighted means
  virtual IDevDataPointSet* weightedMean ( const IDataPointSet& a, const IDataPointSet& b, std::string strategy = "" ) = 0;
  /// new method from AIDA 3.2  (only needed ones) 
  virtual IDevDataPointSet* createY(const std::string & title, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym) = 0;
  virtual IDevDataPointSet* createX(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm) = 0;
  virtual IDevDataPointSet* createXY(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym) = 0;
  virtual IDevDataPointSet* createXYZ(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm) = 0;

};  


  }
}

#endif /* ifndef AIDA_IDEVDATAPOINTSETFACTORY */
