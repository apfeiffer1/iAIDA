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

#include "AIDA_TupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA/IAnnotation.h"

static const std::string emptyString = "";

iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::AIDA_TupleHeader():
  m_annotation( createAnnotation() ),
  m_numberOfRows( 0 ),
  m_currentRowNumber( -1 ),
  m_pathInStore(emptyString),
  m_variableDescriptions(),
  m_variableIndices(),
  m_options( emptyString )
{}


iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::~AIDA_TupleHeader()
{
  if ( m_annotation ) delete m_annotation;
  for ( unsigned int i = 0; i < m_variableDescriptions.size(); ++i ) {
    if ( m_variableDescriptions[i].second ) delete m_variableDescriptions[i].first;
  }

}


AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::annotation()
{
  return *m_annotation;
}


const AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::annotation() const
{
  return *m_annotation;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::setVariableDescription( AIDA::Dev::ITupleVariableDescription* description,
								     bool passOwnership )
{
  std::map< std::string, int >::iterator iVariable = m_variableIndices.find( description->variableName() );
  if ( iVariable != m_variableIndices.end() ) {
    return false;
  }
  else {
    m_variableIndices.insert( std::make_pair( description->variableName(), static_cast<int>( m_variableDescriptions.size() ) ) );
    m_variableDescriptions.push_back( std::make_pair( description, passOwnership ) );
  }
  return true;
}


AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::variableDescription( int subVariableIndex )
{
  if ( subVariableIndex < 0 || subVariableIndex >= static_cast<int>(m_variableDescriptions.size()) ) return 0;
  return m_variableDescriptions[subVariableIndex].first;
}


const AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::variableDescription( int subVariableIndex ) const
{
  if ( subVariableIndex < 0 || subVariableIndex >= static_cast<int>(m_variableDescriptions.size()) ) return 0;
  return m_variableDescriptions[subVariableIndex].first;
}


int
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::variableIndex( const std::string& variableName ) const
{
  std::map< std::string, int >::const_iterator i = m_variableIndices.find( variableName );
  if ( i == m_variableIndices.end() ) return -1;
  else return i->second;
}


#include <memory>
#include <stdexcept>
#include "AIDA_Annotation/AIDA_Annotation.h"

AIDA::IAnnotation*
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::createAnnotation()
{
  /*
  std::auto_ptr<AIDA::Dev::IDevAnalysisFactory> af(dynamic_cast<AIDA::Dev::IDevAnalysisFactory*>(AIDA_createAnalysisFactory() ));
  if ( ! af.get() ) throw std::runtime_error( "Could not create an analysis factory" );
  AIDA::Dev::IAnnotationFactory* anf = af->annotationFactory();
  if ( ! anf ) throw std::runtime_error( "Could not find an annotation factory" );
  return anf->createAnnotation();
  */
  return new iAIDA::AIDA_Annotation::AIDA_Annotation( );
}
