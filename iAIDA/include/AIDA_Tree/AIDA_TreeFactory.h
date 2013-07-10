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
#ifndef ANAPHE_TREEFACTORY
#define ANAPHE_TREEFACTORY 1

#include <string>
#include "AIDA/ITreeFactory.h"

// type ahead declarations 
namespace AIDA {
  class ITree;
}

namespace iAIDA {
  namespace aida_tree {

/// Implementation of the AIDA::ITreeFactory abstract interface

class AIDA_TreeFactory : public virtual AIDA::ITreeFactory
{
public:
  /// Constructor
  AIDA_TreeFactory();
  /// Destructor
  ~AIDA_TreeFactory();
  /// Methods inherited from the ITreeFactory class
  AIDA::ITree* create();
  AIDA::ITree* create( const std::string & storeName,
		       const std::string & storeType = "",
		       bool readOnly = false,
		       bool createNew = true,
		       const std::string & options = "" );

// protected: 
//   AIDA::ITree* m_create( const std::string & storeName,
// 		       const std::string & storeType = "",
// 		       bool readOnly = false,
// 		       bool createNew = true,
// 		       const std::string & options = "" );

};

  }
}

#endif /* ifndef ANAPHE_TREEFACTORY */
