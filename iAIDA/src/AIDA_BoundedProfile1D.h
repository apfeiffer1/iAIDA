// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDABOUNDEDPROFILE1D
#define AIDANATIVE_AIDABOUNDEDPROFILE1D 1

#include "AIDA_Profile1D.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Profile 1D histogram with boundaries on the y axis

class AIDA_BoundedProfile1D : public AIDA_Profile1D
{

public:
  /// Constructor for fixed-sized bin profile histogram
  AIDA_BoundedProfile1D( const std::string& title,
			 int numberOfBins,
			 double lowEdge,
			 double highEdge,
			 double lowBound,
			 double highBound );
  /// Constructor for variable-sized bin profile histogram
  AIDA_BoundedProfile1D( const std::string& title,
			 const std::vector< double >& edges,
			 double lowBound,
			 double highBound );
  /// Copy constructor
  AIDA_BoundedProfile1D( const AIDA_BoundedProfile1D& h );
  /// Destructor
  virtual ~AIDA_BoundedProfile1D() {};

  /// Methods needed to be reimplemented
  virtual bool reset();
  virtual int extraEntries() const;
  virtual bool fill( double x, double y, double weight = 1. ) ;
  virtual bool add( const AIDA::IProfile1D & h );

private:
  /// extra entries
  int                m_extraEntries;

  /// Bounds on extra coordinate
  double             m_lowBound;
  double             m_highBound;

  /// No asignment operator
  AIDA_BoundedProfile1D& operator=( const AIDA_BoundedProfile1D& );
};

  }
}

#endif /* ifndef AIDANATIVE_AIDAPROFILE1D */
