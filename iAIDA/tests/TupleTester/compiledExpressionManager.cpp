#include "TupleTester.h"
#include "CompiledExpressionManager.h"
#include <stdexcept>
#include <memory>

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif

double
TupleTester::testCompiledExpressionManager( std::ostream& os )
{
  try {
    os << "Creating a CompiledExpressionManager." << std::endl;
    std::auto_ptr< iAIDA::AIDA_Tuple_native::CompiledExpressionManager > mgr( new iAIDA::AIDA_Tuple_native::CompiledExpressionManager );
    if ( ! mgr.get() ) throw std::runtime_error( "Could not create a CompiledExpressionManager object" );

    std::map<std::string, std::string> variableTypes;
    variableTypes.insert( std::make_pair( std::string( "a" ), std::string( "int" ) ) );
    variableTypes.insert( std::make_pair( std::string( "b" ), std::string( "float" ) ) );
    variableTypes.insert( std::make_pair( std::string( "c" ), std::string( "double" ) ) );
    variableTypes.insert( std::make_pair( std::string( "d" ), std::string( "bool" ) ) );
    os << "Variables defined: " << std::endl;
    for ( std::map<std::string, std::string>::const_iterator iVariable = variableTypes.begin();
	  iVariable != variableTypes.end(); ++iVariable ) {
      os << iVariable->first << " : " << iVariable->second << std::endl;
    }

    std::string expressionFilter = "a*b + c > 0";
    std::set<std::string> variablesUsed;

    os << "Creating a filter expression from using the string \"" << expressionFilter << "\"" << std::endl;
    std::auto_ptr<iAIDA::AIDA_Tuple_native::IFilterExpressionBase> filterExpression = mgr->createFilterExpression( expressionFilter,
														    variableTypes,
														    variablesUsed );
    if ( ! filterExpression.get() ) throw std::runtime_error( "Could not create a filter expression" );
    os << "Variables used: " << std::endl;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      os << *iVariable << std::endl;
    }


    os << "Creating a filter expression from using the same string" << std::endl;
    variablesUsed.clear();
    std::auto_ptr<iAIDA::AIDA_Tuple_native::IFilterExpressionBase> filterExpressionSame = mgr->createFilterExpression( expressionFilter,
															variableTypes,
															variablesUsed );
    if ( ! filterExpressionSame.get() ) throw std::runtime_error( "Could not create a filter expression" );
    os << "Variables used: " << std::endl;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      os << *iVariable << std::endl;
    }


    std::string expressionEvaluator = "a + c";
    os << "Creating a evaluator expression from using the string \"" << expressionEvaluator << "\"" << std::endl;
    std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase> evaluatorExpression = mgr->createEvaluatorExpression( expressionEvaluator,
															     variableTypes,
															     variablesUsed );
    if ( ! evaluatorExpression.get() ) throw std::runtime_error( "Could not create a evaluator expression" );
    os << "Variables used: " << std::endl;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      os << *iVariable << std::endl;
    }


    os << "Creating a evaluator expression from using the same string" << std::endl;
    variablesUsed.clear();
    std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase> evaluatorExpressionSame = mgr->createEvaluatorExpression( expressionEvaluator,
																 variableTypes,
																 variablesUsed );
    if ( ! evaluatorExpressionSame.get() ) throw std::runtime_error( "Could not create a evaluator expression" );
    os << "Variables used: " << std::endl;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      os << *iVariable << std::endl;
    }


    return 1;
  }
  catch ( std::exception& e ){
    os << e.what() << std::endl;
    return 0;
  }
}
