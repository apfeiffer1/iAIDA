#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

#include "AIDA_CPP/Exceptions.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

#include <typeinfo>
#include <boost/shared_ptr.hpp>

int doIt() 
{
  // create a tree

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 

  std::string tupleOptions = "--RWN";

  bool readOnly = false;
  bool createNew = true;
  boost::shared_ptr<AIDA::ITree> tree(tf->create("exatup.aida","xml",readOnly,createNew)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  boost::shared_ptr<AIDA::ITupleFactory> tupF ( af->createTupleFactory(*tree) ) ;

  AIDA::ITuple * tuple = tupF->create("100", "100", "float px; float py; float pz; float mass", tupleOptions);
  if ( !tuple ) {
    std::cerr << "ERROR creating tuple !!" << std::endl;
    return 1;
  }

  const int ia = tuple->findColumn( "px" );
  const int ib = tuple->findColumn( "py" );
  const int ic = tuple->findColumn( "pz" );
  const int id = tuple->findColumn( "mass" );

  std::cout << "Filling the tuple..." << std::endl;
  int iMax = 1000;
  for ( int i = 0; i < iMax; ++i ) {

    if ( i% (iMax/10) == 1 ) {
      std::cout << " i = " << i << std::endl;
    }

    float val = static_cast<float>( i );
    int col = ia;
    //    std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( ia ) << " and row " << i << std::endl;
    }
    val = val + 1;
    col = ib;
    //   std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( ib ) << " and row " << i << std::endl;
    }
    val = 2*val;
    col = ic;
    //    std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( ic ) << " and row " << i << std::endl;
    }
    val = 5*val;
    col = id;
    //    std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( id ) << " and row " << i << std::endl;
    }

    if ( ! tuple->addRow() ) {
      std::cerr << "Could not add tuple row " << i << std::endl;
      // std::ostringstream error;
      // error << "Could not add in the tuple row " << i << std::ends;
      // throw std::runtime_error( error.str() );
    }
  }

  std::cout << "Rows written : " << tuple->rows() << std::endl;

  // write out the tuple
  tree->commit();
  tree->close();

  std::cout << "Tuple written to file" << std::endl;

  
  return 0;
}

int main( int, char** )
{
  try {
   doIt();
  } catch( std::exception& e ) {
    std::cerr << e.what() << std::endl;
    std::cout << "Test failed." << std::endl;
    return 1;
  }
  return 0;
}
