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
#ifndef IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE
#define IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE 1

#include "AIDA_Filter.h"

namespace AIDA {
  namespace Dev {
    class ITupleHeader;
  }
}

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_FilterRange : public AIDA_Filter
{
public:
  // Constructor
  AIDA_FilterRange( CompiledExpressionManager& manager,
		    const std::string& expression,
		    int numberOfRows,
		    int startingRow );
  // Destructor
  ~AIDA_FilterRange(){};

  // Methods inherited from AIDA/IFilter
  bool initialize( AIDA::ITuple & tuple );
  bool accept() const;
 
private:
  int                               m_startingRow;
  int                               m_endingRow;
  AIDA::Dev::ITupleHeader*          m_header;
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE */
