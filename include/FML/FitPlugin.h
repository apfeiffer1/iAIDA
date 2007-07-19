#ifndef FML_FITPLUGINTYPES_H
#define FML_FITPLUGINTYPES_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include "FML/Util/Plugin.h"
# include "FML/Util/PluginFinder.h"

# include "FML/FitMethod.h"

namespace fml
{
  // namespace FML
  // {

typedef Util::AbstractPlugin<FitMethodFunction,std::string> FitMethodPlugin;
    
typedef Util::PluginRegistry<FitMethodFunction,std::string> FitMethodRegistry;

  // }
}

#endif
