// Emacs -*- C++ -*-
#ifndef AIDANATIVE_VARIABLEBINAXIS
#define AIDANATIVE_VARIABLEBINAXIS 1

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

#endif /* ifndef AIDANATIVE_VARIABLEBINAXIS */
