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

// Emacs -*- C++ -*-
#ifndef IAIDA_MEMORYDATAPOINTSETCOPIER
#define IAIDA_MEMORYDATAPOINTSETCOPIER 1


// forward declarations
namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IDevDataPointSetFactory;
    class IDevManagedObject;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

/// Class responsible for copying data point sets in a memory store

class MemoryDataPointSetCopier
{
public:
  MemoryDataPointSetCopier( );
  ~MemoryDataPointSetCopier(){};
  AIDA::Dev::IDevManagedObject* createCopy( const AIDA::IManagedObject& dataObject,   AIDA::Dev::IDevDataPointSetFactory & df );
private:

};

  }
}

#endif
