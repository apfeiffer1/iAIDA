#include "TupleTester.h"
#include "TupleVariableDescriptionBuilder.h"
#include "AIDA_TupleHeader.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include <memory>
#include <stdexcept>

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif

static void dumpDescription( std::ostream& os,
			     const AIDA::Dev::ITupleVariableDescription& description,
			     int depth )
{
  for ( int i = 0; i < depth; ++i ) os << "  ";
  os << description.variableName() << " : " << description.variableType() << std::endl;
  for ( int iVar = 0; iVar < description.numberOfVariables(); ++iVar ) {
    dumpDescription( os, *( description.variableDescription( iVar ) ), depth + 1 );
  }
}


double
TupleTester::testDescriptionBuilder( std::ostream& os )
{
  try {
    os << "Testing the TupleVariableDescriptionBuilder class" << std::endl;
    iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory fact;
    iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder builder( fact );

    std::vector<std::string> names;
    std::vector<std::string> types;

    std::string input = "float  a, b; int c;;  ; Tuple{ float a; int b} d,e; bool f; float g; Tuple{int n; Tuple{float a} t1; Tuple{float b} t2, t3 }h";

    os << "Analysing the string : " << input << std::endl;
    builder.decompose( input, names, types );
    os << "Variables : " << std::endl;
    for ( unsigned int i = 0; i < names.size(); ++i ) {
      os << names[i] << " : " << types[i] << std::endl;
    }

    os << "Building a description chain. " << std::endl;
    std::auto_ptr< AIDA::Dev::ITupleHeader > header( new iAIDA::AIDA_Tuple_native::AIDA_TupleHeader  );
    if ( ! header.get() ) throw std::runtime_error( "Could not create a header object" );
    if ( ! builder.buildDescription( *header, input ) ) {
      throw std::runtime_error( "Could not build the description" );
    }

    os << "Description : " << std::endl;
    for ( int iVar = 0; iVar < header->numberOfVariables(); ++iVar ) {
      const AIDA::Dev::ITupleVariableDescription& description = *( header->variableDescription( iVar ) );
      dumpDescription( os, description, 0 );
    }

    return 1.0;
  }
  catch ( std::exception& e ) {
    os << e.what() << std::endl;
    return 0;
  }
}
