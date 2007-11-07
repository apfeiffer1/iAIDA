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
#ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS
#define IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS 1

#include "AIDA_Dev/ITupleVariableStatistics.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_TupleVariableStatistics : virtual public AIDA::Dev::ITupleVariableStatistics {

public:
  /// Constructor
  AIDA_TupleVariableStatistics();
  /// Destructor
  ~AIDA_TupleVariableStatistics() {/*nop*/;}

  /// Methods inherited from AIDA::DEV::ITupleVariableStatistics
  bool setStatistics( double valueMin,
		      double valueMax,
		      double valueMean,
		      double valueRms,
		      int numberOfEntries );
  double min() const;
  double max() const;
  double mean() const;
  double rms() const;
  int entries() const;
  bool reset();
  bool addEntry( double value );

private:
  double   m_min;
  double   m_max;
  double   m_sumOfValues;
  double   m_sumOfValuesSquared;
  int      m_entries;

  // no copy constructor or assignment operator
  AIDA_TupleVariableStatistics( const AIDA_TupleVariableStatistics& );
  AIDA_TupleVariableStatistics& operator=( const AIDA_TupleVariableStatistics& );
};

  }
}

#include "AIDA_TupleVariableStatistics.inl"

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS */
