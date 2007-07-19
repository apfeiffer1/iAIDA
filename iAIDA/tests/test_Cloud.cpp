#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  // Creating a tree mapped to an hbook file
  bool readOnly = false;
  bool createNew = true;
  std::auto_ptr<AIDA::ITree> tree( tf->create("cloud.hbook", "hbook", readOnly, createNew ) );

  std::cout << "Created a Tree with store " << tree->storeName() << std::endl;  
  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

  // Creating a cloud1D
  AIDA::ICloud1D& c1 = *( hf->createCloud1D( "c1", "Example Cloud") );
  c1.fill(1);
  c1.fill(2); 
  
  // convert cloud 

  const AIDA::IHistogram1D& h1 = c1.histogram(); 

  // create a copy of the histogram 
  AIDA::IHistogram1D& h2 = *( hf->createCopy(" 1",h1) ); 

  tree->commit(); 
  tree->close(); 
}

