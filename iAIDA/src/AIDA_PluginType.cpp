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

//<<<<<< INCLUDES                                                       >>>>>>

#include "AIDA_Plugin/AIDA_PluginType.h"
#include <iostream>
#include <iomanip>

namespace AIDA_Plugin { 

  AIDA_PluginType::AIDA_PluginType ( )
  { }

  AIDA_PluginType::~AIDA_PluginType (void)
  {}

  AIDA::IPlotterFactory * AIDA_PluginType::createPlotterFactory(int, char * * , const std::string &, const std::string & ) 
  {
    std::cerr << "Cannot create a Plotter Factory since corresponing plugin is not loaded" << std::endl; 
    return 0; 
  }

}  // end namespace AIDA_Plugin
