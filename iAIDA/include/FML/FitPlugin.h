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
