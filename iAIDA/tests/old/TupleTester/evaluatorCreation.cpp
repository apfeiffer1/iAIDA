#include "TupleTester.h"
#include "AIDA_Tuple_native/AIDA_DevEvaluatorFactory.h"
#include "AIDA/IEvaluator.h"
#include <stdexcept>
#include <memory>

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif

double
TupleTester::testEvaluatorCreation( std::ostream& os )
{
  try {
    os << "Creating a evaluator factory" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevEvaluatorFactory > factory( new iAIDA::AIDA_Tuple_native::AIDA_DevEvaluatorFactory );
    if ( ! factory.get() ) throw std::runtime_error( "Could not create a evaluator factory" );
    os << "Creating a scripted evaluator" << std::endl;
    std::auto_ptr< AIDA::IEvaluator > evaluator( factory->createScripted( "a*x + b" ) );
    if ( ! evaluator.get() ) throw std::runtime_error( "Could not create a scripted evaluator" );
    return 1;
  }
  catch ( std::exception& e ){
    os << e.what() << std::endl;
    return 0;
  }
}
