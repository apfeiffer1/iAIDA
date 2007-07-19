#include <iostream>
#include <stdexcept>
#include <memory>
#include <iomanip>
#include "TupleTester.h"
#include <fstream>
#include "DummyBackingStore.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"
#include "AIDA_Dev/IDevTupleFactory.h"

#ifdef OLDSTREAMS
# define ios_base ios
#endif

int main( int, char** )
{
  try {

    int totalTests = 0;
    double successfullTests =0;
    TupleTester tester;

    std::ofstream fg4b134( "fg4b134.out" );
    successfullTests += tester.testGeant4Bug134( fg4b134 );
    totalTests++;

    std::ofstream ftvdb( "ftvdb.out" );
    successfullTests += tester.testDescriptionBuilder( ftvdb );
    totalTests++;

    std::ofstream fexp( "fexp.out" );
    successfullTests += tester.testExpressionParser( fexp );
    totalTests++;

    std::ofstream ffc( "ffc.out" );
    successfullTests += tester.testFilterCreation( ffc );
    totalTests++;

    std::ofstream fec( "fec.out" );
    successfullTests += tester.testEvaluatorCreation( fec );
    totalTests++;

    std::ofstream fcem( "fcem.out" );
    successfullTests += tester.testCompiledExpressionManager( fcem );
    totalTests++;

    std::cout << "Creating a tuple factory..." << std::endl;
    std::auto_ptr<AIDA::Dev::IDevTupleFactory> fp( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory );
    if ( ! fp.get() ) throw std::runtime_error( "Could not create a dev tuple factory" );
    AIDA::Dev::IDevTupleFactory& factory = *fp;

    std::cout << "Creating a dummy backing store..." << std::endl;
    std::auto_ptr<AIDA::Dev::IBackingStore> store( new DummyBackingStore( factory ) );
    if ( ! store.get() ) throw std::runtime_error( "Could not create a dummy store" );

    const std::string tuplePath = "/dir1/tuple1";

    std::ofstream fcft( "fcft.out" );
    successfullTests += tester.testCreateAndFillTuple( fcft, *store, factory, tuplePath );
    totalTests++;

    std::ofstream frrt( "frrt.out" );
    successfullTests += tester.testRetrieveAndReadTuple( frrt, *store, factory, tuplePath );
    totalTests++;

    if ( totalTests > 0 ) successfullTests *= 100.0 / totalTests;
    else successfullTests = 0;
    std::cout << "Test succeeded with a score of "
	      << std::setiosflags(std::ios_base::fixed)
	      << std::setprecision(2)
	      << successfullTests << "%" << std::endl;
    return 0;
  }
  catch( std::exception& e ) {
    std::cerr << e.what() << std::endl;
    std::cout << "Test failed." << std::endl;
    return 1;
  }
}
