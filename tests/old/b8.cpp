#include <iostream>
#include <cstdlib>
#include <memory>
#include <assert.h>

#include "AIDA/AIDA.h"

#include <sstream>

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );

  // Creating the plotter factory
  /*
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );

  // Creating a plotter
  std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
  plotter->show();
  */
  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );
  assert( tf.get());

  // Creating a tree in memory
  std::auto_ptr< AIDA::ITree > tree( tf->create() );
  assert( tree.get());

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );
  assert( hf.get());


  // Creating a function factory whose functions will be handled by the tree
  std::auto_ptr< AIDA::IFunctionFactory > ff( af->createFunctionFactory( *tree ) );
  assert( ff.get());

  // Creating a tuple
  std::auto_ptr< AIDA::ITupleFactory > tpf( af->createTupleFactory( *tree ) );
  assert( tpf.get());

  // Creating a fit factory
  std::auto_ptr< AIDA::IFitFactory > fitf( af->createFitFactory() );
  assert( fitf.get());
 

  // Creating a tree mapped to an hbook file
  bool readOnly = true;
  bool createNew = false;
  std::auto_ptr<AIDA::ITree> treeHBook( tf->create("tuple.hbook", "hbook", readOnly, createNew, "lrec=2048" ) );
  if (! treeHBook.get() ) { 
    std::cerr <<"Error: could not open HBOOK file for reading" << std::endl;
    return -1;
   }
    
  // Creating a tree mapped to an XML file
  readOnly = false;
  createNew = true;
  std::auto_ptr<AIDA::ITree> treeXML( tf->create("tuple_analysis.xml", "xml", readOnly, createNew ) );
  if (! treeXML.get() ) { 
    std::cerr <<"Error: could not open XML file for writing" << std::endl;
    return -1;
   }

  // Mounting the hbook and xml trees under the master memory tree.
  tree->mkdir( "hbook" );
  tree->mkdir( "xml" );
  tree->mount( "/hbook", *treeHBook, "/" );
  tree->mount( "/xml", *treeXML, "/" );

  // Fetching the tuple
  AIDA::IManagedObject * obj =  tree->find( "/hbook/100" ); 
  if (!obj) { 
    std::cerr <<"Error: could not retrieve  Tuple from the file" << std::endl;
    return -1;
   }
  AIDA::ITuple& tp = dynamic_cast<AIDA::ITuple&>( *obj );

  // tuple description
  std::cout << "Tuple title : " << tp.title() << std::endl;
  std::cout << "Tuple variables : " << std::endl;
  for ( int i = 0; i < tp.columns(); ++i ) {
    std::cout << tp.columnName(i) << " \t " << tp.columnType(i) << std::endl;
  }

  // Creating an evaluator object
  std::auto_ptr<AIDA::IEvaluator> eval( tpf->createEvaluator( "beamEnergy / nTracks" ) );
  std::auto_ptr<AIDA::IFilter> cut( tpf->createFilter( "goodTrigger && nTracks > 0" ) );

  // Creating a cloud to project the tuple
  AIDA::ICloud1D& cl = *( hf->createCloud1D( "cloud_1", "Beam energy per track" ) );
  tp.project( cl, *eval, *cut );

  // Copying the cloud into a binned histogram in XML
  tree->cd( "/xml" );
  AIDA::IHistogram1D& h1 = *( hf->createCopy( "histo_1", cl.histogram() ) );
  h1.setTitle( "Beam energy per track" );

  // Creating the function which is going to be fitted with the histogram data
  AIDA::IFunction& gaussFun = *( ff->createFunctionByName("Gaussian Fit","G") );

  // set function initial parameters
  gaussFun.setParameter("mean" , h1.mean() );
  gaussFun.setParameter("sigma", h1.rms() );
  gaussFun.setParameter("amp"  , 100 );

  // Creating the fitter
  std::auto_ptr<AIDA::IFitter> fitter( fitf->createFitter() );

  // Performing the fit
  AIDA::IFitResult& fitResult = *( fitter->fit( h1, gaussFun ) );
  const std::vector<std::string>& parNames = fitResult.fittedParameterNames();
  const std::vector<double>& par = fitResult.fittedParameters();
  const std::vector<double>& epar = fitResult.errors();

  // Add in the annotation of the histogram the fit result
  AIDA::IAnnotation& annotation = h1.annotation();
  for ( unsigned int i = 0; i < par.size(); ++i ) {
    std::ostringstream os;
    os << par[i] << " +/- " << epar[i];
    annotation.addItem( parNames[i], os.str() );
  }

  // Remove some existing annotation items
  annotation.removeItem( "Overflow" );
  annotation.removeItem( "Underflow" );
  annotation.removeItem( "Extra Entries" );

  // Plotting the function with its annotation and then the histogram
  /*
  plotter->currentRegion().plot( h1, "annotation" );
  std::ostringstream osOptions;
  osOptions << "[" << h1.axis().lowerEdge() << "," << h1.axis().upperEdge() << "] overlay";
  plotter->currentRegion().plot( gaussFun, osOptions.str() );
  plotter->refresh();
  */

  // Listing the tree contents
  std::cout << "Full tree contents :" << std::endl;
  tree->ls( "/", 1 );

  // Flushing the results
  tree->commit();
  tree->close();

  // Wait for the keyboard return to avoid destroying the plotter window too quickly.
  //std::cout << "Press <ENTER> to exit" << std::endl;
  //std::cin.get();

  return 0;
}
