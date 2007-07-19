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
