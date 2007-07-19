#ifndef AIDA_PLUGIN_AIDAPLUGIN_FACTORY_H
# define AIDA_PLUGIN_AIDAPLUGIN_FACTORY_H 1


# include "AIDA_Plugin/AIDA_PluginType.h"
# include "PluginManager/PluginFactory.h"

#include <string>
#include <vector>
#include <map>

namespace AIDA_Plugin { 

class AIDA_PluginFactory : 
{
public:
    static AIDA_PluginFactory *get (void);

    //AIDA_PluginType *create (Iterator info) const;
    AIDA_PluginType *create (const std::string &name) const;

public:
  typedef void (*Function) (void);

private:
    AIDA_PluginFactory (void);
    static AIDA_PluginFactory s_instance;

private:
  /// keep a list of libs we have already open
  std::vector<std::string > libList;
  /// and a map to the handles
  std::map <std::string, void *> libMap;
};


}  // end namespace AIDA_Plugin 

#endif // PLUGIN_MANAGER_TEST_PLUGIN_FACTORY_H
