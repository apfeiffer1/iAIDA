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

#ifndef PLUGIN_MANAGER_PLUGIN_DBVIEW_BASE_H
# define PLUGIN_MANAGER_PLUGIN_DBVIEW_BASE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include <string>

namespace iAIDA {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Module;
class PluginInfo;
class ModuleDescriptor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class to bridge #PluginManager and #PluginFactory.  */
class PluginFactoryBase
{
public:
    PluginFactoryBase (const std::string &tag);
    virtual ~PluginFactoryBase (void);

    // Category tag
    const std::string &	category (void) const;

    // Notifications from PluginManager.
    virtual void	restore (Module *module, ModuleDescriptor *info) =0;
    virtual void	addInfo (PluginInfo *info) = 0;
    virtual void	removeInfo (PluginInfo *info) = 0;

protected:
    // Notifications from derived class
    virtual void	rebuild (void);

private:
    std::string	m_tag;

    // undefined, no semantics
    PluginFactoryBase (const PluginFactoryBase &);
    PluginFactoryBase &operator= (const PluginFactoryBase &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace iAIDA
#endif // PLUGIN_MANAGER_PLUGIN_DBVIEW_BASE_H
