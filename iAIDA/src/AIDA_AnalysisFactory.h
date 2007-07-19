// Project   : LCG
// Package   : src
// Author    : Lorenzo.MONETA@cern.ch 
// Created by: moneta  at Mon Mar  8 09:54:02 2004

#ifndef SRC_AIDA_ANALYSISFACTORY_H
#define SRC_AIDA_ANALYSISFACTORY_H 1


#include "AIDA/IAnalysisFactory.h"

#include "AIDA_Plugin/AIDA_PluginType.h"
#include "boost/shared_ptr.hpp"
typedef boost::shared_ptr<AIDA_Plugin::AIDA_PluginType> PluginType;


namespace iAIDA
{ 

/**
 * PUT YOUR COMMENT HERE.
 * This class does not support copying.
 */
  class AIDA_AnalysisFactory : public virtual AIDA::IAnalysisFactory 
{

public: 
  AIDA_AnalysisFactory(); 
  virtual ~AIDA_AnalysisFactory(); 

private:
  /// copying unimplemented in this class.  
  AIDA_AnalysisFactory(const AIDA_AnalysisFactory &); 
  /// copying unimplemented in this class.  
  AIDA_AnalysisFactory & operator = (const AIDA_AnalysisFactory &); 

public:

  /// Methods inherited from the IAnalysisFactory class
  AIDA::ITreeFactory * createTreeFactory();
  AIDA::IHistogramFactory * createHistogramFactory( AIDA::ITree& tree );
  AIDA::IDataPointSetFactory * createDataPointSetFactory( AIDA::ITree& tree );
  AIDA::ITupleFactory * createTupleFactory( AIDA::ITree& tree );
  AIDA::IFunctionFactory * createFunctionFactory( AIDA::ITree& tree );
  AIDA::IPlotterFactory * createPlotterFactory( int argc = 0, char** args = 0, const std::string & imp = "", const std::string & lib = ""  );
  AIDA::IFitFactory * createFitFactory();

 protected: 

private:  
  std::string m_treePlugin; 
  std::string m_histogramPlugin;
  std::string m_dataPointSetPlugin;
  std::string m_tuplePlugin;
  std::string m_functionPlugin;
  std::string m_fitPlugin;
  std::string m_plotterPlugin;

}; 

} // end namespace iAIDA
#endif 
