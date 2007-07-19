#ifndef EDGECALCULATOR_H
#define EDGECALCULATOR_H

namespace iAIDA {
  namespace AIDA_Histogram_native {
    class EdgeCalculator
    {
    public:
      EdgeCalculator() {}
      ~EdgeCalculator() {}
      bool calculateEdges( double edgeLow,
			   double edgeHigh,
			   int bins,
			   double& newEdgeLow,
			   double& newEdgeHigh ) const;
    };
  }
}

static const iAIDA::AIDA_Histogram_native::EdgeCalculator edgeCalculator;

#endif
