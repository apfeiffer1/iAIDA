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

#ifndef FML_FUNCTIONPLUGINTYPES_H
#define FML_FUNCTIONPLUGINTYPES_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include "FML/IFMLFunction.h"

# include "FML/Util/Plugin.h"
# include "FML/Util/PluginFinder.h"

namespace fml
{
// namespace FML
// {

typedef Util::AbstractPlugin<IFMLFunction,std::string> FunctionPlugin;

typedef Util::PluginRegistry<IFMLFunction,std::string> FunctionRegistry;

/// Helper typedef.
typedef Util::PluginLoader<IFMLFunction> FunctionPluginLoader;


// helper functions, probably of no interest to the public

/**
 * return plugin id string corresponding to the nameId (possibly matched
 * and transformed), return "" if no name match
 */
std::string matchAndQueryFunctionPluginLocator(std::string nameId, const FunctionPluginLoader::Locator& locator );

/** 
 * return plugin if nameId found or matched in the current content of
 * function registry.
 */
FunctionPlugin * matchAndQueryFunctionRegistry(std::string nameId);

// }
}

#endif
