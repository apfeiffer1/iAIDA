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
