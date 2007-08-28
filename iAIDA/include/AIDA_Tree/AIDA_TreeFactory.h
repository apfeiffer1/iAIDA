// Emacs -*- C++ -*-
#ifndef ANAPHE_TREEFACTORY
#define ANAPHE_TREEFACTORY 1

#include <string>
#include "AIDA/ITreeFactory.h"

// type ahead declarations 
namespace AIDA {
  class ITree;
}

namespace iAIDA {
  namespace aida_tree {

/// Implementation of the AIDA::ITreeFactory abstract interface

class AIDA_TreeFactory : public virtual AIDA::ITreeFactory
{
public:
  /// Constructor
  AIDA_TreeFactory();
  /// Destructor
  ~AIDA_TreeFactory();
  /// Methods inherited from the ITreeFactory class
  AIDA::ITree* create();
  AIDA::ITree* create( const std::string & storeName,
		       const std::string & storeType = "",
		       bool readOnly = false,
		       bool createNew = true,
		       const std::string & options = "" );

// protected: 
//   AIDA::ITree* m_create( const std::string & storeName,
// 		       const std::string & storeType = "",
// 		       bool readOnly = false,
// 		       bool createNew = true,
// 		       const std::string & options = "" );

};

  }
}

#endif /* ifndef ANAPHE_TREEFACTORY */
