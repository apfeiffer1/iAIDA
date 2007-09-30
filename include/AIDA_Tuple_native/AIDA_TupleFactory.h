// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEFACTORY_H
#define IAIDA_AIDA_TUPLE_AIDA_TUPLEFACTORY_H 1

#include "AIDA/ITupleFactory.h"

namespace AIDA {
  namespace Dev {
    class IDevTree;
    class IDevTupleFactory;
    class IDevFilterFactory;
    class IDevEvaluatorFactory;
  }
}

namespace iAIDA {
  namespace AIDA_Tuple_native {

/// Implementation of the AIDA::ITupleFactory class

class AIDA_TupleFactory : virtual public AIDA::ITupleFactory
{
public:
  /// Constructor
  AIDA_TupleFactory( AIDA::Dev::IDevTree& tree,
		     AIDA::Dev::IDevTupleFactory& tupleFactory,
		     AIDA::Dev::IDevFilterFactory& filterFactory,
		     AIDA::Dev::IDevEvaluatorFactory& evaluatorrFactory );
  /// Destructor
  ~AIDA_TupleFactory() {};

  // Methods inherited from AIDA::ITupleFactory
  AIDA::ITuple* create( const std::string& name,
			const std::string & title,
			const std::vector<std::string>& columnNames,
			const std::vector<std::string>& columnType,
			const std::string & options = "" );

  AIDA::ITuple* create( const std::string& name,
			const std::string& title,
			const std::string& columns,
			const std::string& options = "" );

  AIDA::ITuple* createChained( const std::string& name,
			       const std::string& title,
			       const std::vector<AIDA::ITuple*>& tupleSet );

  AIDA::ITuple* createChained( const std::string& name,
			       const std::string& title,
			       const std::vector<std::string>& tupleSet);

  AIDA::ITuple* createFiltered( const std::string& name,
			        AIDA::ITuple& tuple,
				AIDA::IFilter& filter );

  AIDA::ITuple* createFiltered( const std::string& name,
			        AIDA::ITuple& tuple,
				AIDA::IFilter& filter,
				const std::vector<std::string>& columns );

  AIDA::IFilter* createFilter( const std::string& expression );

  AIDA::IFilter* createFilter( const std::string& expression,
			       int rowsToProcess,
			       int startingRow = 0 );

  AIDA::IEvaluator* createEvaluator( const std::string& expression );

private:
  AIDA::Dev::IDevTree&              m_tree;
  AIDA::Dev::IDevTupleFactory&      m_tupleFactory;
  AIDA::Dev::IDevFilterFactory&     m_filterFactory;
  AIDA::Dev::IDevEvaluatorFactory&  m_evaluatorFactory;
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLEFACTORY_H */
