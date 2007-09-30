// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_AIDA_FILTER
#define IAIDA_AIDA_TUPLE_AIDA_FILTER 1

#include "AIDA/IFilter.h"
#include "AIDA_Tuple_native/IFilterExpressionBase.h"
#include <memory>

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class CompiledExpressionManager;

class AIDA_Filter : virtual public AIDA::IFilter {
public:
  // Constructor
  AIDA_Filter( CompiledExpressionManager& manager,
	       const std::string& expression );
  // Destructor
  virtual ~AIDA_Filter(){};

  // Methods inherited from AIDA/IFilter
  virtual bool initialize( AIDA::ITuple & tuple );
  virtual bool accept() const;
  std::string  expression() const;

protected : 
  std::string m_cleanupExpression( ) const;

private:
  CompiledExpressionManager&                     m_manager;
  std::string                                    m_expression;
  std::auto_ptr<IFilterExpressionBase>           m_filter;
  bool                                           m_simpleFilter;      // to flag simple cases as "1" or "0" 
  bool                                           m_accept;            // used in the simple cases 
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_FILTER */
