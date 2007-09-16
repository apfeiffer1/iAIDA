#ifndef AIDAROOT_HISTOGRAMTraits_H
#define AIDAROOT_HISTOGRAMTraits_H 1
#include "AIDA_ROOT/Histogram1D.h"
#include "AIDA_ROOT/Histogram2D.h"
#include "AIDA_ROOT/Histogram3D.h"
#include "AIDA_ROOT/Profile1D.h"
#include "AIDA_ROOT/Profile2D.h"

namespace AIDA_ROOT {

  struct HistogramTraits {
   /*
    typedef typename Traits::BaseHistogram BaseHistogram;
    typedef AIDA_ROOT::Cloud1D Cloud1D;
    typedef AIDA_ROOT::Cloud2D Cloud2D;
    typedef AIDA_ROOT::Cloud3D Cloud3D;
    */
    typedef AIDA_ROOT::Histogram1D Histogram1D;
    typedef AIDA_ROOT::Histogram2D Histogram2D;
    typedef AIDA_ROOT::Histogram3D Histogram3D;
    typedef AIDA_ROOT::Profile1D Profile1D;
    typedef AIDA_ROOT::Profile2D Profile2D;
    


  };

}
#endif // AIDAROOT_HISTOGRAMTraits_H
