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

// Project   : LCG
// Package   : src
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Mon Mar  8 09:54:02 2004


#include "src/AIDA_AnalysisFactory.h"

#include "AIDA_Tree/AIDA_TreeFactory.h"
#include "AIDA_Fitter/AIDA_FitFactory.h"

#include "AIDA_Histogram_native/AIDA_HistogramFactory.h"
#include "AIDA_DataPointSet_native/AIDA_DataPointSetFactory.h"
#include "AIDA_Function/AIDA_FunctionFactory.h"
#include "AIDA_Tuple_native/AIDA_TupleFactory.h"

#ifdef HAVE_GRACE
#include "AIDAGracePlotter/AIDA_PlotterFactory.h"
#endif

#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "AIDA_DataPointSet_native/AIDA_DevDataPointSetFactory.h"
#include "AIDA_Function/AIDA_DevFunctionFactory.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"
#include "AIDA_Tuple_native/AIDA_DevFilterFactory.h"
#include "AIDA_Tuple_native/AIDA_DevEvaluatorFactory.h"

#include <boost/shared_ptr.hpp>


// default implementations: 

static const std::string defaultTreePlugin = "AIDA_Tree_pi";
static const std::string defaultHistogramPlugin = "AIDA_Histogram_Native";
static const std::string defaultDataPointSetPlugin = "AIDA_DataPointSet_Native";
static const std::string defaultTuplePlugin = "AIDA_Tuple_Native";
static const std::string defaultFunctionPlugin = "AIDA_Function_Native";
static const std::string defaultFitPlugin = "AIDA_Fitter_Native";
static const std::string defaultPlotterPlugin = "AIDA_Plotter_Lab";


// C entry point 
AIDA::IAnalysisFactory* AIDA_createAnalysisFactory()
{
  return new iAIDA::AIDA_AnalysisFactory();
}


namespace iAIDA
{ 

  AIDA_AnalysisFactory::AIDA_AnalysisFactory() : 
    m_treePlugin(defaultTreePlugin),
    m_histogramPlugin(defaultHistogramPlugin),
    m_dataPointSetPlugin(defaultDataPointSetPlugin),
    m_tuplePlugin(defaultTuplePlugin),
    m_functionPlugin(defaultFunctionPlugin),
    m_fitPlugin(defaultFitPlugin),
    m_plotterPlugin(defaultPlotterPlugin),
    devHiF(0), devDpsF(0), devTpF(0), devFiF(0),devEvF(0),devFuF(0)
{
}

AIDA_AnalysisFactory::~AIDA_AnalysisFactory() 
{
  delete devHiF;
  delete devDpsF;
  delete devTpF;
  delete devFiF;
  delete devEvF;
  delete devFuF;
}

AIDA_AnalysisFactory::AIDA_AnalysisFactory(const AIDA_AnalysisFactory &) 
{
}

AIDA_AnalysisFactory & AIDA_AnalysisFactory::operator = (const AIDA_AnalysisFactory &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}



AIDA::ITreeFactory*
AIDA_AnalysisFactory::createTreeFactory()
{
  return new aida_tree::AIDA_TreeFactory();
}


AIDA::IHistogramFactory*
AIDA_AnalysisFactory::createHistogramFactory( AIDA::ITree& tree )
{
  AIDA::Dev::IDevTree *devTree = dynamic_cast<AIDA::Dev::IDevTree *> (&tree);
  delete devHiF;		// remove old one if we have one
  devHiF = new AIDA_Histogram_native::AIDA_DevHistogramFactory();  
  return new AIDA_Histogram_native::AIDA_HistogramFactory( *devTree, *devHiF ); 
}


AIDA::IDataPointSetFactory*
AIDA_AnalysisFactory::createDataPointSetFactory( AIDA::ITree& tree )
{
  AIDA::Dev::IDevTree *devTree = dynamic_cast<AIDA::Dev::IDevTree *> (&tree);
  delete devDpsF;		// remove old one if we have one
  devDpsF = new AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory();  
  return new AIDA_DataPointSet_native::AIDA_DataPointSetFactory( *devTree, *devDpsF ); 
}


AIDA::ITupleFactory*
AIDA_AnalysisFactory::createTupleFactory( AIDA::ITree& tree )
{
  AIDA::Dev::IDevTree *devTree = dynamic_cast<AIDA::Dev::IDevTree *> (&tree);
  // delete old version of the dev-Factories first
  delete devTpF; devTpF = new AIDA_Tuple_native::AIDA_DevTupleFactory();  
  delete devFiF; devFiF = new AIDA_Tuple_native::AIDA_DevFilterFactory();  
  delete devEvF; devEvF = new AIDA_Tuple_native::AIDA_DevEvaluatorFactory();  
  return new AIDA_Tuple_native::AIDA_TupleFactory( *devTree, *devTpF, *devFiF, *devEvF ); 
}


AIDA::IFunctionFactory*
AIDA_AnalysisFactory::createFunctionFactory( AIDA::ITree& tree )
{
  AIDA::Dev::IDevTree *devTree = dynamic_cast<AIDA::Dev::IDevTree *> (&tree);
  delete devFuF;		// remove old one if we have one
  devFuF = new AIDA_Function::AIDA_DevFunctionFactory();  
  return new AIDA_Function::AIDA_FunctionFactory( *devTree, *devFuF ); 

}


AIDA::IFitFactory*
AIDA_AnalysisFactory::createFitFactory()
{
  return new AIDA_Fitter::AIDA_FitFactory(); 
}


AIDA::IPlotterFactory*
AIDA_AnalysisFactory::createPlotterFactory( int argc, char** args, const std::string & imp, const std::string & lib )
{
#ifdef HAVE_GRACE
  return new AidaNative::AIDA_Plotter_Grace::AIDA_PlotterFactory();
#else
  return 0;
#endif
}

} // end namespace iAIDA
