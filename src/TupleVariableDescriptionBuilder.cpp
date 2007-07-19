#include "AIDA/ITuple.h"
#include "TupleVariableDescriptionBuilder.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/ITupleHeader.h"
#include <list>

iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::TupleVariableDescriptionBuilder( AIDA::Dev::IDevTupleFactory& factory ):
  m_factory( factory )
{}


bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::buildDescription( AIDA::Dev::ITupleHeader& header,
									      const std::string& descriptionString )
{
  std::vector<std::string> names;
  std::vector<std::string> types;
  if ( ! decompose( descriptionString, names, types ) ) return false;
  else return buildDescription( header, names, types );
}


bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::buildDescription( AIDA::Dev::ITupleHeader& header,
									      const std::vector<std::string>& variableNames,
									      const std::vector<std::string>& variableTypes )
{
  return buildDescriptionT( header, variableNames, variableTypes );
}


bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::buildDescription( AIDA::Dev::ITupleVariableDescription& description,
									      const std::string& descriptionString )
{
  std::vector<std::string> names;
  std::vector<std::string> types;
  if ( ! decompose( descriptionString, names, types ) ) return false;
  else return buildDescription( description, names, types );
}


bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::buildDescription( AIDA::Dev::ITupleVariableDescription& description,
									      const std::vector<std::string>& variableNames,
									      const std::vector<std::string>& variableTypes )
{
  return buildDescriptionT( description, variableNames, variableTypes );
}


static std::string cleanup( const std::string& input )
{
  std::string description = input;
  bool updated = true;
  while ( updated ) {
    bool updatedLocal = false;
    // First remove trailing spaces and semicolons
    while ( description.size() > 0 ) {
      char last = description[ description.size() - 1 ];
      if ( last == ' ' || last == ';' || last == '\n' || last == '\r' || last == '\t' ) {
	description = description.substr( 0, description.size() - 1 );
	updatedLocal = true;
      }
      else {
	break;
      }
    }

    // Remove starting spaces or semicolons
    while ( description.size() > 0 ) {
      char first = description[0];
      if ( first == ' ' || first == ';' || first == '\n' || first == '\r' || first == '\t' ) {
	description = description.substr( 1 );
	updatedLocal = true;
      }
      else {
	break;
      }
    }

    // Remove outer {} pairs
    while ( description.size() > 1 ) {
      char first = description[0];
      char last = description[ description.size() - 1 ];
      if ( first == '{' && last == '}' ) {
	description = description.substr( 1, description.size() - 2 );
	updatedLocal = true;
      }
      else {
	break;
      }
    }
    updated = updatedLocal;
  }

  return description;
}


static bool tokenize( const std::string& description, std::list< std::string >& pieces, char separator )
{
  int numberOfBraces = 0;
  unsigned int firstChar = 0;
  unsigned int lastChar = 0;
  for ( unsigned int i = 0; i < description.size(); ++i ) {
    char c = description[i];
    if ( c == separator ) {
      if ( numberOfBraces == 0 ) { // Check that we are outside a bace pair
	if ( firstChar == i ) { // Ignore consequtive colons
	  ++firstChar;
	}
	else {
	  pieces.push_back( description.substr( firstChar, i-firstChar ) );
	  firstChar = i+1;
	  lastChar = i;
	}
      }
    }
    else if ( c == '{' ) {
      ++numberOfBraces;
    }
    else if ( c == '}' ) {
      --numberOfBraces;
      if ( numberOfBraces < 0 ) return false;
    };
  }

  if ( lastChar != description.size() ) pieces.push_back( description.substr( firstChar ) );

  if ( numberOfBraces != 0 ) return false;
  else return true;
}

bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::decompose( const std::string& inputRaw,
								       std::vector<std::string>& variableNames,
								       std::vector<std::string>& variableTypes ) const
{

  std::string input = inputRaw;
  // first replace all ';' by ',':
  for ( unsigned int i = 0; i < input.size(); ++i ) {
    if( input[i] == ';' ) input[i] = ',';
  }

  // Now break the description into pieces seperated by , unless they are withing braces
  std::list< std::string > pieces;
  if ( ! tokenize( cleanup( input ), pieces, ',' ) ) return false;

    // ... loop over the pieces and extract variable type from name
  std::string varType = ""; // set some default ...
  std::string varName = "";
  for ( std::list< std::string >::const_iterator iPiece = pieces.begin();
	iPiece != pieces.end(); ++iPiece ) {
    std::string description = cleanup( *iPiece );
    if ( description == "" ) { continue; } // ignore empty strings

    // Extract the type.
    unsigned int varNameStart = 0;
    for ( unsigned int i = 0; i < description.size(); ++i ) {
      char c = description[i];
      if ( c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '{' ) {
	varType = description.substr( 0, i );
	varNameStart = i;
	break;
      }
    }

    // Check if the variable type is a tuple
    if ( varType.find( "uple" ) != std::string::npos ) {
      int braces = 0;
      for ( unsigned int i = varType.size(); i < description.size(); ++i ) {
	char c = description[i];
	varType += c;
	if ( c == '{' ) ++braces;
	else if ( c == '}' ) {
	  --braces;
	  if ( braces == 0 ) {
	    varNameStart = i+1;
	    break;
	  }
	}
      }
    }

    varName = description.substr( varNameStart, description.size() );

    variableNames.push_back( cleanup( varName ) );
    variableTypes.push_back( varType );
  }

  return true;
}

 /*-ap 
bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::decompose( const std::string& input,
								       std::vector<std::string>& variableNames,
								       std::vector<std::string>& variableTypes ) const
{

  std::cerr << "\n\n--------------------------------------------------------------------------------\n" << std::endl;
  std::cerr << "analyzing '" << input << "'" << std::endl;

  // Break the description into pieces seperated by ; unless they are withing braces
  std::list< std::string > pieces;
  if ( ! tokenize( cleanup( input ), pieces, ';' ) ) return false;

  // Now loop over the pieces and extract variable type from name
  for ( std::list< std::string >::const_iterator iPiece = pieces.begin();
	iPiece != pieces.end(); ++iPiece ) {
    std::string description = cleanup( *iPiece );


    // Extract the type.
    std::string varType = "";
    for ( unsigned int i = 0; i < description.size(); ++i ) {
      char c = description[i];
      if ( c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '{' ) {
	varType = description.substr( 0, i );
	break;
      }
    }

    // Check if the variable type is a tuple
    if ( varType.find( "uple" ) != std::string::npos ) {
      int braces = 0;
      for ( unsigned int i = varType.size(); i < description.size(); ++i ) {
	char c = description[i];
	varType += c;
	if ( c == '{' ) ++braces;
	else if ( c == '}' ) {
	  --braces;
	  if ( braces == 0 ) break;
	}
      }
    }

    if ( varType != "" ) {
      std::list< std::string> varNames;
      if ( tokenize( cleanup( description.substr( varType.size() ) ), varNames, ',' ) ) {
	for ( std::list< std::string>::const_iterator iVarName = varNames.begin();
	      iVarName != varNames.end(); ++iVarName ) {
	  variableNames.push_back( cleanup( *iVarName ) );
	  variableTypes.push_back( varType );
	}
      }
    }
  }

  std::cerr << "decomposition:" << std::endl;
  for (unsigned int i=0; i< variableNames.size(); i++ ) {
    std::cerr << "type '" << variableTypes[i] << "' name '" << variableNames[i] << "'" << std::endl;
  }

  return true;
}
 */
bool
iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::compose(const AIDA::ITuple & tp, std::string& descriptionString) 
{ 
  // compose a desciption string using names and types 
  // used by subtuples 
  static const std::string tupleType = "AIDA::ITuple";

  for ( int j = 0; j <  tp.columns() ; ++j) { 
    if (tp.columnType(j) != tupleType) 
      descriptionString += " " + tp.columnType(j) + "  " + tp.columnName(j);
    else {
      descriptionString += tupleType + "{ ";
      const AIDA::ITuple *stp = tp.getTuple(j); 
      compose(*stp,descriptionString);
      
      descriptionString += " } " + tp.columnName(j);
    }
     
    if (j != tp.columns()-1)  descriptionString += ";";
    
  }
  return true;
}
