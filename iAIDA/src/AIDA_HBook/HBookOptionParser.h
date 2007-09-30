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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKPARSER_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKPARSER_H

#include <string>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookOptionParser
{
private:
  static const int DEFAULT_RECORD_LENGTH = 1024;
  static const int DEFAULT_BUFFER_SIZE_FOR_RWN = 1024;

public:
  /// Constructor initializing the class with the option string
  HBookOptionParser( const std::string& options );
  /// Destructor
  ~HBookOptionParser(){}

  /// returns the file record length
  int recordLength() const;

  /// Checks if the errors are stored together with the contents of the histogram
  bool areErrorsStored() const;

  /// Returns the buffer size for the creation of RWN tuples
  int bufferSizeForRWN() const;

private:
  int m_recordLength;
  bool m_areErrorsStored;
  int m_bufferSizeForRWN;
};

  }
}

#endif
