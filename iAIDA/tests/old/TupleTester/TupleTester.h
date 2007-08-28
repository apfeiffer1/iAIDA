#ifndef TUPLETESTER_H
#define TUPLETESTER_H 1

#include <string>
#include <iosfwd>

namespace AIDA {
  namespace Dev {
    class IBackingStore;
    class IDevTupleFactory;
  }
}

class TupleTester {
public:
  TupleTester();
  ~TupleTester();

  double testExpressionParser( std::ostream& os );
  double testFilterCreation( std::ostream& os );
  double testEvaluatorCreation( std::ostream& os );
  double testCompiledExpressionManager( std::ostream& os );
  double testDescriptionBuilder( std::ostream& os );
  double testCreateAndFillTuple( std::ostream& os,
				 AIDA::Dev::IBackingStore& store,
				 AIDA::Dev::IDevTupleFactory& factory,
				 const std::string& tuplePath );
  double testRetrieveAndReadTuple( std::ostream& os,
				   AIDA::Dev::IBackingStore& store,
				   AIDA::Dev::IDevTupleFactory& factory,
				   const std::string& tuplePath );
  double testGeant4Bug134( std::ostream& os );
};

#endif
