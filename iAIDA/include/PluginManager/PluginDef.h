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

#ifndef PLUGIN_MANAGER_PLUGIN_DEF_H
# define PLUGIN_MANAGER_PLUGIN_DEF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include <string>

namespace iAIDA {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ModuleDef;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Helper to simplify registration in #ModuleDef implementations. */
template <class Factory, class Type>
struct PluginDef
{
    static void declare (ModuleDef *def);
    static void declareName (ModuleDef *def, std::string name);
    static void installFactory (ModuleDef *def);
    static void installFactoryName (ModuleDef *def, std::string name);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Add @c Type as @c Type::catalogLabel() for module @a def.  Call
    this method in the module definition's @c query() method.  */
template <class Factory, class Type>
inline void
PluginDef<Factory,Type>::declare (ModuleDef *def)
{ Factory::get ()->declare (def, Type::catalogLabel ()); }

/** Add @c Type as @a name for module @a def.  Call this method
    in the module definition's @c query() method.  */
template <class Factory, class Type>
inline void
PluginDef<Factory,Type>::declareName (ModuleDef *def, std::string name)
{ Factory::get ()->declare (def, name); }

/** Add factory for @c Type, previously registered as @c
    Type::catalogLabel() for module @a def.  Call this method in
    the module definition's @c attach() method.  */
template <class Factory, class Type>
inline void
PluginDef<Factory,Type>::installFactory (ModuleDef *def)
{
    typedef typename Factory::Types::template AutoFactory<Type> AutoFactory;
    Factory::get ()->installFactory (def, Type::catalogLabel (), new AutoFactory);
}

/** Add factory for @c Type, previously registered as @a name for
    module @a def.  Call this method in the module definition's
    @c attach() method.  */
template <class Factory, class Type>
inline void
PluginDef<Factory,Type>::installFactoryName (ModuleDef *def, std::string name)
{
    typedef typename Factory::Types::template AutoFactory<Type> AutoFactory;
    Factory::get ()->installFactory (def, name, new AutoFactory);
}

} // namespace iAIDA
#endif // PLUGIN_MANAGER_PLUGIN_DEF_H
