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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOK_HBOOKFILEREGISTRY_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOK_HBOOKFILEREGISTRY_H

#include <map>
#include <string>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookFileRegistry
{
public:
  static HBookFileRegistry& theRegistry();

protected:
  HBookFileRegistry() {};
  HBookFileRegistry( const HBookFileRegistry& );
  HBookFileRegistry& operator=( const HBookFileRegistry& );

public:
  ~HBookFileRegistry() {};

  int nextAvailableLUN( const std::string& fileName );
  void releaseLUN( int lun );

private:
  std::map< int, std::string > m_allocatedLUNs;
  std::map< std::string, int > m_fileNames;
};

  }
}
#endif
