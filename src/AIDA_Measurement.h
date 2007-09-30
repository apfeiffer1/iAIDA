// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_DATAPOINTSET_AIDAMEASUREMENT
#define IAIDA_AIDA_DATAPOINTSET_AIDAMEASUREMENT 1

#include "AIDA/IMeasurement.h"

/// Implementation of the AIDA IMeasurement class

namespace iAIDA {
  namespace AIDA_DataPointSet_native {

class AIDA_Measurement : virtual public AIDA::IMeasurement {
public:
  /// Constructor
  AIDA_Measurement();
  /// Destructor
  ~AIDA_Measurement() {/* nop */};

  /// Methods inherited from IMeasurement
  double value() const;
  double errorPlus() const;
  double errorMinus() const;
  bool setValue( double v );
  bool setErrorPlus( double ep );
  bool setErrorMinus( double em );

private:
  double m_value;
  double m_errorPlus;
  double m_errorMinus;
};

  }
}


/// Inline methods
#include "AIDA_Measurement.inl"

#endif /* ifndef IAIDA_AIDA_DATAPOINTSET_AIDAMEASUREMENT */
