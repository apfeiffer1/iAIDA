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
#ifndef IAIDA_VARIABLEBINAXIS
#define IAIDA_VARIABLEBINAXIS 1

// includes

#include "AIDA/IAxis.h"
#include <vector>
#include <map>

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Class defining a histogram axis with bins of variable size implementing the AIDA::IAxis interface

class VariableBinAxis : virtual public AIDA::IAxis
{
public:
  /// Constructor
  VariableBinAxis( const std::vector< double> & edges );
  /// Destructor
  ~VariableBinAxis() {/*nop*/;}

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
  /// The vector of the edges
  std::vector< double > m_binEdges;
  // : store infomation also in a map for performance reasons 
  /// The map of the upper edges and the bin index
  std::map< double, int > m_binUpperEdges;
};

  }
}

#include "VariableBinAxis.inl"

#endif /* ifndef IAIDA_VARIABLEBINAXIS */
