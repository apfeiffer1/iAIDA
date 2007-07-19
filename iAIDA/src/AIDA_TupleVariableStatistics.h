// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS
#define AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS 1

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

#endif /* ifndef AIDANATIVE_AIDA_TUPLE_AIDA_TUPLEVARIABLESTATISTICS */
