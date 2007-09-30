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

#ifndef HBOOK_PATH_FORMATER_H
#define HBOOK_PATH_FORMATER_H

#include <string>
#include <vector>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookPathFormater
{
public:
  static const HBookPathFormater& theFormater();

protected:
  HBookPathFormater() {};
  HBookPathFormater( const HBookPathFormater&);
  HBookPathFormater& operator=( const HBookPathFormater&);

public:
  ~HBookPathFormater(){};

  // Decomposes a path
  std::vector< std::string > formPathNames( const std::string& path ) const;
};

  }
}

#include "HBookPathFormater.inl"

#endif
