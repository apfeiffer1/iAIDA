// Emacs -*- C++ -*-
#ifndef AIDA_ITUPLEVARIABLESTATISTICS
#define AIDA_ITUPLEVARIABLESTATISTICS 1

namespace AIDA {
  namespace Dev {

/// Abstract interface class for the statistics of a tuple variable

class ITupleVariableStatistics {

public:
  /// Destructor
  virtual ~ITupleVariableStatistics() {/*nop*/;}

  /// Sets/Retrieves globally the statistical values for this variable
  virtual bool setStatistics( double valueMin,
			      double valueMax,
			      double valueMean,
			      double valueRms,
                              int numberOfEntries ) = 0;
  virtual double min() const = 0;
  virtual double max() const = 0;
  virtual double mean() const = 0;
  virtual double rms() const = 0;
  virtual int entries() const = 0;

  /// Resets the statistics
  virtual bool reset() = 0;

  /// Adds an entry
  virtual bool addEntry( double value ) = 0;
};

  }
}

#endif /* ifndef AIDA_ITUPLEVARIABLESTATISTICS */
