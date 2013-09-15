#include <iostream>
#include <cstdlib>
#include <memory>

#include <boost/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>

#include "AIDA/AIDA.h"

// #include "AIDA_CPP/Exceptions.h"
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>

int main( int, char** ) {

   // init the random engine
   boost::mt19937 rng;   // I don't seed it on purpose (it's not relevant)
   boost::normal_distribution<> nd(0.0, 10.0); // centre at 0, width of 10
   boost::variate_generator<boost::mt19937&, 
                            boost::normal_distribution<> > var_nor(rng, nd);

  // create a tree

  boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
  boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 
  bool readOnly = false;
  bool createNew = true;
  std::string options = "uncompress";        // default: use zip compression
  boost::shared_ptr<AIDA::ITree> tree(tf->create("tuple.xml","XML",readOnly,createNew, options)); 
  boost::shared_ptr<AIDA::ITupleFactory> tupF( af->createTupleFactory(*tree) );

  // create a simple tuple
  AIDA::ITuple * tuple = tupF->create("100", "100", "float lin; float sin; float gauss; float flat", options);
  if ( !tuple ) {
    std::cerr << "ERROR creating tuple !!" << std::endl;
    return 1;
  }

  const int ia = tuple->findColumn( "lin" );
  const int ib = tuple->findColumn( "sin" );
  const int ic = tuple->findColumn( "gauss" );
  const int id = tuple->findColumn( "flat" );

  std::cout << "Filling the tuple..." << std::endl;
  int iMax = 1000;
  for ( int i = 0; i < iMax; ++i ) {

    if ( i% (iMax/10) == 1 ) {
      std::cout << " i = " << i << std::endl;
    }
    float val = static_cast<float>( i );
    
    double valA = val;
    int col = ia;
    if ( ! tuple->fill( col, valA ) ) { std::cerr << "Could not fill the tuple for variable " << tuple->columnName( col ) << " and row " << i << std::endl; }

    double valB = sin(val/10.);
    col = ib;
    if ( ! tuple->fill( col, valB ) ) { std::cerr << "Could not fill the tuple for variable " << tuple->columnName( col ) << " and row " << i << std::endl; }

    double valC = var_nor();
    col = ic;
    if ( ! tuple->fill( col, valC ) ) { std::cerr << "Could not fill the tuple for variable " << tuple->columnName( col ) << " and row " << i << std::endl; }

    double valD = 50 * static_cast<double>( std::rand() ) / RAND_MAX;
    col = id;
    if ( ! tuple->fill( col, valD ) ) { std::cerr << "Could not fill the tuple for variable " << tuple->columnName( col ) << " and row " << i << std::endl; }

    if ( ! tuple->addRow() ) { std::cerr << "Could not add tuple row " << i << std::endl; }
  }
  std::cout << "Rows written : " << tuple->rows() << std::endl;

  std::cout << "tree listing : " << std::endl; 
  // browse the tree
  tree->ls(); 

  std::cout << "storename " << tree->storeName() << std::endl; 

  // save files 
  std::cout << "write tree contents to disk " << std::endl; 
  tree->commit(); 

  std::cout << "That's it !" << std::endl;

  return 0;
}
