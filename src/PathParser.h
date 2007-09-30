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

// Emacs -*- C++ -*-
#ifndef IAIDA_PATHPARSER
#define IAIDA_PATHPARSER 1

#include <list>
#include <string>

namespace iAIDA {
  namespace aida_tree {

/// Utility class used to create well formatted path names

class PathParser {

public:
  /// Constructor
  PathParser();
  /// Destructor
  ~PathParser();
  /// Forms a list of strings given a path
  std::list< std::string > formNames( const std::string& path ) const;
  /// Forms a path from a list of strings (appends one "/" between two names).
  std::string formPath( const std::list< std::string >& names ) const;
  /// Forms the parent dir given a path
  std::string parent( const std::string& path ) const;
};

  }
}

static const iAIDA::aida_tree::PathParser tree_path_parser;

#endif /* ifndef IAIDA_PATHPARSER */
