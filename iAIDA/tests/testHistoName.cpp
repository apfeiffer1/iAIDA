//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "AIDA/AIDA.h"

int main() {

  AIDA::IAnalysisFactory* analysisFactory  = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();

  bool readOnly = false;
  bool createNew = true;
  AIDA::ITree* tree = treeFactory->create("testHistoName.root","root",readOnly, createNew);
  delete treeFactory;

  AIDA::IHistogramFactory* histogramFactory = analysisFactory->createHistogramFactory(*tree);

  AIDA::IHistogram1D* h1 = histogramFactory->createHistogram1D("h_1","doseV1 dose in rad",2,0.0,2.0);
  AIDA::IHistogram1D* h2 = histogramFactory->createHistogram1D("doseV1_dose","doseV1 dose in rad",2,0.0,2.0);

  delete histogramFactory;

  h1->fill(0.5);
  h2->fill(1.5);

  tree->commit();
  tree->close();

  delete tree;
  delete analysisFactory;

}


