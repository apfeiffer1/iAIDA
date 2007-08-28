#ifndef FML_MINIMIZERPLUGINTYPES_H
#define FML_MINIMIZERPLUGINTYPES_H

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

# include "FML/Minimizer.h"

namespace fml
{
  // namespace FML
  // {

typedef Util::AbstractPlugin<Minimizer,std::string> MinimizerPlugin;

typedef Util::PluginRegistry<Minimizer,std::string> MinimizerRegistry;

  // }
}

#endif
