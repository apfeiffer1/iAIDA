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
  bool readOnly = false;
  bool createNew = true;
  std::string options = "--CWN";
  boost::shared_ptr<AIDA::ITree> tree(tf->create("exatupcwn.hbk","hbook",readOnly,createNew)); 
  boost::shared_ptr<AIDA::IHistogramFactory> factory( af->createHistogramFactory(*tree) );

  boost::shared_ptr<AIDA::ITupleFactory> tupF ( af->createTupleFactory(*tree) ) ;

  AIDA::ITuple * tuple = tupF->create("100", "100", "int n; float pt; float mass", options);
  if ( !tuple ) {
    std::cerr << "ERROR creating tuple !!" << std::endl;
    return 1;
  }

  const int ia = tuple->findColumn( "n" );
  const int ib = tuple->findColumn( "pt" );
  const int id = tuple->findColumn( "mass" );

  std::cout << "Filling the tuple..." << std::endl;
  int iMax = 1000;
  for ( int i = 0; i < iMax; ++i ) {

    if ( i% (iMax/10) == 1 ) {
      std::cout << " i = " << i << std::endl;
    }

    {
    int val = static_cast<int>( i );
    int col = ia;
    //    std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( ia ) << " and row " << i << std::endl;
    }
    }
    float val = static_cast<float> (i);
    int col = ib;
    //   std::cerr << "going to fill val " << val << " into " << col << std::endl;
    if ( ! tuple->fill( col, val ) ) {
      std::cerr << "Could not fill the tuple for variable " << tuple->columnName( ib ) << " and row " << i << std::endl;
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
  std::cout << "That's it !" << std::endl;

  return 0;
}
