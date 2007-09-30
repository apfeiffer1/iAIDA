// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE
#define IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE 1

#include "AIDA_Filter.h"

namespace AIDA {
  namespace Dev {
    class ITupleHeader;
  }
}

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_FilterRange : public AIDA_Filter
{
public:
  // Constructor
  AIDA_FilterRange( CompiledExpressionManager& manager,
		    const std::string& expression,
		    int numberOfRows,
		    int startingRow );
  // Destructor
  ~AIDA_FilterRange(){};

  // Methods inherited from AIDA/IFilter
  bool initialize( AIDA::ITuple & tuple );
  bool accept() const;
 
private:
  int                               m_startingRow;
  int                               m_endingRow;
  AIDA::Dev::ITupleHeader*          m_header;
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_FILTERRANGE */
