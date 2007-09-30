//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ExpressionParser.h"

#ifdef OLDSTREAMS
# include <iostream>
# include <strstream>
# define istringstream istrstream
#else
# include <ostream>
# include <sstream>
#endif

static bool areTablesInitialized = false;

static std::set<char> forbiddenCharactersInVariables = std::set<char>();

static void
initializeTables() {
  forbiddenCharactersInVariables.insert( ' ' );
  forbiddenCharactersInVariables.insert( '`' );
  forbiddenCharactersInVariables.insert( '~' );
  forbiddenCharactersInVariables.insert( '!' );
  forbiddenCharactersInVariables.insert( '@' );
  forbiddenCharactersInVariables.insert( '#' );
  forbiddenCharactersInVariables.insert( '$' );
  forbiddenCharactersInVariables.insert( '%' );
  forbiddenCharactersInVariables.insert( '^' );
  forbiddenCharactersInVariables.insert( '&' );
  forbiddenCharactersInVariables.insert( '*' );
  forbiddenCharactersInVariables.insert( '(' );
  forbiddenCharactersInVariables.insert( ')' );
  forbiddenCharactersInVariables.insert( '-' );
  forbiddenCharactersInVariables.insert( '=' );
  forbiddenCharactersInVariables.insert( '+' );
  forbiddenCharactersInVariables.insert( '|' );
  forbiddenCharactersInVariables.insert( '\\' );
  forbiddenCharactersInVariables.insert( '{' );
  forbiddenCharactersInVariables.insert( '}' );
  forbiddenCharactersInVariables.insert( '[' );
  forbiddenCharactersInVariables.insert( ']' );
  forbiddenCharactersInVariables.insert( ';' );
  forbiddenCharactersInVariables.insert( ':' );
  forbiddenCharactersInVariables.insert( '"' ); // It look as if this is ignored
  forbiddenCharactersInVariables.insert( '\'' );
  forbiddenCharactersInVariables.insert( '<' );
  forbiddenCharactersInVariables.insert( ',' );
  forbiddenCharactersInVariables.insert( '>' );
  forbiddenCharactersInVariables.insert( '.' );
  forbiddenCharactersInVariables.insert( '?' );
  forbiddenCharactersInVariables.insert( '/' );

  // other white spaces or funny characters
  forbiddenCharactersInVariables.insert( '\n' );
  forbiddenCharactersInVariables.insert( '\t' );
  forbiddenCharactersInVariables.insert( '\r' );

  areTablesInitialized = true;
}


iAIDA::AIDA_Tuple_native::ExpressionParser::ExpressionParser():
  m_headerFiles(),
  m_namespaces()
{
  if ( ! areTablesInitialized ) initializeTables();
}

iAIDA::AIDA_Tuple_native::ExpressionParser::~ExpressionParser()
{}


bool
iAIDA::AIDA_Tuple_native::ExpressionParser::useHeaderFiles( const std::set< std::string >& headerFiles )
{
  m_headerFiles = headerFiles;
  return true;
}


const std::set<std::string>&
iAIDA::AIDA_Tuple_native::ExpressionParser::headerFilesUsed() const
{
  return m_headerFiles;
}


bool
iAIDA::AIDA_Tuple_native::ExpressionParser::useNamespaces( const std::set< std::string >& namespaces )
{
  m_namespaces = namespaces;
  return true;
}


const std::set<std::string>&
iAIDA::AIDA_Tuple_native::ExpressionParser::namespacesUsed() const
{
  return m_namespaces;
}


std::set<std::string>
iAIDA::AIDA_Tuple_native::ExpressionParser::mathematicalEvaluation( const std::string& expression,
								     const std::map< std::string, std::string >& variablesToTypes,
								     const std::string& functionName,
								     std::ostream& os ) const
{
  os << "/* START OF AUTOMATICALLY GENERATED FILE */" << std::endl;
  std::set<std::string> variablesUsed = findUsedVariables( expression, variablesToTypes );

  // include the base class header file
  os << "#include \"AIDA_Tuple_native/IEvaluatorExpressionBase.h\"" << std::endl;

  // include the other header files
  for ( std::set<std::string>::const_iterator iHeaderFile = m_headerFiles.begin();
	iHeaderFile != m_headerFiles.end(); ++iHeaderFile ) {
    os << "#include "<< *iHeaderFile << std::endl;
  }
  os << std::endl;
  for ( std::set<std::string>::const_iterator iNamespace = m_namespaces.begin();
	iNamespace != m_namespaces.end(); ++iNamespace ) {
    os << "using namespace "<< *iNamespace << ";" << std::endl;
  }

  // declaring this class
  std::string theClassName = "EvaluatorExpression_" + functionName;
  os << std::endl
     << "class " << theClassName << " : virtual public iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase {" << std::endl
     << "public:" << std::endl
     << "  " << theClassName << "(){}" << std::endl
     << "  ~" << theClassName << "(){}" << std::endl
     << "  bool bind( const std::map<std::string, void*>& args ) {" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    os << "    m_" << variableName << " = reinterpret_cast< const "
       << variableType << " *>( args.find(\"" << variableName << "\")->second );" << std::endl;
  }
  os << "    return true;" << std::endl
     << "  };" << std::endl
     << "  double evaluate() const {" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    // LM : return if pointers are invalid
    os << "    if ( ! m_" << variableName << " ) return 0.0; " << std::endl; 
    os << "    const " << variableType << "& " << variableName << " = * m_" << variableName << ";" << std::endl;
  }
  if ( existsVariableInExpression( "return", expression ) ) {
    os << "    " << correctIdentation( expression ) << std::endl;
  }
  else {
    std::istringstream is( expression.c_str() );
    std::string str = "";
    is >> str;
    if ( str == "" ) {
      os << "    return 1.0;" << std::endl;
    }
    else {
      os << "    return ( " << expression << " );" << std::endl;
    }
  }
  os << "  }" << std::endl
     << "private:" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    os << "  const " << variableType << "* m_" << variableName << ";" << std::endl;
  }
  os << "};" << std::endl
     << std::endl;

  // Declaration of the factory
  os << "static iAIDA::AIDA_Tuple_native::IEvaluatorExpressionFactory<" << theClassName << "> factory;" << std::endl
     << "iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBaseFactory& " << functionName << " = factory;\n" << std::endl;

  os << "/* END OF AUTOMATICALLY GENERATED FILE */" << std::endl;
  return variablesUsed;
}


std::set<std::string>
iAIDA::AIDA_Tuple_native::ExpressionParser::logicalEvaluation( const std::string& expression,
								const std::map< std::string, std::string >& variablesToTypes,
								const std::string& functionName,
								std::ostream& os ) const
{
  os << "/* START OF AUTOMATICALLY GENERATED FILE */" << std::endl;
  std::set<std::string> variablesUsed = findUsedVariables( expression, variablesToTypes );

  // include the base class header file
  os << "#include \"AIDA_Tuple_native/IFilterExpressionBase.h\"" << std::endl;

  // include the other header files
  for ( std::set<std::string>::const_iterator iHeaderFile = m_headerFiles.begin();
	iHeaderFile != m_headerFiles.end(); ++iHeaderFile ) {
    os << "#include "<< *iHeaderFile << std::endl;
  }
  os << std::endl;
  for ( std::set<std::string>::const_iterator iNamespace = m_namespaces.begin();
	iNamespace != m_namespaces.end(); ++iNamespace ) {
    os << "using namespace "<< *iNamespace << ";" << std::endl;
  }

  // declaring this class
  std::string theClassName = "FilterExpression_" + functionName;
  os << std::endl
     << "class " << theClassName << " : virtual public iAIDA::AIDA_Tuple_native::IFilterExpressionBase {" << std::endl
     << "public:" << std::endl
     << "  " << theClassName << "(){}" << std::endl
     << "  ~" << theClassName << "(){}" << std::endl
     << "  bool bind( const std::map<std::string, void*>& args ) {" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    os << "    m_" << variableName << " = reinterpret_cast< const "
       << variableType << " *>( args.find(\"" << variableName << "\")->second );" << std::endl;
  }
  os << "    return true;" << std::endl
     << "  };" << std::endl
     << "  bool accept() const {" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    // LM : return if pointers are invalid
    os << "    if ( ! m_" << variableName << " ) return false; " << std::endl; 
    os << "    const " << variableType << "& " << variableName << " = * m_" << variableName << ";" << std::endl;
  }
  if ( existsVariableInExpression( "return", expression ) ) {
    os << "    " << correctIdentation( expression ) << std::endl;
  }
  else {
    std::istringstream is( expression.c_str() );
    std::string str = "";
    is >> str;
    if ( str == "" ) {
      os << "    return true;" << std::endl;
    }
    else {
      os << "    return ( " << expression << " );" << std::endl;
    }
  }
  os << "  }" << std::endl
     << "private:" << std::endl;
  for ( std::set<std::string>::const_iterator iArgument = variablesUsed.begin();
	iArgument != variablesUsed.end(); ++iArgument ) {
    const std::string& variableName = *iArgument;
    std::string variableType = variablesToTypes.find( variableName )->second;
    if ( variableType == "AIDA::ITuple" ) variableType = "AIDA::Dev::IDevTuple";
    os << "  const " << variableType << "* m_" << variableName << ";" << std::endl;
  }
  os << "};" << std::endl
     << std::endl;

  // Declaration of the factory
  os << "static iAIDA::AIDA_Tuple_native::IFilterExpressionFactory<" << theClassName << "> factory;" << std::endl
     << "iAIDA::AIDA_Tuple_native::IFilterExpressionBaseFactory& " << functionName << " = factory;\n" << std::endl;

  os << "/* END OF AUTOMATICALLY GENERATED FILE */" << std::endl;
  return variablesUsed;
}


std::set<std::string>
iAIDA::AIDA_Tuple_native::ExpressionParser::findUsedVariables( const std::string& expression,
								const std::map< std::string, std::string >& variablesToTypes ) const
{
  std::set<std::string> variablesUsed;
  for ( std::map< std::string, std::string >::const_iterator iVariable = variablesToTypes.begin();
	iVariable != variablesToTypes.end(); ++iVariable ) {
    const std::string& variableName = iVariable->first;
    if ( existsVariableInExpression( variableName, expression ) ) variablesUsed.insert( variableName );
  }
  return variablesUsed;
}


bool
iAIDA::AIDA_Tuple_native::ExpressionParser::existsVariableInExpression( const std::string& variable,
									 const std::string& expression ) const
{
  bool foundVariable = false;
  for ( std::string::size_type pos = expression.find( variable, 0 );
	pos != std::string::npos;
	pos = expression.find( variable, pos + variable.size() ) ) {
    bool isCharBeforeGood = true;
    if ( pos > 0 ) { // check the previous character
      char c = expression[pos-1];
      if ( forbiddenCharactersInVariables.find( c ) == forbiddenCharactersInVariables.end() ) {
	isCharBeforeGood = false;
      }
      else if ( c == '\"' ) isCharBeforeGood = false; // For some funny reason we have to include this !!!
    }
    bool isCharAfterGood = true;
    std::string::size_type posAfter = pos + variable.size();
    if ( isCharBeforeGood && posAfter < expression.size() ) { // check the character after
      char c = expression[posAfter];
      if ( forbiddenCharactersInVariables.find( c ) == forbiddenCharactersInVariables.end() ) {
	isCharAfterGood = false;
      }
      else if ( c == '\"' ) isCharAfterGood = false; // For some funny reason we have to include this !!!
    }
    if ( isCharBeforeGood && isCharAfterGood ) {
      foundVariable = true;
      break;
    }
  }
  return foundVariable;
}


std::string
iAIDA::AIDA_Tuple_native::ExpressionParser::correctIdentation( const std::string& expression ) const
{
  std::string output = "";
  for ( unsigned int iChar = 0; iChar < expression.size(); ++iChar ) {
    char c = expression[iChar];
    output += c;
    if ( c == '\n' || c == '\r' ) output += "    ";
  }
  return output;
}
