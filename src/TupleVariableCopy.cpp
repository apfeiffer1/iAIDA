#include "TupleVariableCopy.h"
#include <vector>
#include <string>

static const std::string doubleType = "double";
static const std::string floatType = "float";
static const std::string intType = "int";
static const std::string shortType = "short";
static const std::string longType = "long";
static const std::string charType = "char";
static const std::string boolType = "bool";
static const std::string stringType = "std::string";
static const std::string tupleType = "AIDA::ITuple";

void
iAIDA::AIDA_Tuple_native::TupleVarialbeCopyTuple::copyTupleVariable( const AIDA::ITuple& tupleFrom,
								      int columnFrom,
								      AIDA::ITuple& tupleTo,
								      int columnTo ) const
{
  // Get the sub tuples from both objects.
  AIDA::ITuple* subTupleFrom = const_cast<AIDA::ITuple*>( tupleFrom.getTuple( columnFrom ) );
  AIDA::ITuple* subTupleTo = tupleTo.getTuple( columnTo );
  
  // Get the descripion. We assume that it is the same...
  const int ncolumns = tupleFrom.columns();
  std::vector<std::string> names( ncolumns );
  std::vector<std::string> types( ncolumns );
  std::vector< iAIDA::AIDA_Tuple_native::TupleVariableCopyBase* > copyMethods;
  bool typesOK = true;
  for ( int i = 0; i < ncolumns; ++i ) {
    names[i] = subTupleFrom->columnName( i );
    std::string type = subTupleFrom->columnType( i );
    types[i] = type;
    if ( type == doubleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyDouble );
    else if ( type == floatType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyFloat );
    else if ( type == longType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyLong );
    else if ( type == intType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyInt );
    else if ( type == shortType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyShort );
    else if ( type == charType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyChar );
    else if ( type == boolType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyBoolean );
    else if ( type == stringType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyString );
    else if ( type == tupleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyTuple );
    else {
      typesOK = false;
      break;
    }
  }
  if ( ! typesOK ) {
    for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
    return;
  }
  
  subTupleFrom->start();
  subTupleTo->reset();
  while( subTupleFrom->next() ) {
    subTupleTo->addRow();
    // Copy over the values
    for ( int i = 0; i < ncolumns; ++i ) {
      copyMethods[i]->copyTupleVariable( *subTupleFrom, i, *subTupleTo, i );
    }
  }
  
  for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
  subTupleTo->start(); // Trick to flush the last row into the store.
}
