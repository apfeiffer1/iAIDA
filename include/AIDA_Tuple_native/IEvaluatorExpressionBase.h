// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE
#define IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE 1

#include <string>
#include <map>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class IEvaluatorExpressionBase {
public:
  virtual ~IEvaluatorExpressionBase() {};
  virtual bool bind( const std::map<std::string, void*>& args ) = 0;
  virtual double evaluate() const = 0;
};

class IEvaluatorExpressionBaseFactory {
public:
  virtual ~IEvaluatorExpressionBaseFactory() {};
  virtual IEvaluatorExpressionBase* create() = 0;
};

template<class T>
class IEvaluatorExpressionFactory : virtual public IEvaluatorExpressionBaseFactory {
public:
  IEvaluatorExpressionFactory() {};
  ~IEvaluatorExpressionFactory() {};
  IEvaluatorExpressionBase* create() { return new T; }  
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE */
