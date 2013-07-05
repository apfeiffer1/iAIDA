
#include "AIDA/AIDA.h"
#include <iostream>
#include <assert.h>
using namespace std;

int main(){

  AIDA::ITreeFactory* treeFactory;  
  AIDA::ITree* inputTree;
  /*
  AIDA::IFilter* particleFilter; //will select the particle
  AIDA::IFilter* orderFilter; // will select if primary or secondary particle
  */
  AIDA::IFilter* tupleFilter;

  AIDA::IEvaluator* tupleEnergyEvaluator;
  AIDA::IEvaluator* tupleMomXevaluator;
  AIDA::IEvaluator* tupleMomYevaluator;
  AIDA::IEvaluator* tupleMomZevaluator;

  AIDA::IAnalysisFactory* analysisFactory;
  AIDA::ITupleFactory* tupleFactory;
  
  AIDA::IHistogramFactory* histoFactory;

  AIDA::ITuple* tupleInput;

  AIDA::IPlotterFactory* plotterFactory;
  AIDA::IPlotter* plotter;

  analysisFactory = AIDA_createAnalysisFactory();
  treeFactory = analysisFactory->createTreeFactory();
  inputTree = treeFactory->create("testem14.XML","XML", true);
  assert (inputTree);
	
  AIDA::ITree *memTree = treeFactory->create("mem","memory");
  
  inputTree->ls();
  tupleInput = dynamic_cast<AIDA::ITuple*>(inputTree->find("10"));

  assert(tupleInput);
  
  // now we have the input data. What we are going to do is to filter the tuple 
  // in order to have a tuple of primaries or a tuple for secondaries.
  // these tuple will be filtered agin in order to have a tuple per particle to be plotted
  
  histoFactory = analysisFactory->createHistogramFactory( *memTree );
  AIDA::IHistogram1D* histoEn = histoFactory->createHistogram1D("10", "Energy", 10000 ,0, 1000);
  AIDA::IHistogram1D* histoX = histoFactory->createHistogram1D("11", "X Direction", 1000 ,-1.001, 1.001);
  AIDA::IHistogram1D* histoY = histoFactory->createHistogram1D("12", "Y Direction", 1000 ,-1.001, 1.001);
  AIDA::IHistogram1D* histoZ = histoFactory->createHistogram1D("13", "Z Direction", 1000 ,-1.001, 1.001);
  std::cout << "Histos booked" << std::endl;

  tupleFilter = tupleFactory->createFilter("(ParentTrackId ==  0) && (ParticleType == 0)");
  tupleFilter->initialize(*tupleInput);
  
  tupleEnergyEvaluator = tupleFactory->createEvaluator("ParticleEnergy * 10e6");
  tupleMomXevaluator = tupleFactory->createEvaluator("ParallelMomentumDirection(X)");
  tupleMomYevaluator = tupleFactory->createEvaluator("TransverseMomentumDirection(Y)");
  tupleMomZevaluator = tupleFactory->createEvaluator("TransverseMomentumDirection(Z)");
  
  tupleEnergyEvaluator->initialize(*tupleInput);
  tupleMomXevaluator->initialize(*tupleInput);
  tupleMomYevaluator->initialize(*tupleInput);
  tupleMomZevaluator->initialize(*tupleInput);
  
  tupleInput->project(*histoEn, *tupleEnergyEvaluator, *tupleFilter);
  tupleInput->project(*histoX, *tupleMomXevaluator, *tupleFilter);
  tupleInput->project(*histoY, *tupleMomYevaluator, *tupleFilter);
  tupleInput->project(*histoZ, *tupleMomZevaluator, *tupleFilter);

  plotterFactory = analysisFactory->createPlotterFactory();
  plotter = plotterFactory->create();
	assert(plotter);

	std::cout << "PLotter created" << std::endl;
	
  plotter->show();  
  plotter->currentRegion().plot(*histoEn,"");
  cout << "Press <ENTER> to continue" << endl;
  cin.get();
  plotter->hide(); 


  plotter->show();  
  plotter->currentRegion().plot(*histoX,"");
  cout << "Press <ENTER> to continue" << endl;
  cin.get();
  plotter->hide(); 

  plotter->show();  
  plotter->currentRegion().plot(*histoY,"");
  cout << "Press <ENTER> to continue" << endl;
  cin.get();
  plotter->hide(); 

  plotter->show();  
  plotter->currentRegion().plot(*histoZ,"");
  cout << "Press <ENTER> to continue" << endl;
  cin.get();
  plotter->hide(); 
  
  inputTree->close();
  
  std::cout << "Test run OK." << std::endl;

}

