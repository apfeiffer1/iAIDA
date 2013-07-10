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
