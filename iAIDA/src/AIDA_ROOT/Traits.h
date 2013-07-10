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

#ifndef AIDAROOT_Traits_H
#define AIDAROOT_Traits_H 1
#include "AIDA_ROOT/Histogram1D.h"

namespace AIDA_ROOT {

  struct HistogramTraits {
   /*
    typedef typename Traits::BaseHistogram BaseHistogram;
    typedef AIDA_ROOT::Cloud1D Cloud1D;
    typedef AIDA_ROOT::Cloud2D Cloud2D;
    typedef AIDA_ROOT::Cloud3D Cloud3D;
    */
    typedef AIDA_ROOT::Histogram1D Histogram1D;
    /*
    typedef AIDA_ROOT::Histogram2D Histogram2D;
    typedef AIDA_ROOT::Histogram3D Histogram3D;
    typedef AIDA_ROOT::Profile1D Profile1D;
    typedef AIDA_ROOT::Profile2D Profile2D;
    */
    


  };

}
#endif // AIDAROOT_Traits_H
