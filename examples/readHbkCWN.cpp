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
  bool readOnly = true;
  bool createNew = false;
  boost::shared_ptr<AIDA::ITree> tree(tf->create("exatupcwn.hbk", "hbook", readOnly, createNew)); 

  //  boost::shared_ptr<AIDA::ITupleFactory> tupF ( af->createTupleFactory(*tree) ) ;

  AIDA::ITuple * tuple = dynamic_cast<AIDA::ITuple*> ( tree->find("100") );
  if ( !tuple ) {
    std::cerr << "ERROR reading tuple !!" << std::endl;
    return 1;
  }

  const int ia = tuple->findColumn( "n" );
  const int ib = tuple->findColumn( "pt" );
  const int id = tuple->findColumn( "mass" );

  std::cout << "Checking the tuple..." << std::endl;

  tuple->start();
  int i = 0;
  double va, vb, vc, vd;
  while ( tuple->next() ) {
    if (++i < 10) {
      va = tuple->getFloat(ia);
      vb = tuple->getFloat(ib);
      vd = tuple->getFloat(id);
      std::cerr << "Found col " << tuple->columnName( ia ) << ": " << va << std::endl;
      std::cerr << "Found col " << tuple->columnName( ib ) << ": " << vb << std::endl;
      std::cerr << "Found col " << tuple->columnName( id ) << ": " << vd << std::endl;
    }
  }

  std::cout << "Rows found : " << i << std::endl;

  tree->commit();
  tree->close();
  
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
