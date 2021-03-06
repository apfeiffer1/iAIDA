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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_CWNTUPLE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_CWNTUPLE_H

#include "IPersistentTuple.h"
#include "SubTuple.h"
#include "VectorTuple.h"

#include <map>
#include <vector>
#include <set>

namespace AIDA {
  namespace Dev {
    class IDevTupleFactory;
    class IDevTuple;
    class ITupleHeader;
    class IBackingStore;
    class ITupleVariableDescription;
  }
}

namespace iAIDA {
  namespace AIDA_HBookStore {

class CWNtuple : virtual public IPersistentTuple
{
public:
  CWNtuple( const std::string& memoryDirectory, int id );
  ~CWNtuple();

  bool writeDescription( AIDA::Dev::ITupleHeader& header,
			 AIDA::Dev::IDevTupleFactory& tf,
			 AIDA::Dev::IBackingStore* store );
  bool readDescription( AIDA::Dev::ITupleHeader& header,
			AIDA::Dev::IDevTupleFactory& tf,
			AIDA::Dev::IBackingStore* store );

  bool bindVariable( int variableIndex );
  bool clearBindings();
  bool writeTupleRow( int rowNumber );
  bool readTupleRow( int rowNumber );
  bool reset();
  void* variableAddress( int variableIndex );
  const void* variableAddress( int variableIndex ) const;

  bool bindVariable( int, const std::string& ) {return true; }
  bool clearBindings( const std::string& s ) {return true;}
  bool writeTupleRow( int i, const std::string& s ) {return m_iSubTuples.find(s)->second->writeTupleRow(i);}
  bool readTupleRow( int i, const std::string& s ) {return m_iSubTuples.find(s)->second->readTupleRow(i);}
  bool reset( const std::string& s ) {return m_iSubTuples.find(s)->second->reset();}
  void* variableAddress( int i, const std::string& s ) {
    void* p = m_iSubTuples.find(s)->second->variableAddress(i);
    return p;
  }
  const void* variableAddress( int i, const std::string& s ) const {
    const void* p = m_iSubTuples.find(s)->second->variableAddress(i);
    return p;
  }

private:
  typedef enum { DOUBLE, FLOAT, INT, BOOL, STRING, TUPLE } ColumnType;

private:
  std::string                                          m_dir;
  int                                                  m_id;
  bool                                                 m_initializedReading;
  std::vector< std::pair<ColumnType, void*> >          m_cache; // data passed to the tuple
  std::vector< std::string >                           m_names; // names passed to the tuple
  std::vector< std::string >                           m_blockNames; // names of the tuple block for a given variable

  std::vector< std::pair<AIDA::Dev::IDevTuple*, int> > m_devTuples; // subtuples
  std::map< std::string, ISubTuple* >                  m_iSubTuples; // subtuple data pointers
  std::vector< SubTuple >                              m_subTuples; // subtuple data
  std::map< int, VectorTuple<double> >                 m_doubleTuples; // non scalar double variables
  std::map< int, VectorTuple<float> >                  m_floatTuples; // non scalar float variables
  std::map< int, VectorTuple<int> >                    m_intTuples; // non scalar int variables

  std::set< int >                                      m_boundVariables; // set for variables currently bound.
  std::map< int, std::pair<char*, unsigned int> >      m_charVariables; // cache for string variables.
  std::vector< int* >                                  m_boolVariables; // boolean variables

private:
  bool findDimensionsFromOptionString( const std::string& options,
				       const std::string& tupleVariable,
				       std::vector<int>& dimensions,
				       std::string& indexVariableName ) const;

  std::pair< std::string, void* > writeSubTupleDataDescription( int variableIndex,
								AIDA::Dev::ITupleVariableDescription* description,
								const std::string& options,
								AIDA::Dev::ITupleHeader& header,
								AIDA::Dev::IDevTupleFactory& tf,
								AIDA::Dev::IBackingStore* store,
								std::map< int, std::string >& indexVariables );

  std::pair< std::string, void* > writeDoubleDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							      const std::string& options );

  std::pair< std::string, void* > writeFloatDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							     const std::string& options );

  std::pair< std::string, void* > writeIntDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							   const std::string& options );

  std::pair< std::string, void* > writeBoolDataDescription( AIDA::Dev::ITupleVariableDescription* description,
							    const std::string& options );

  std::pair< std::string, char* > writeStringDataDescription( int variableIndex,
							      AIDA::Dev::ITupleVariableDescription* description,
							      const std::string& options );

};

  }
}

#endif
