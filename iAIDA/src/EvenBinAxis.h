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
#ifndef IAIDA_EVENBINAXIS
#define IAIDA_EVENBINAXIS 1

// includes

#include "AIDA/IAxis.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Class defining a histogram axis with bins of the same size implementing the AIDA::IAxis interface

class EvenBinAxis : virtual public AIDA::IAxis
{
public:
  /// Constructor
  EvenBinAxis( int nBins,
	       double lowEdge,
	       double highEdge );
  /// Destructor
  ~EvenBinAxis() {/*nop*/;}

  /// Methods inherited from AIDA::IAxis
  bool isFixedBinning() const;
  double lowerEdge() const;
  double upperEdge() const;
  int bins() const;
  double binLowerEdge( int index ) const;
  double binUpperEdge( int index ) const;
  double binWidth( int index ) const;
  int coordToIndex( double coord ) const;

private:
  int m_bins;
  double m_lowEdge;
  double m_highEdge;
  double m_binSize;
};

  }
}

#include "EvenBinAxis.inl"

#endif /* ifndef IAIDA_EVENBINAXIS */
