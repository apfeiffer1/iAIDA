// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINT
#define IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINT 1

#include "AIDA/IDataPoint.h"

// forward declarations
namespace AIDA {
  class IMeasurement;
}

namespace iAIDA {
  namespace AIDA_DataPointSet_native {

/// Implementation of the AIDA IDataPoint class

class AIDA_DataPoint : virtual public AIDA::IDataPoint
{
public:
  /// Constructor given the dimension
  AIDA_DataPoint( int dimension );
  /// Copy constructor
  AIDA_DataPoint( const AIDA_DataPoint& dp );
  /// Constructor from another AIDA IDataPoint
  AIDA_DataPoint( const AIDA::IDataPoint& dp );
  /// Assignment operators
  AIDA_DataPoint& operator=( const AIDA_DataPoint& dp );
  AIDA::IDataPoint& operator=( const AIDA::IDataPoint& dp );

  /// Destructor
  ~AIDA_DataPoint();

  /// Methods inherited from IDataPoint
  int dimension() const;
  AIDA::IMeasurement* coordinate( int c );
  const AIDA::IMeasurement* coordinate( int c ) const;

private:
  int                        m_dimension;
  AIDA::IMeasurement**       m_measurements;
};

  }
}

// inline methods
#include "AIDA_DataPoint.inl"

#endif /* ifndef IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINT */
