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

#ifndef PLUGIN_MANAGER_PLUGIN_ERROR_H
# define PLUGIN_MANAGER_PLUGIN_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "SealBase/Error.h"

namespace iAIDA {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Error object for the plug-in manager.  All lower-level errors
    are chained to this type before being passed upwards.  */
class PluginError : public Error
{
public:
    PluginError (Error *reason);
    
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace iAIDA
#endif // PLUGIN_MANAGER_PLUGIN_ERROR_H
