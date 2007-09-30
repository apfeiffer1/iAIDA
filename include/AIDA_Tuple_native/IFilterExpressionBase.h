// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE
#define IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE 1

#include <string>
#include <map>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class IFilterExpressionBase {
public:
  virtual ~IFilterExpressionBase() {};
  virtual bool bind( const std::map<std::string, void*>& args ) = 0;
  virtual bool accept() const = 0;
};

class IFilterExpressionBaseFactory {
public:
  virtual ~IFilterExpressionBaseFactory() {};
  virtual IFilterExpressionBase* create() = 0;
};

template<class T>
class IFilterExpressionFactory : virtual public IFilterExpressionBaseFactory {
public:
  IFilterExpressionFactory() {};
  ~IFilterExpressionFactory() {};
  IFilterExpressionBase* create() { return new T; }  
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE */
