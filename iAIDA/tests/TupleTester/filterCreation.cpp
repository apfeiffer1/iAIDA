#include "TupleTester.h"
#include "AIDA_Tuple_native/AIDA_DevFilterFactory.h"
#include "AIDA/IFilter.h"
#include <stdexcept>
#include <memory>

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif

double
TupleTester::testFilterCreation( std::ostream& os )
{
  try {
    os << "Creating a filter factory" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevFilterFactory > factory( new iAIDA::AIDA_Tuple_native::AIDA_DevFilterFactory );
    if ( ! factory.get() ) throw std::runtime_error( "Could not create a filter factory" );
    os << "Creating a scripted filter" << std::endl;
    std::auto_ptr< AIDA::IFilter > filter( factory->createScripted( "a*x + b > 0" ) );
    if ( ! filter.get() ) throw std::runtime_error( "Could not create a scripted filter" );
    os << "Creating a scripted filter with range" << std::endl;
    std::auto_ptr< AIDA::IFilter > filterRange( factory->createScripted( "a*y + b > 0", 100, 1 ) );
    if ( ! filterRange.get() ) throw std::runtime_error( "Could not create a scripted filter" );
    return 1;
  }
  catch ( std::exception& e ){
    os << e.what() << std::endl;
    return 0;
  }
}
