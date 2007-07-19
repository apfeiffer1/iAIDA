#include "CWNtuple.h"
#include "VectorTuple.h"
#include "SubTuple.h"
#include "HBook.h"

#include "AIDA_Dev/ITupleFactoryProvider.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"

#include <stdexcept>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

static const std::string emptyString = "";

pi::AIDA_HBookStore::CWNtuple::CWNtuple( const std::string& memoryDirectory,
					     int id ):
  m_dir( memoryDirectory ),
  m_id( id ),
  m_initializedReading( false )
{}


pi::AIDA_HBookStore::CWNtuple::~CWNtuple()
{
  for ( std::vector< std::pair< ColumnType, void* > >::iterator iObject = m_cache.begin();
	iObject != m_cache.end(); ++iObject ) {
    pi::AIDA_HBookStore::CWNtuple::ColumnType type = iObject->first;
    switch ( type ) {
    case pi::AIDA_HBookStore::CWNtuple::DOUBLE :
      delete reinterpret_cast< double * >( iObject->second );
      break;
    case pi::AIDA_HBookStore::CWNtuple::FLOAT :
      delete reinterpret_cast< float * >( iObject->second );
      break;
    case pi::AIDA_HBookStore::CWNtuple::INT :
      delete reinterpret_cast< int * >( iObject->second );
      break;
    case pi::AIDA_HBookStore::CWNtuple::BOOL :
      delete reinterpret_cast< int * >( iObject->second );
      break;
    case pi::AIDA_HBookStore::CWNtuple::STRING :
      delete reinterpret_cast< std::string * >( iObject->second );
      break;
    case pi::AIDA_HBookStore::CWNtuple::TUPLE :
      // do nothing; all the tuples will be cleared below
      break;
    };
  }

  for ( std::map< int, std::pair<char*, unsigned int> >::iterator iVar = m_charVariables.begin();
	iVar != m_charVariables.end(); ++iVar ) delete [] ( iVar->second ).first;

  for ( std::vector< std::pair< AIDA::Dev::IDevTuple*, int > >::iterator iTuple = m_devTuples.begin();
	iTuple != m_devTuples.end(); ++iTuple ) {
    delete iTuple->first;
  }
}


bool
pi::AIDA_HBookStore::CWNtuple::writeDescription( AIDA::Dev::ITupleHeader& header,
						     AIDA::Dev::IDevTupleFactory& tf,
						     AIDA::Dev::IBackingStore* store )
{
  // Collect the names first
  const std::string blockName = "VARBLOCK";
  for ( int i = 0; i < header.numberOfVariables(); ++i ) {
    AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( i );
    m_boundVariables.insert( i );
    m_names.push_back( description->variableName() );
    m_blockNames.push_back( blockName );
  };

  // Declare the variables to HBOOK
  const std::string options = " " + header.options();
  std::map< int, std::string > indexVariables;

  for ( int i = 0; i < header.numberOfVariables(); ++i ) {
    AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( i );
    const std::string type = description->variableType();
    const std::string name = description->variableName();
    if ( type != "std::string" ) {
      std::pair< std::string, void* > steeringStringAndAddress;
      if ( type == "double" ) {
	steeringStringAndAddress = writeDoubleDataDescription( description, options );
      }
      else if ( type == "float" ) {
	steeringStringAndAddress = writeFloatDataDescription( description, options );
      }
      else if ( type == "int" ) {
	steeringStringAndAddress = writeIntDataDescription( description, options );
      }
      else if ( type == "bool" ) {
	steeringStringAndAddress = writeBoolDataDescription( description, options );
      }
      else if ( type == "AIDA::ITuple" ) {
	steeringStringAndAddress = writeSubTupleDataDescription( i, description, options, header, tf, store, indexVariables );
      }
      else {
	throw std::runtime_error( "Variables of type \"" + type + "\" are not supported for CWN" );
      }
      pi::AIDA_HBookStore::HBook::describeCWNvariables( m_id, blockName,
							    steeringStringAndAddress.second,
							    steeringStringAndAddress.first );
    }
    else {
      std::pair< std::string, char* > steeringStringAndAddress = writeStringDataDescription( i, description, options );
      pi::AIDA_HBookStore::HBook::describeCWNvariablesChar( m_id, blockName,
								steeringStringAndAddress.second,
								steeringStringAndAddress.first );
    }
  }

  // Set the number of rows for the tuples which are indexed
  for ( std::map< int, std::string >::const_iterator iv = indexVariables.begin();
	iv != indexVariables.end(); ++iv ) {
    const std::string& varName = iv->second;
    for ( int i = 0; i < static_cast<int>( m_names.size() ); ++i ) {
      if ( m_names[i] == varName ) {
	m_devTuples[iv->first].second = -i;
	break;
      }
    }
  }

  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::readDescription( AIDA::Dev::ITupleHeader& header,
						    AIDA::Dev::IDevTupleFactory& tf,
						    AIDA::Dev::IBackingStore* store )
{
  std::string optionString = "";
  std::string title;
  std::vector<std::pair<float,float> > columnMinAndMax;
  m_names.clear();
  pi::AIDA_HBookStore::HBook::getTupleParameters( m_id, title, m_names, columnMinAndMax);
  m_blockNames.clear();
  std::vector< std::pair< pi::AIDA_HBookStore::CWNtuple::ColumnType, int > > typesAndDimensionality;
  for ( unsigned int i = 0; i < m_names.size(); ++i ) {
    std::pair< std::string, std::string > blockAndType =
      pi::AIDA_HBookStore::HBook::getCWNtupleVariableDescription( m_id,
								      static_cast<int>( i + 1 ) );

    // Overwrite the variable name. HBook gives it truncated with HGIVEN !!!
    std::string descriptionString = blockAndType.second;
    std::string correctName = "";
    for ( unsigned int ic = 0; ic < descriptionString.size(); ++ic ) {
      char c = descriptionString[ic];
      if ( c == ':' || c == '(' || c == '[' ) break;
      correctName += c;
    }
    m_names[i] = correctName;

    m_blockNames.push_back( blockAndType.first );
    optionString += " " + descriptionString;

    // Resolve the type
    std::string typeDescription = descriptionString.substr( blockAndType.second.find(":") + 1 );
    char typeLetter = typeDescription[0];

    // Resolve the dimensions
    int dimensionality = 0;
    std::string::size_type idx = blockAndType.second.find("(");
    if ( idx != std::string::npos ) {
      ++dimensionality;
      std::istringstream is( blockAndType.second.substr( idx + 1 ).c_str() );
      char c;
      while ( is >> c ) {
	if ( c == ',' ) ++dimensionality;
	else if ( c == ')' ) break;
      }
    }

    if ( dimensionality > 0 ) {
      if ( typeLetter != 'R' && typeLetter != 'I' && typeLetter != 'U' )
	throw std::runtime_error( "Unsupported sub-tuple type : " + typeLetter );
    }

    pi::AIDA_HBookStore::CWNtuple::ColumnType type;
    if ( typeLetter == 'R' ) {
      if ( typeDescription[2] == '8' ) {
	type = pi::AIDA_HBookStore::CWNtuple::DOUBLE;
      }
      else {
	type = pi::AIDA_HBookStore::CWNtuple::FLOAT;
      }
    }
    else if ( typeLetter == 'I' || typeLetter == 'U' ) {
      if ( typeDescription[2] != '4' ) throw std::runtime_error( "64 bit integers are not supported" );
      else {
	type = pi::AIDA_HBookStore::CWNtuple::INT;
      }
    }
    else if ( typeLetter == 'L' ) {
      type = pi::AIDA_HBookStore::CWNtuple::BOOL;
    }
    else if ( typeLetter == 'C' ) {
      type = pi::AIDA_HBookStore::CWNtuple::STRING;
    }
    else throw std::runtime_error( "Unrecognised type : " + typeLetter );
    typesAndDimensionality.push_back( std::make_pair( type, dimensionality ) );
  }

  int numberOfEntries = header.numberOfRows();
  std::map< int, std::string > indexVariables;
  for ( unsigned int iVar = 0; iVar < m_names.size(); ++iVar ) {
    AIDA::Dev::ITupleVariableDescription* description = tf.createDescription();
    description->setVariableName( m_names[iVar] );
    if ( ! header.setVariableDescription( description, true ) ) {
      throw std::runtime_error( "Could not declare a new variable named " + m_names[iVar] );
    }
    description->statistics().setStatistics( columnMinAndMax[iVar].first,
					     columnMinAndMax[iVar].second,
					     0, 0, numberOfEntries );

    pi::AIDA_HBookStore::CWNtuple::ColumnType type = typesAndDimensionality[iVar].first;

    // Create the descriptions and the internal structures
    if ( typesAndDimensionality[iVar].second > 0 ) { // sub tuples...
      description->setVariableType( "AIDA::ITuple" );
      AIDA::Dev::ITupleVariableDescription* desp = description;
      for ( int dimensionality = typesAndDimensionality[iVar].second - 1;
	    dimensionality > 0; --dimensionality ) {
	AIDA::Dev::ITupleVariableDescription* desd = tf.createDescription();
	desd->setVariableName( m_names[iVar] );
	desd->setVariableType( "AIDA::ITuple" );
	desp->setVariableDescription( desd, true );
	desp = desd;
      }
      AIDA::Dev::ITupleVariableDescription* desd = tf.createDescription();
      desp->setVariableDescription( desd, true );
      desd->setVariableName( m_names[iVar] );

      if ( type == pi::AIDA_HBookStore::CWNtuple::DOUBLE )
	desd->setVariableType( "double" );
      else if ( type == pi::AIDA_HBookStore::CWNtuple::FLOAT )
	desd->setVariableType( "float" );
      else if ( type == pi::AIDA_HBookStore::CWNtuple::INT )
	desd->setVariableType( "int" );
      else throw std::runtime_error( "No valid type for sub-tuples" );
      writeSubTupleDataDescription( static_cast<int>(iVar), description, optionString,
				    header, tf, store, indexVariables );
    }
    else {
      if ( type == pi::AIDA_HBookStore::CWNtuple::DOUBLE ) {
	description->setVariableType( "double" );
	writeDoubleDataDescription( description, optionString );
      }
      else if ( type == pi::AIDA_HBookStore::CWNtuple::FLOAT ) {
	description->setVariableType( "float" );
	writeFloatDataDescription( description, optionString );
      }
      else if ( type == pi::AIDA_HBookStore::CWNtuple::INT ) {
	description->setVariableType( "int" );
	writeIntDataDescription( description, optionString );
      }
      else if ( type == pi::AIDA_HBookStore::CWNtuple::BOOL ) {
	description->setVariableType( "bool" );
	writeBoolDataDescription( description, optionString );
      }
      else if ( type == pi::AIDA_HBookStore::CWNtuple::STRING ) {
	description->setVariableType( "std::string" );
	writeStringDataDescription( static_cast<int>(iVar), description, optionString );
      }
      else throw std::runtime_error( "Unrecognized type for a CWN tuple" );
    }
  }


  // Set the number of rows for the tuples which are indexed
  for ( std::map< int, std::string >::const_iterator iv = indexVariables.begin();
	iv != indexVariables.end(); ++iv ) {
    const std::string& varName = iv->second;
    for ( int i = 0; i < static_cast<int>( m_names.size() ); ++i ) {
      if ( m_names[i] == varName ) {
	m_devTuples[iv->first].second = -i;
	break;
      }
    }
  }

  clearBindings();
  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::bindVariable( int variableIndex )
{
  if ( m_boundVariables.find( variableIndex ) != m_boundVariables.end() ) return true;
  pi::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  const std::string& blockVar = m_blockNames[ variableIndex ];

  std::ostringstream os;
  os << "$SET:" << m_names[variableIndex];
#ifndef BADENDS
  os << std::ends;
#endif
  std::string steeringString = os.str();
  pi::AIDA_HBookStore::CWNtuple::ColumnType type = m_cache[variableIndex].first;
  if ( type != pi::AIDA_HBookStore::CWNtuple::STRING ) {
    if ( type == pi::AIDA_HBookStore::CWNtuple::DOUBLE ||
	 type == pi::AIDA_HBookStore::CWNtuple::FLOAT ||
	 type == pi::AIDA_HBookStore::CWNtuple::INT ||
	 type == pi::AIDA_HBookStore::CWNtuple::BOOL ) {
      pi::AIDA_HBookStore::HBook::describeCWNvariables( m_id, blockVar,
							    m_cache[variableIndex].second,
							    steeringString );
    }
    else if ( type == pi::AIDA_HBookStore::CWNtuple::TUPLE ) {
      void * address = 0;
      if ( m_doubleTuples.find( variableIndex ) != m_doubleTuples.end() ) {
	address = m_doubleTuples.find( variableIndex )->second.startAddress();
      }
      else if ( m_floatTuples.find( variableIndex ) != m_floatTuples.end() ) {
	address = m_floatTuples.find( variableIndex )->second.startAddress();
      }
      else if ( m_intTuples.find( variableIndex ) != m_intTuples.end() ) {
	address = m_intTuples.find( variableIndex )->second.startAddress();
      }
      pi::AIDA_HBookStore::HBook::describeCWNvariables( m_id, blockVar,
							    address,
							    steeringString );
    }
  }
  else {
    pi::AIDA_HBookStore::HBook::describeCWNvariablesChar( m_id, blockVar,
							      m_charVariables.find( variableIndex )->second.first,
							      steeringString );
  }
  m_boundVariables.insert( variableIndex );
  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::clearBindings()
{
  pi::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  const std::string blockVar = " ";
  const std::string steeringString = "$CLEAR";
  pi::AIDA_HBookStore::HBook::describeCWNvariables( m_id, blockVar, 0 , steeringString );
  m_boundVariables.clear();
  m_initializedReading = false;
  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::writeTupleRow( int rowNumber )
{
  // Update first the string variables
  for ( std::map< int, std::pair<char*, unsigned int> >::iterator iVar = m_charVariables.begin();
	iVar != m_charVariables.end(); ++iVar ) {
    if ( m_boundVariables.find( iVar->first ) != m_boundVariables.end() ) {
      const std::string& userString = *( reinterpret_cast< std::string* >( m_cache[ iVar->first ].second ) );
      char * charfromCache = iVar->second.first;
      unsigned int nChars = iVar->second.second;
      for ( unsigned int iChar = 0; iChar < nChars; ++iChar ) {
	if ( iChar < userString.size() ) {
	  charfromCache[iChar] = userString[iChar];
	}
	else charfromCache[iChar] = ' ';
      }
    }
  }

  // Update the boolean variables
  for ( unsigned int i = 0; i < m_boolVariables.size(); ++i ) {
    bool b = *( reinterpret_cast<bool*>(m_boolVariables[i]) );
    int* v = reinterpret_cast<int*>(m_boolVariables[i]);
    if ( b ) *v = 0x1;
    else *v = 0;
  }

  pi::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  pi::AIDA_HBookStore::HBook::fillCWNtuple( m_id );

  // Reset the sub-tuples.
  for( std::vector< std::pair< AIDA::Dev::IDevTuple*, int > >::iterator iTuple = m_devTuples.begin();
       iTuple != m_devTuples.end(); ++iTuple ) {
     iTuple->first->reset();
   }
  m_initializedReading = false;
  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::readTupleRow( int rowNumber )
{
  if ( ! m_initializedReading ) {
    for ( int i = 0; i < static_cast<int>( m_names.size() ); ++i ) bindVariable( i );
    m_initializedReading = true;
  }
  else {
    pi::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  }

  if ( ! pi::AIDA_HBookStore::HBook::readCWNtupleRow( m_id, rowNumber + 1 ) ) return false;


  // Update the boolean variables
  for ( unsigned int i = 0; i < m_boolVariables.size(); ++i ) {
    bool* b = reinterpret_cast<bool*>(m_boolVariables[i]);
    int v = *( reinterpret_cast<int*>(m_boolVariables[i]) );
    if ( ( v & 0x1 ) != 0 ) *b = true;
    else *b = false;
  }

  // Update the string variables
  for ( std::map< int, std::pair<char*, unsigned int> >::iterator iVar = m_charVariables.begin();
	iVar != m_charVariables.end(); ++iVar ) {
    if ( m_boundVariables.find( iVar->first ) != m_boundVariables.end() ) {
      std::string& userString = *( reinterpret_cast< std::string* >( m_cache[ iVar->first ].second ) );
      char * charfromCache = iVar->second.first;
      unsigned int nChars = iVar->second.second;
      std::string tempString = "";
      for ( unsigned int iChar = 0; iChar < nChars; ++iChar ) tempString += charfromCache[iChar];
      userString = tempString;
    }
  }

  // Set the correct dimensions for the sub-tuples.
  for( std::vector< std::pair< AIDA::Dev::IDevTuple*, int > >::iterator iTuple = m_devTuples.begin();
       iTuple != m_devTuples.end(); ++iTuple ) {
    int numberOfRowsInTuple = iTuple->second;
    if ( numberOfRowsInTuple < 0 ) {
      iTuple->first->header().setNumberOfRows( *( reinterpret_cast<int*>( m_cache[ -numberOfRowsInTuple ].second ) ) );
    }
    else {
      iTuple->first->header().setNumberOfRows( numberOfRowsInTuple );
    }
  }

  return true;
}


bool
pi::AIDA_HBookStore::CWNtuple::reset()
{
  pi::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  pi::AIDA_HBookStore::HBook::resetTuple( m_id );
  return true;
}


void*
pi::AIDA_HBookStore::CWNtuple::variableAddress( int variableIndex )
{
  if ( variableIndex < 0 || variableIndex >= static_cast<int>( m_cache.size() ) ) return 0;
  return m_cache[ variableIndex ].second;
}


const void*
pi::AIDA_HBookStore::CWNtuple::variableAddress( int variableIndex ) const
{
  if ( variableIndex < 0 || variableIndex >= static_cast<int>( m_cache.size() ) ) return 0;
  return m_cache[ variableIndex ].second;
}


std::pair< std::string, void* >
pi::AIDA_HBookStore::CWNtuple::writeDoubleDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							       const std::string& options )
{
  void* address = new double;
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::DOUBLE, address ) );
  return std::make_pair( description->variableName() + ":R*8", address );
}

std::pair< std::string, void* >
pi::AIDA_HBookStore::CWNtuple::writeFloatDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							      const std::string& options )
{
  void* address = new float;
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::FLOAT, address ) );
  return std::make_pair( description->variableName() + ":R*4", address );
}

std::pair< std::string, void* >
pi::AIDA_HBookStore::CWNtuple::writeIntDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							    const std::string& options )
{
  const std::string& name = description->variableName();
  std::string steeringString = name;
  void* address = new int;
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::INT, address ) );
  std::string::size_type idx = options.find( " " + name );
  if ( idx != std::string::npos ) {
    std::istringstream is( options.substr( idx + name.size() + 1 ).c_str() );
    int minValue, maxValue;
    bool foundEdges = false;
    char c;
    is >> std::ws >> c;
    if ( !( is.eof() ) && c == '[' ) {
      is >> std::ws >> minValue >> std::ws >> c;
      if ( !( is.eof() ) && c == ',' ) {
	is >> std::ws >> maxValue >> std::ws >> c;
	if ( c == ']' ) foundEdges = true;
      }
    }
    if ( foundEdges ) {
      std::ostringstream os;
      os << "[" << minValue << "," << maxValue << "]";
#ifndef BADENDS
      os << std::ends;
#endif
      steeringString += os.str();
    }
  }
  steeringString += ":I*4";
  return std::make_pair( steeringString, address );
}

std::pair< std::string, void* >
pi::AIDA_HBookStore::CWNtuple::writeBoolDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							     const std::string& options )
{
  int* iaddr = new int;
  void* address = iaddr;
  m_boolVariables.push_back( iaddr );
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::BOOL, address ) );
  return std::make_pair( description->variableName() + ":L", address );
}

std::pair< std::string, char* >
pi::AIDA_HBookStore::CWNtuple::writeStringDataDescription( int variableIndex,
							       AIDA::Dev::ITupleVariableDescription* description,
							       const std::string& options )
{
  const std::string& name = description->variableName();
  void* p =  new std::string;
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::STRING, p ) );
  unsigned int stringSize = 4;
  std::string::size_type idx = options.find( name );
  if ( idx != std::string::npos ) {
    std::istringstream is( options.substr( idx + name.size() ).c_str() );
    unsigned int temp = 0;
    char c;
    is >> std::ws >> c >> std::ws;
    if ( c == ':' && ! is.eof() ) {
      is >> c;
      if ( c == 'C' && ! is.eof() ) is >> c;
    }
    if ( c == '*' && ! is.eof() ) {
      is >> temp;
      if ( temp != 0 && temp%4 == 0 && temp <= 32 ) stringSize = temp;
    }
  }
  char* cc = new char[stringSize];
  m_charVariables.insert( std::make_pair( variableIndex, std::make_pair( cc, stringSize ) ) );
  std::ostringstream os;
  os << name << ":C*" << stringSize;
#ifndef BADENDS
  os << std::ends;
#endif
  std::string steeringString = os.str();
  return std::make_pair( steeringString, cc );
}


bool
pi::AIDA_HBookStore::CWNtuple::findDimensionsFromOptionString( const std::string& options,
								   const std::string& tupleVariable,
								   std::vector<int>& dimensions,
								   std::string& indexVariableName ) const
{
  std::string::size_type idx = options.find( " " + tupleVariable );
  if ( idx == std::string::npos ) return false;
  dimensions.clear();
  std::string subOptions = options.substr( idx + tupleVariable.size() + 1 );
  idx = subOptions.find("(");
  if ( idx == std::string::npos ) return false;
  std::string::size_type idx1 = subOptions.find(")");
  if ( idx1 == std::string::npos ) return false;
  if ( idx + 2 > idx1 ) return false;
  std::string stringToProcess = subOptions.substr( idx + 1, idx1 - idx - 1 );

  std::vector< std::string > tokens;
  while ( true ) {
    std::string::size_type pos = stringToProcess.find( "," );
    if ( pos == std::string::npos ) break;
    tokens.push_back( stringToProcess.substr( 0, pos ) );
    stringToProcess = stringToProcess.substr( pos + 1 );
  }
  tokens.push_back( stringToProcess );

  if ( tokens.size() == 0 ) return false;
  for ( unsigned int i = 0; i < tokens.size() - 1; ++i ) {
    std::istringstream is( tokens[i].c_str() );
    int dim = 0;
    is >> dim;
    if ( dim == 0 ) return false;
    else dimensions.push_back( dim );
  }

  // for the last one check whether it is an existing name
  std::istringstream is( tokens.back().c_str() );
  std::string slast;
  is >> slast;
  indexVariableName = "";
  for ( unsigned int i = 0; i < m_names.size(); ++i ) {
    if ( m_names[i] == slast ) {
      indexVariableName = slast;
      break;
    }
  }

  if ( indexVariableName != "" ) {
    std::string::size_type ix = options.find( " " + indexVariableName );
    if ( ix == std::string::npos ) return false;
    std::istringstream iss( options.substr( ix + indexVariableName.size() + 1 ).c_str() );
    int minValue, maxValue;
    bool foundEdges = false;
    char c;
    iss >> std::ws >> c;
    if ( !( iss.eof() ) && c == '[' ) {
      iss >> std::ws >> minValue >> std::ws >> c;
      if ( !( iss.eof() ) && c == ',' ) {
	iss >> std::ws >> maxValue >> std::ws >> c;
	if ( c == ']' ) foundEdges = true;
      }
    }
    if ( ! foundEdges ) return false;
    else dimensions.push_back( maxValue );
  }
  else {
    std::istringstream iss( slast.c_str() );
    int dim = 0;
    iss >> dim;
    if ( dim == 0 ) return false;
    else dimensions.push_back( dim );
  }
  return true;
}


std::pair< std::string, void* >
pi::AIDA_HBookStore::CWNtuple::writeSubTupleDataDescription( int variableIndex,
								 AIDA::Dev::ITupleVariableDescription* description,
								 const std::string& options,
								 AIDA::Dev::ITupleHeader& header,
								 AIDA::Dev::IDevTupleFactory& tf,
								 AIDA::Dev::IBackingStore* store,
								 std::map< int, std::string >& indexVariables )
{
  const std::string& name = description->variableName();
  std::string steeringString = name;
  void* address = 0;
  int dimension = 0;
  std::vector<int> dimensions;
  std::string indexVariable = "";
  if ( ! findDimensionsFromOptionString( options, name, dimensions, indexVariable ) ) {
    throw std::runtime_error( "Could not resolve the dimensions of the variable " + name );
  }

  int totalSize = 1;
  for ( unsigned int idim = 0; idim < dimensions.size(); ++idim ) totalSize *= dimensions[idim];
  std::ostringstream os;
  os << "(";
  for ( unsigned int idim = 0; idim < dimensions.size() - 1; ++idim ) {
    os << dimensions[idim];
    if ( dimensions.size() > 1 && idim != dimensions.size() - 1 ) os << ",";
  }
  if ( indexVariable != "" ) os << indexVariable;
  else os << dimensions.back();
  os << ")";
#ifndef BADENDS
  os << std::ends;
#endif
  steeringString += os.str();

  // Find the type of the variable and create the cache
  pi::AIDA_HBookStore::ISubTuple* st = 0;
  AIDA::Dev::ITupleVariableDescription* des = description;
  while ( true ) {
    const std::string stype = des->variableType();
    if ( stype == "AIDA::ITuple" ) {
      if ( des->numberOfVariables() != 1 ) std::runtime_error( "Variable name has invalid declaration" );
      ++dimension;
      des = des->variableDescription( 0 );
    }
    else {
      if ( stype == "double" ) {
	steeringString += ":R*8";
	m_doubleTuples.insert( std::make_pair( variableIndex,
					       pi::AIDA_HBookStore::VectorTuple<double>( totalSize,
											     dimensions.front() ) ) );
	pi::AIDA_HBookStore::VectorTuple<double> * vt = &( m_doubleTuples.find( variableIndex )->second );
	address = vt->startAddress();
	st = vt;
      }
      else if ( stype == "float" ) {
	steeringString += ":R*4";
	m_floatTuples.insert( std::make_pair( variableIndex,
					      pi::AIDA_HBookStore::VectorTuple<float>( totalSize,
											    dimensions.front() ) ) );
	pi::AIDA_HBookStore::VectorTuple<float> * vt = &( m_floatTuples.find( variableIndex )->second );
	address = vt->startAddress();
	st = vt;
      }
      else if ( stype == "int" ) {
	steeringString += ":I*4";
	m_intTuples.insert( std::make_pair( variableIndex,
					    pi::AIDA_HBookStore::VectorTuple<int>( totalSize,
										       dimensions.front() ) ) );
	pi::AIDA_HBookStore::VectorTuple<int> * vt = &( m_intTuples.find( variableIndex )->second );
	address = vt->startAddress();
	st = vt;
      }
      else {
	throw std::runtime_error( "Tuples of type \"" + stype + "\" are not supported for CWN" );
      }
      break;
    }
  }

  // The leaf tuple
  AIDA::Dev::IDevTuple* leafTuple = tf.create( store, emptyString, emptyString );
  std::string tuplePath = "v";
  for ( int ii = 0; ii < variableIndex; ++ii ) tuplePath += "_";
  tuplePath += header.pathInStore();
  leafTuple->header().setPathInStore( tuplePath );

  AIDA::Dev::IDevTuple* lastTuple = leafTuple;
  m_devTuples.push_back( std::make_pair( lastTuple, dimensions.front() ) );
  m_iSubTuples.insert( std::make_pair( tuplePath, st ) );

  // The other tuples
  for ( unsigned int ist = 1; ist < dimensions.size(); ++ist ) {
    m_subTuples.push_back( pi::AIDA_HBookStore::SubTuple( lastTuple, st, dimensions[ist] ) );
    st = &( m_subTuples.back() );
    std::string tuplePathInStore = "v";
    for ( int ii = 0; ii < variableIndex; ++ii ) tuplePathInStore += "_";
    for ( unsigned int ii = 0; ii < ist; ++ii ) tuplePathInStore += "-";
    tuplePathInStore += header.pathInStore();
    lastTuple = tf.create( store, emptyString, emptyString );
    lastTuple->header().setPathInStore( tuplePathInStore );
    m_devTuples.push_back( std::make_pair( lastTuple, dimensions[ist] ) );
    m_iSubTuples.insert( std::make_pair( tuplePathInStore, st ) );
  }

  if ( indexVariable != "" ) {
    indexVariables.insert( std::make_pair( static_cast<int>( m_devTuples.size() - 1 ), indexVariable ) );
  }

  void* p = lastTuple;
  m_cache.push_back( std::make_pair( pi::AIDA_HBookStore::CWNtuple::TUPLE, p ) );
  
  des = description->variableDescription(0);
  const unsigned int ntuplesTotal = m_devTuples.size();
  for ( unsigned int ist = 1; ist <= dimensions.size(); ++ist ) {
    m_devTuples[ntuplesTotal-ist].first->header().setVariableDescription( des, false );
    if ( ist < dimensions.size() ) des = des->variableDescription(0);
  }

  return std::make_pair( steeringString, address );
}
