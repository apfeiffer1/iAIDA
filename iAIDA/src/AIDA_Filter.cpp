#include "AIDA_Filter.h"
#include "CompiledExpressionManager.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include <typeinfo>
#include <algorithm>


iAIDA::AIDA_Tuple_native::AIDA_Filter::AIDA_Filter( CompiledExpressionManager& manager,
						     const std::string& expression ):
  m_manager( manager ),
  m_expression( expression ),
  m_filter( 0 ), 
  m_simpleFilter(false), 
  m_accept(false)
{}


bool
iAIDA::AIDA_Tuple_native::AIDA_Filter::initialize( AIDA::ITuple & tuple )
{
  // LM : special case for simple expression = "1" or "0": clean up and look if 1/0 or true/false
  std::string es = m_cleanupExpression(  ) ; 
  if (es == "1" || es == "true" ) { 
    m_simpleFilter = true;
    m_accept = true; 
    return true; 
  }
  else if ( es == "0" || es == "false") { 
    m_simpleFilter = true;
    m_accept = false; 
    return true; 
  }
  else 
    m_simpleFilter = false; 
    
    


  // LM : FIXME: this should be also implemented for foreign tuple 
  // implementations and not for IDevTuple 

  try{
    AIDA::Dev::IDevTuple& tp = dynamic_cast<AIDA::Dev::IDevTuple&>( tuple );
    AIDA::Dev::ITupleHeader& header = tp.header(); // Fetch the variables and their types
    std::map<std::string, std::string> variableTypes;
    int numberOfVariables = header.numberOfVariables();
    for ( int i = 0; i < numberOfVariables; ++i ) {
      const std::string& variableName = header.variableDescription(i)->variableName();
      const std::string& variableType = header.variableDescription(i)->variableType();
      variableTypes.insert( std::make_pair( variableName, variableType ) );
    }

    // Create the object
    std::set<std::string> variablesUsed;
    std::auto_ptr< iAIDA::AIDA_Tuple_native::IFilterExpressionBase > filter = m_manager.createFilterExpression( m_expression,
														 variableTypes,
														 variablesUsed );
    //assert(filter.get()); 
    if ( ! filter.get() ) return false;
    m_filter = filter;

    // Bind the variables
    std::map<std::string, void*> variableAddresses;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      void* p = tp.variableAddress( *iVariable );
      //assert(p); 
      if ( !p ) return false;
      variableAddresses.insert( std::make_pair( *iVariable, p ) );
    }
    m_filter->bind( variableAddresses );

    return true;
  }
  catch( std::bad_cast ) {
    //assert(0); 
    return false;
  }
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Filter::accept() const
{
  if (m_simpleFilter) return m_accept; 
  if ( m_filter.get() ) return m_filter->accept();
  else return false;
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Filter::expression() const
{
  return m_expression;
}

std::string 
iAIDA::AIDA_Tuple_native::AIDA_Filter::m_cleanupExpression( ) const {
  std::string es = m_expression; 
  // remove trailing space and columns
  while ( es.size() > 0 ) {
    char last = es[ es.size() - 1 ];
    if ( last == ' ' || last == ';' || last == '\n' || last == '\r' || last == '\t' ) {   
      es = es.substr( 0, es.size() - 1 );
    }
    else { 
      break; 
    }
  }
  // Remove starting spaces or colons
  while ( es.size() > 0 ) {
    char first = es[0];
    if ( first == ' ' || first == ';' || first == '\n' || first == '\r' || first == '\t' ) {
      es = es.substr( 1 );
    }
    else {
      break;
    }
  }      
  // convert all to lower case
  //std::transform( es.begin(), es.end(), es.begin(), tolower); 
  return es; 
}
