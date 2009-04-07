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
#ifndef IAIDA_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY
#define IAIDA_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY 1

#include "AIDA_Dev/IDevTupleFactory.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {

/// Implementation of the IDevTupleFactory interface

class AIDA_DevTupleFactory : virtual public AIDA::Dev::IDevTupleFactory
{
public:
  /// Constructor
  AIDA_DevTupleFactory();
  /// Destructor
  ~AIDA_DevTupleFactory();
  // Methods inherited from IDevTupleFactory
  AIDA::Dev::IDevTuple* create( AIDA::Dev::IBackingStore* backingStore,
				const std::string& title,
				std::string options = "");


  AIDA::Dev::IDevTuple* createChained( const std::string& title,
				       const std::vector<AIDA::Dev::IDevTuple*>& set );
  AIDA::Dev::ITupleVariableDescription* createDescription();
  AIDA::Dev::ITupleVariableDescription* createDescription( const AIDA::Dev::ITupleVariableDescription& original );

  // build header from columns string (names and types) 
  bool buildTupleHeader(const std::vector<std::string>& columnNames, 
			const std::vector<std::string>& columnTypes,
			const std::string & pathInStore, 
			AIDA::Dev::ITupleHeader & header);  
  bool buildTupleHeader(const std::string & columnDescr, 
			const std::string & pathInStore, 
			AIDA::Dev::ITupleHeader & header);  

};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY */
