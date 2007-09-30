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
