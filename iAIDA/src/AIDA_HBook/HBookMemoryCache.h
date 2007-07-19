#ifndef PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_MEMORYCACHE_H
#define PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_MEMORYCACHE_H

#include <string>
#include <set>

namespace pi {
  namespace AIDA_HBookStore {

class HBookMemoryCache
{
public:
  HBookMemoryCache( const std::string& zebraName );
  ~HBookMemoryCache();

  // Flushes the contents of the memory store to the file
  void flushToFile( const std::set< std::string >& tuplesToUpdate ) const;

  // Returns the directory in file
  std::string directoryInFile( const std::string& directory ) const;

  // Returns the directory in memory
  std::string directoryInMemory( const std::string& directory ) const;

private:
  // clears the contents of a directory
  void clearDirectory( const std::string& directory ) const;

  // clears the histograms from the cache
  void clearHistogramsFromCache( const std::string& directory ) const;

  // flushes the objects in a given directory
  void flushToFile( const std::string& directory,
		    const std::set< std::string >& tuplesToUpdate ) const;

private:
  std::string m_zebraName;
};

  }
}


#endif
