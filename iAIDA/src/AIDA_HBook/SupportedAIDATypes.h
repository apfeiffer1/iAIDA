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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUPPORTEDAIDATYPES_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUPPORTEDAIDATYPES_H

#include <string>
#include <set>

namespace iAIDA {
  namespace AIDA_HBookStore {

class SupportedAIDATypes
{
public:
  static const SupportedAIDATypes& supportedTypes();

protected:
  SupportedAIDATypes();
  SupportedAIDATypes( const SupportedAIDATypes& );
  SupportedAIDATypes& operator=( const SupportedAIDATypes& );

public:
  ~SupportedAIDATypes() {}
  bool isTypeSupported( const std::string& type ) const;

private:
  std::set< std::string > m_supportedTypes;
};

  }
}

#endif
