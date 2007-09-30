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

#ifndef FML_FUNCTIONCATALOGUE_H
#define FML_FUNCTIONCATALOGUE_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include "FML/Util/PluginFinder.h"
# include "FML/FunctionPlugin.h"

# include "FML/IFMLFunction.h"

# include <memory>

/**
 * \file Function Catalogue
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2002
 * \simplified version by L.Moneta, CERN/PH-SFT 2004
 */

namespace fml
{
  // namespace FML
  // {

    /**
     * Catalogue mantains and allows to create functions 
     * present in the libraries (internal function registry)  
     */
    class FunctionCatalogue
    {
    public:



      /**
       * Create function from a string identifier.
       *
       * If the identifier is an algebraic expression ("E+G") look up for the 
       * implementation of the expression components in the internal function
       * registry. 
       */
      static std::auto_ptr<IFMLFunction> createFunction(std::string nameId);

      /**
       * List all available functions (registred).
       */
      static std::vector<std::string> listAvailableFunctions();


    };

  // }
}

#endif
