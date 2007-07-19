#include "TupleTester.h"
#include "ExpressionParser.h"
#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif


double
TupleTester::testExpressionParser( std::ostream& os )
{
  os << "*** Testing the expression parser ***" << std::endl;
  iAIDA::AIDA_Tuple_native::ExpressionParser parser;

  std::map<std::string, std::string> variableTypes;
  variableTypes[ "n" ] = "int";
  variableTypes[ "b" ] = "bool";
  variableTypes[ "energy" ] = "double";
  variableTypes[ "mass" ] = "double";
  variableTypes[ "particles" ] = "AIDA::ITuple";

  os << "Variables defined: " << std::endl;
  for ( std::map<std::string, std::string>::const_iterator iVariable = variableTypes.begin();
	iVariable != variableTypes.end(); ++iVariable ) {
    os << iVariable->first << " : " << iVariable->second << std::endl;
  }

  os << "Defining which header files to include and which namespaces to use." << std::endl;
  std::set<std::string> headerFilesToUse;
  headerFilesToUse.insert("<cmath>");
  headerFilesToUse.insert("\"AIDA_Dev/IDevTuple.h\"");
  parser.useHeaderFiles( headerFilesToUse );
  std::set<std::string> namespacesToUse;
  namespacesToUse.insert( "std" );
  parser.useNamespaces( namespacesToUse );

  const std::string logicalFunctionName = "aLogicalFunctionName";
  std::string expression = "( n*(energy/mass) < 1232.02 ) && b";

  os << "Forming the logical function for the expression : " << std::endl << expression << std::endl << std::endl;
  parser.logicalEvaluation( expression, variableTypes, logicalFunctionName, os );
  os << std::endl;
  expression = "int i = n-1;\nif (particles.rows() > i) {\n  return true;\n} else return false;";
  os << "Forming the logical function for the expression : " << std::endl << expression << std::endl;
  os << std::endl;
  parser.logicalEvaluation( expression, variableTypes, logicalFunctionName, os );
  os << std::endl;

  const std::string mathematicalFunctionName = "aMathematicalFunctionName";
  expression = "energy/sqrt(mass)";
  os << "Forming the mathematical function for the expression : " << std::endl << expression << std::endl;
  os << std::endl;
  parser.mathematicalEvaluation( expression, variableTypes, mathematicalFunctionName, os );
  os << std::endl;

  expression = "double totalEnergy = 0;\n";
  expression += "int energyColumn = particles.findColumn( \"energy\" );\n";
  expression += "particles.start();\n";
  expression += "for ( int iParticle = 0; iParticle < particles.rows(); ++iParticle) {\n";
  expression += "  particles.next();\n";
  expression += "  totalEnergy += particles.getDouble(energyColumn);\n";
  expression += "}\n";
  expression += "return totalEnergy;";
  os << "Forming the mathematical function for the expression : " << std::endl << expression << std::endl;
  os << std::endl;
  parser.mathematicalEvaluation( expression, variableTypes, mathematicalFunctionName, os );
  os << std::endl;

  return 1.0;
}
