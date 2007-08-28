#include "TupleTester.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "TupleVariableDescriptionBuilder.h"
#include <stdexcept>
#include <memory>

#ifdef OLDSTREAMS
# include <iostream>
# include <strstream>
# define ostringstream ostrstream
#else
# include <ostream>
# include <sstream>
#endif

double
TupleTester::testCreateAndFillTuple( std::ostream& os,
				     AIDA::Dev::IBackingStore& store,
				     AIDA::Dev::IDevTupleFactory& factory,
				     const std::string& tuplePath )
{
  os << "Creating a tuple..." << std::endl;
  std::auto_ptr<AIDA::Dev::IDevTuple> tuple( factory.create( &store, "Example tuple" ) );
  if ( ! tuple.get() ) throw std::runtime_error( "Could not create a tuple" );
  tuple->header().setPathInStore( tuplePath );

  const std::string description = "float a, b; double x,y,z; int i; bool l; Tuple{int n; float a} t1, t2; Tuple{ int n; Tuple{ int n } st } t3; string s";
  iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder builder( factory );
  if ( ! builder.buildDescription( tuple->header(), description ) ) {
    throw std::runtime_error( "Could not build the description" );
  }

  if ( ! store.writeTupleHeader( tuple->header() ) ) {
    throw std::runtime_error( "Could not write the header of the tuple in the store" );
  }

  const int ia = tuple->findColumn( "a" );
  const int ib = tuple->findColumn( "b" );
  const int ix = tuple->findColumn( "x" );
  const int iy = tuple->findColumn( "y" );
  const int iz = tuple->findColumn( "z" );
  const int ii = tuple->findColumn( "i" );
  const int il = tuple->findColumn( "l" );
  const int it1 = tuple->findColumn( "t1" );
  const int it2 = tuple->findColumn( "t2" );
  const int it3 = tuple->findColumn( "t3" );
  const int is = tuple->findColumn( "s" );

  os << "Filling the tuple..." << std::endl;
  for ( int i = 0; i < 1000; ++i ) {
    if ( ! tuple->fill( ia, static_cast<float>( i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ia ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ib, static_cast<float>( i + 1 ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ib ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ix, static_cast<double>( 2*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ix ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( iy, static_cast<double>( 3*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( iy ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( iz, static_cast<double>( 4*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( iz ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ii, static_cast<int>( i - 1 ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ii ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( il, ( ( i%3 == 0 ) ? true : false ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( il ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    AIDA::ITuple* subTuple1 = tuple->getTuple(it1);
    if ( ! subTuple1 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int itn1 = subTuple1->findColumn( "n" );
    const int ita1 = subTuple1->findColumn( "a" );
    for ( int j1 = 0; j1 < 10; ++j1 ) {
      if ( ! subTuple1->fill( itn1, j1 ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple1->fill( ita1, static_cast<float>( 2*j1 ) ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple1->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }

    AIDA::ITuple* subTuple2 = tuple->getTuple(it2);
    if ( ! subTuple2 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int itn2 = subTuple2->findColumn( "n" );
    const int ita2 = subTuple2->findColumn( "a" );
    for ( int j2 = 0; j2 < 15; ++j2 ) {
      if ( ! subTuple2->fill( itn2, 3*j2 ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple2->fill( ita2, static_cast<float>( 4*j2 ) ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple2->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }

    AIDA::ITuple* subTuple3 = tuple->getTuple(it3);
    if ( ! subTuple3 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int in = subTuple3->findColumn( "n" );
    const int ist = subTuple3->findColumn( "st" );
    for ( int j3 = 0; j3 < 20; ++j3 ) {
      if ( ! subTuple3->fill( in, j3 ) ) throw std::runtime_error( "Could not fill the subtuple" );

      AIDA::ITuple* subTuple = subTuple3->getTuple(ist);
      if ( ! subTuple ) throw std::runtime_error( "Could not retrieve the sub-subtuple" );
      const int n = subTuple->findColumn( "n" );
      for ( int j4 = 0; j4 < 5; ++j4 ) {
	if ( ! subTuple->fill( n, 10*j3 + j4 ) ) throw std::runtime_error( "Could not fill the sub-subtuple" );
	if ( ! subTuple->addRow() ) throw std::runtime_error( "Could not add a row in the sub-subtuple" );
      }

      if ( ! subTuple3->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }

    std::ostringstream oss;
    oss << "string_" << i << std::ends;
    const std::string s = oss.str();
    if ( ! tuple->fill( is, s ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( is ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->addRow() ) {
      std::ostringstream error;
      error << "Could not add in the tuple row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }
  }

  os << "Rows written : " << tuple->rows() << std::endl;


  return 1;
}
