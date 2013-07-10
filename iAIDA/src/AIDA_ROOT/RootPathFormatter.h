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

#ifndef IAIDA_AIDA_ROOTSTORE_ROOTPATHFORMATTER_H
#define IAIDA_AIDA_ROOTSTORE_ROOTPATHFORMATTER_H

#include <string>
#include <vector>

namespace iAIDA {
  namespace AIDA_RootStore {

class RootPathFormatter
{
public:
  static const RootPathFormatter& theFormatter();

protected:
  RootPathFormatter() {};
  RootPathFormatter( const RootPathFormatter&);
  RootPathFormatter& operator=( const RootPathFormatter&);

public:
  ~RootPathFormatter(){};

  // Decomposes a path
  std::vector< std::string > formPathNames( const std::string& path ) const;
};

  }
}

#include "RootPathFormatter.inl"

#endif
