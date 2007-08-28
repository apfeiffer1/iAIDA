#include "AIDA/AIDA.h"

int main() {

  AIDA::IAnalysisFactory* analysisFactory  = AIDA_createAnalysisFactory();

  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();

  AIDA::ITree* tree = treeFactory->create();
  delete treeFactory;

  AIDA::IHistogramFactory* histogramFactory =
    analysisFactory->createHistogramFactory(*tree);

  AIDA::IHistogram1D* aidaObject = histogramFactory->createHistogram1D("10",2,0.0,2.0);
  delete histogramFactory;

  aidaObject->fill(0.5);
  aidaObject->fill(1.5);

  delete tree;
  delete analysisFactory;

}


