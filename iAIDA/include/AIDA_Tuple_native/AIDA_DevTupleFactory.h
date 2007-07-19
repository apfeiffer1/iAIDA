// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY
#define AIDANATIVE_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY 1

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

#endif /* ifndef AIDANATIVE_AIDA_TUPLE_AIDA_DEVTUPLEFACTORY */
