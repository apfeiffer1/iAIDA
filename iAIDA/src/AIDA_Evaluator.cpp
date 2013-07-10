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

#include "AIDA_Evaluator.h"
#include "CompiledExpressionManager.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include <typeinfo>


static const std::string doubleType = "double";
static const std::string floatType = "float";
static const std::string intType = "int";
static const std::string shortType = "short";
static const std::string longType = "long";
static const std::string charType = "char";
static const std::string boolType = "bool";
static const std::string stringType = "std::string";
static const std::string tupleType = "AIDA::ITuple";


iAIDA::AIDA_Tuple_native::AIDA_Evaluator::AIDA_Evaluator( CompiledExpressionManager& manager,
							   const std::string& expression ):
  m_manager( manager ),
  m_expression( expression ),
  m_evaluator( 0 ), 
  m_simpleEvaluator(false), 
  m_variableAdress(0)
{}


bool
iAIDA::AIDA_Tuple_native::AIDA_Evaluator::initialize( AIDA::ITuple & tuple )
{

  

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


    // LM check for simple evaluator  : expression = one variable . No need to compile in this case
    // first clean up expression 
    // LM : special case for simple expression = "1" or "0": clean up and look if 1/0 or true/false
    std::string es = m_cleanupExpression( ); 

    // if string es = one variable 
    std::map<std::string, std::string>::iterator iVar = variableTypes.find(es); 
    if ( iVar != variableTypes.end() ) {     
      m_simpleEvaluator = true; 
      m_variableType = iVar->second; 
      m_variableAdress = tp.variableAddress(iVar->first); 
      return true; 
    }
    else 
      m_simpleEvaluator = false; 
      


    // Create the object
    std::set<std::string> variablesUsed;
    std::auto_ptr<iAIDA::AIDA_Tuple_native::IEvaluatorExpressionBase> evaluator = m_manager.createEvaluatorExpression( m_expression,
															variableTypes,
															variablesUsed );
    if ( ! evaluator.get() ) return false;
    m_evaluator = evaluator;

    // Bind the variables
    std::map<std::string, void*> variableAddresses;
    for ( std::set<std::string>::const_iterator iVariable = variablesUsed.begin();
	  iVariable != variablesUsed.end(); ++iVariable ) {
      void* p = tp.variableAddress( *iVariable );
      if ( !p ) return false;
      variableAddresses.insert( std::make_pair( *iVariable, p ) );
    }
    m_evaluator->bind( variableAddresses );

    return true;
  }
  catch( std::bad_cast ) {
    return false;
  }
}


double
iAIDA::AIDA_Tuple_native::AIDA_Evaluator::evaluateDouble() const
{
  if (m_simpleEvaluator) return m_simpleEvaluation(); 
  if ( m_evaluator.get() ) return m_evaluator->evaluate();
  else return false;
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Evaluator::expression() const
{
  return m_expression;
}


// evaluation for simple cases : evaluator = one variable 

double 
iAIDA::AIDA_Tuple_native::AIDA_Evaluator::m_simpleEvaluation() const
{ 
  double value = 0; 
  if (!m_variableAdress) return 0; 
  if ( m_variableType == doubleType ) value = *(reinterpret_cast<double*>(m_variableAdress) );
  else if  ( m_variableType == floatType ) value = static_cast<double>( *(reinterpret_cast<float*>(m_variableAdress) ) );
  else if  ( m_variableType == longType ) value = static_cast<double>( *(reinterpret_cast<long*>(m_variableAdress) ) );
  else if  ( m_variableType == intType ) value = static_cast<double>( *(reinterpret_cast<int*>(m_variableAdress) ) );
  else if  ( m_variableType == shortType ) value = static_cast<double>( *(reinterpret_cast<short*>(m_variableAdress) ) );
  else if  ( m_variableType == charType ) value = static_cast<double>( *(reinterpret_cast<char*>(m_variableAdress) ) );
  else if  ( m_variableType == boolType ) value = ( *(reinterpret_cast<bool*>(m_variableAdress) ) == true ? 1 : 0 );
  // for strings and tuples evaluator is always zero 
  return value;   
}


std::string 
iAIDA::AIDA_Tuple_native::AIDA_Evaluator::m_cleanupExpression( ) const {
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
  return es; 
}
