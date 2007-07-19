// Emacs -*- C++ -*-
#ifndef AIDANATIVE_FITFACTORY
#define AIDANATIVE_FITFACTORY 1

#include <string>
#include "AIDA/IFitFactory.h"

// type ahead declarations 
namespace AIDA {
  class IFitter;
}

namespace iAIDA {
  namespace AIDA_Fitter {

/// Implementation of the AIDA::IFitFactory abstract interface

class AIDA_FitFactory : public virtual AIDA::IFitFactory
{
public:
  /// Constructor
  AIDA_FitFactory();
  /// Destructor
  ~AIDA_FitFactory();
  /// Methods inherited from the IFitFactory class
  AIDA::IFitData* createFitData();
  AIDA::IFitter* createFitter( const std::string &fitterType = "Chi2",
			       const std::string &engineType = "",
			       const std::string &options = "");
};

  }
}

#endif /* ifndef AIDANATIVE_FITFACTORY */
