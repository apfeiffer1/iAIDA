// make binding to pure AIDA
// create Aida AF

#include "AIDA/AIDA.h"
#include <vector>
#include <string>

namespace iAIDA { 

  class AFCreator { 

    public : 
      
      AFCreator() {}
    ~AFCreator() {}
    
    AIDA::IAnalysisFactory* createAnalysisFactory() { 
      return AIDA_createAnalysisFactory();
    }
  };

  //  std::vector<double> vd; 
  //std::vector<int> vi; 
  //std::vector<std::string> vs; 

}

// instantiate vectors

#ifndef _WIN32
std::vector<AIDA::IEvaluator *> ve; 
std::vector<AIDA::ITuple *> vt; 
#endif
