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

#ifndef PLUGIN_MANAGER_CONFIG_H
# define PLUGIN_MANAGER_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealPlatform/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def PLUGIN_MANAGER_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #PLUGIN_MANAGER_BUILD_DLL is defined, imported
         if #PLUGIN_MANAGER_BUILD_ARCHIVE is not defined, and left
	 alone if latter is defined (for an archive library build).  */

/** @def PLUGIN_MANAGER_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #PLUGIN_MANAGER_API should be exported.  */

/** @def PLUGIN_MANAGER_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
	 library, not as a shared library.  Lack of this indicates
	 that the header is included during the use of a shared
	 library of this package, and all entities marked with
	 #PLUGIN_MANAGER_API should be imported.  */

# ifndef PLUGIN_MANAGER_API
#  ifdef _WIN32
#   if defined PLUGIN_MANAGER_BUILD_DLL
#    define PLUGIN_MANAGER_API __declspec(dllexport)
#   elif ! defined PLUGIN_MANAGER_BUILD_ARCHIVE
#    define PLUGIN_MANAGER_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef PLUGIN_MANAGER_API
#  define PLUGIN_MANAGER_API
# endif

#endif // PLUGIN_MANAGER_CONFIG_H
