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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKFILEHANDLER_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKFILEHANDLER_H

#include <string>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookFileHandler
{
public:
  /// Constructor : opens the file
  HBookFileHandler( const std::string& fileName,
		    int recordLength,
		    bool readOnly,
		    bool createNew );

  /// Destructor : closes the file
  ~HBookFileHandler();

  /// Returns the ZEBRA name associated to the file
  const std::string& zebraName() const;

private:
  int         m_lun;
  std::string m_zebraName;

private:
  bool existsFile( const std::string& fileName ) const;
};

  }
}

#endif
