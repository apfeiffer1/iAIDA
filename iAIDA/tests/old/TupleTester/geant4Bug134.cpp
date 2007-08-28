#include "TupleTester.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "TupleVariableDescriptionBuilder.h"
#include <stdexcept>
#include <memory>

#include "AIDA/AIDA.h"

#ifdef OLDSTREAMS
# include <iostream>
# include <strstream>
# define ostringstream ostrstream
#else
# include <ostream>
# include <sstream>
#endif

double
TupleTester::testGeant4Bug134( std::ostream& os) 
{
  os << "Creating a tuple..." << std::endl;

  AIDA::IAnalysisFactory * af = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory * treeF = af->createTreeFactory();
  AIDA::ITree * tree = treeF->create("geant4Test.aida", "xml", 0, 1);
  AIDA::ITupleFactory * tupF = af->createTupleFactory(*tree);

  AIDA::ITuple * tuple = tupF->create("100", "100", "string Name, float Energy, Time, Weight");

  const int ia = tuple->findColumn( "Name" );
  const int ib = tuple->findColumn( "Energy" );
  const int ic = tuple->findColumn( "Time" );
  const int id = tuple->findColumn( "Weight" );

  os << "Filling the tuple..." << std::endl;
  for ( int i = 0; i < 1000; ++i ) {

    std::ostringstream oss;
    oss << "string_" << i << std::ends;
    const std::string s = oss.str();
    if ( ! tuple->fill( ia, s ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ia ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ib, static_cast<float>( i + 1 ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ib ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ic, static_cast<float>( 2*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ic ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( id, static_cast<float>( 3*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( id ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->addRow() ) {
      std::ostringstream error;
      error << "Could not add in the tuple row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }
  }

  os << "Rows written : " << tuple->rows() << std::endl;

  // write out the tuple
  tree->commit();
  tree->close();

  os << "Tuple written to file" << std::endl;

  // clean up now ...
  delete tree; // this will also take care of the tuple :-)
  delete tupF;
  delete treeF;
  delete af;

  os << "Cleanup done" << std::endl;

  return 1;
}
