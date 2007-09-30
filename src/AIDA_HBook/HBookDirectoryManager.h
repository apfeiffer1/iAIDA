#ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKDIRECTORY_MANAGER_H
#define IAIDA_AIDA_HBOOKSTORE_HBOOKDIRECTORY_MANAGER_H

#include <string>
#include <vector>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookDirectoryManager
{
public:
  HBookDirectoryManager( const std::string& zebraName );
  ~HBookDirectoryManager() {}

  /// Checks the existence of a directory (absolute path)
  bool existsDirectory( const std::string& directory ) const;

  /// Creates a directory (absolute path)
  bool createDirectory( const std::string& directory ) const;

  /// Removes a directory (absolute path)
  bool removeDirectory( const std::string& directory ) const;

  /// Lists the directories inside a given directory
  std::vector< std::string > subDirectories( const std::string& directory ) const;

  /// Lists the one-dimensional histograms in a given directory
  std::vector< int > histograms1D( const std::string& directory ) const;

  /// Lists the two-dimensional histograms in a given directory
  std::vector< int > histograms2D( const std::string& directory ) const;

  /// Lists the one-dimensional profile histograms in a given directory
  std::vector< int > profiles( const std::string& directory ) const;

  /// Lists the tuples in a given directory
  std::vector< int > tuples( const std::string& directory ) const;

  /// Checks if a directory is empty
  bool isDirectoryEmpty( const std::string& directory ) const;

private:
  std::string m_zebraName;

private:
  void copyDirectoryStructureInMemory( const std::string& directory ) const;
};

  }
}

#endif
