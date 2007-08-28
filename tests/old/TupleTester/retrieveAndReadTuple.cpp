#include "TupleTester.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"

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
TupleTester::testRetrieveAndReadTuple( std::ostream& os,
				       AIDA::Dev::IBackingStore& store,
				       AIDA::Dev::IDevTupleFactory& factory,
				       const std::string& tuplePath )
{
  os << "Retrieving a tuple from the store..." << std::endl;
  std::auto_ptr<AIDA::Dev::IDevTuple> tuple( factory.create( &store, "" ) );
  if ( ! tuple.get() ) throw std::runtime_error( "Could not create a tuple" );
  tuple->header().setPathInStore( tuplePath );

  if ( ! store.readTupleHeader( tuple->header() ) ) {
    throw std::runtime_error( "Could not read the header of the tuple from the store" );
  }

  os << "Tuple title : " << tuple->title() << std::endl;
  os << "Variables : name type entries min max mean rms" << std::endl;
  for ( int iVar = 0; iVar < tuple->header().numberOfVariables(); ++iVar ) {
    const AIDA::Dev::ITupleVariableDescription& description = * ( tuple->header().variableDescription( iVar ) );
    const AIDA::Dev::ITupleVariableStatistics& statistics = description.statistics();
    os << iVar << "\t" << description.variableName() << " " << description.variableType()
       << " " << statistics.entries() << " " << statistics.min() << " " << statistics.max() << " "
       << statistics.mean() << " " << statistics.rms() << std::endl;
  }

  os << "Looping over the tuple ..." << std::endl;

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


  if ( tuple->rows() != 1000 ) throw std::runtime_error( "Different number of rows for stored and retrieved tuple." );
  tuple->start();
  for ( int i = 0; tuple->next(); ++i ) {
    if ( tuple->getFloat( ia ) != static_cast<float>( i ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getFloat( ib ) != static_cast<float>( i + 1 ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getDouble( ix ) != static_cast<double>( 2*i ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getDouble( iy ) != static_cast<double>( 3*i ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getDouble( iz ) != static_cast<double>( 4*i ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getInt( ii ) != i-1 ) throw std::runtime_error( "Value retrieved different from value stored" );
    if ( tuple->getBoolean( il ) != ( ( i%3 == 0 ) ? true : false ) ) throw std::runtime_error( "Value retrieved different from value stored" );
    std::ostringstream oss;
    oss << "string_" << i << std::ends;
    const std::string s = oss.str();
    if ( tuple->getString( is ) != s ) throw std::runtime_error( "Value retrieved different from value stored" );


    AIDA::ITuple* subTuple1 = tuple->getTuple(it1);
    if ( ! subTuple1 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    if ( subTuple1->rows() != 10 ) throw std::runtime_error( "Different number of rows for stored and retrieved subtuple." );
    const int itn1 = subTuple1->findColumn( "n" );
    const int ita1 = subTuple1->findColumn( "a" );
    subTuple1->start();
    for ( int j1 = 0; subTuple1->next(); ++j1 ) {
      if ( subTuple1->getInt( itn1 ) != j1 ) throw std::runtime_error( "Value retrieved different from value stored for the subtuple" );
      if ( subTuple1->getFloat( ita1 ) != static_cast<float>( 2*j1 ) ) throw std::runtime_error( "Value retrieved different from value stored for the subtuple" );
    }

    AIDA::ITuple* subTuple2 = tuple->getTuple(it2);
    if ( ! subTuple2 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    if ( subTuple2->rows() != 15 ) throw std::runtime_error( "Different number of rows for stored and retrieved subtuple." );
    const int itn2 = subTuple2->findColumn( "n" );
    const int ita2 = subTuple2->findColumn( "a" );
    subTuple2->start();
    for ( int j2 = 0; subTuple2->next(); ++j2 ) {
      if ( subTuple2->getInt( itn2 ) != 3*j2 ) throw std::runtime_error( "Value retrieved different from value stored for the subtuple" );
      if ( subTuple2->getFloat( ita2 ) != static_cast<float>( 4*j2 ) ) throw std::runtime_error( "Value retrieved different from value stored for the subtuple" );
    }

    AIDA::ITuple* subTuple3 = tuple->getTuple(it3);
    if ( subTuple3->rows() != 20 ) throw std::runtime_error( "Different number of rows for stored and retrieved subtuple." );
    if ( ! subTuple3 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int in = subTuple3->findColumn( "n" );
    const int ist = subTuple3->findColumn( "st" );
    subTuple3->start();
    for ( int j3 = 0; subTuple3->next(); ++j3 ) {
      if ( subTuple3->getInt( in ) != j3 ) throw std::runtime_error( "Value retrieved different from value stored for the subtuple" );

      AIDA::ITuple* subTuple = subTuple3->getTuple(ist);
      if ( ! subTuple ) throw std::runtime_error( "Could not retrieve the sub-subtuple" );
      if ( subTuple->rows() != 5 ) throw std::runtime_error( "Different number of rows for stored and retrieved sub-subtuple." );
      const int n = subTuple->findColumn( "n" );
      subTuple->start();
      for ( int j4 = 0; subTuple->next(); ++j4 ) {
	if ( subTuple->getInt( n ) != 10*j3 + j4 ) throw std::runtime_error( "Value retrieved different from value stored for the sub-subtuple" );
      }
    }
  }

  return 1.0;
}
