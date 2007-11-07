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

#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"
#include "AIDA_Tuple.h"
#include "AIDA_ChainedTuple.h"
#include "AIDA_TupleVariableDescription.h"
#include "TupleVariableDescriptionBuilder.h"

#include <exception>
#include <stdexcept>


iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::AIDA_DevTupleFactory()
{}


iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::~AIDA_DevTupleFactory()
{}


AIDA::Dev::IDevTuple*
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::create( AIDA::Dev::IBackingStore* backingStore,
							 const std::string& title,
							 std::string options )
{
  AIDA::Dev::IDevTuple* tuple = 0;
  try {
    tuple = new iAIDA::AIDA_Tuple_native::AIDA_Tuple;
  }
  catch ( std::exception& ) {
    return 0;
  }
  if ( ! tuple ) return 0;
  tuple->setTitle( title );
  tuple->header().setOptions( options );
  tuple->connectToStore( backingStore );
  return tuple;
}

bool
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::buildTupleHeader(const std::vector<std::string>& columnNames,
							      const std::vector<std::string>& columnTypes, 
							      const std::string & pathInStore, 
							      AIDA::Dev::ITupleHeader & header)  
{
    header.setPathInStore( pathInStore );
    iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder builder( *this );
    return builder.buildDescription( header , columnNames, columnTypes );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::buildTupleHeader(const std::string & columnDescr, 
							      const std::string & pathInStore, 
							      AIDA::Dev::ITupleHeader & header)  
{
    header.setPathInStore( pathInStore );
    iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder builder( *this );
    return builder.buildDescription( header , columnDescr );
}




AIDA::Dev::IDevTuple*
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::createChained( const std::string& title,
								const std::vector<AIDA::Dev::IDevTuple*>& tupleset )
{
  AIDA::Dev::IDevTuple* tuple = 0;
  try {
    tuple = new iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple( tupleset );
  }
  catch( std::exception& ) {
    return 0;
  }
  if ( ! tuple ) return 0;
  tuple->setTitle( title );
  return tuple;
}


AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::createDescription()
{
  return new iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription;
}


AIDA::Dev::ITupleVariableDescription*
iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory::createDescription( const AIDA::Dev::ITupleVariableDescription& original )
{
  AIDA::Dev::ITupleVariableDescription* copy = new iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription;
  copy->setVariableName( original.variableName() );
  copy->setVariableType( original.variableType() );
  for ( int iVariable = 0; iVariable < original.numberOfVariables(); ++iVariable ) {
    copy->setVariableDescription( createDescription( *( original.variableDescription( iVariable ) ) ), true );
  }
  return copy;
}
