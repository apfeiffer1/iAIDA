#ifndef AIDAROOT_HISTOGRAMFACTORY_H
#define AIDAROOT_HISTOGRAMFACTORY_H 1
#include "AIDA_ROOT/HistogramTraits.h"
#include "AIDA_CPP/HistogramFactory.h"

namespace AIDA_ROOT {

  struct HistogramFactory : 
    public AIDA_CPP::HistogramFactory<AIDA_ROOT::HistogramTraits> {
  };
}

#endif // AIDAROOT_HISTOGRAMFACTORY_H
