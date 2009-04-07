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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_MEMORYCACHE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_MEMORYCACHE_H

#include <string>
#include <set>

namespace iAIDA {
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
