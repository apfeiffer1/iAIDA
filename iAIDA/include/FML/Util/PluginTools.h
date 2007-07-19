# ifndef UtilPluginTools_h__included__
# define UtilPluginTools_h__included__

/**
 * Plugin Mechanism,
 *
 * \author Jakub T. Moscicki, CERN 2002
 */

# include "FML/Util/Plugin.h"

namespace Util
{

template<class reg_t>
void list_plugin(std::ostream& os)
 {
   typedef typename reg_t::plugin_t plugin_t;

   std::vector<plugin_t*> v;

   v = reg_t::global_registry().get_vector();

   for(typename std::vector<plugin_t*>::iterator it=v.begin(); it!=v.end(); ++it)
     {
       os << (*it)->id() << std::endl;
     }
 }

}

# endif




