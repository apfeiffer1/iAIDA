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

#ifndef IAIDA_AIDA_ROOTSTORE_AIDA_ROOTSTORE_ROOTTREE_H
#define IAIDA_AIDA_ROOTSTORE_AIDA_ROOTSTORE_ROOTTREE_H

#include "IPersistentTuple.h"
#include <vector>
//#include <map>

class TTree; 
class TFile;

namespace iAIDA {
  namespace AIDA_RootStore {

class RootTree : virtual public IPersistentTuple
{
public:
  // create a new one 
  RootTree( const std::string& name, const std::string & title, const std::vector<std::string> & varList, const std::vector<std::string> & varType, TFile * rootStore );
  // create from a given one
  RootTree( TTree * ttree); 
  ~RootTree(); 

  bool bindVariable( int variableIndex );
  bool clearBindings();
  bool writeTupleRow( int rowNumber );
  bool readTupleRow( int rowNumber );
  bool reset();
  void* variableAddress( int variableIndex );
  const void* variableAddress( int variableIndex ) const;
  void setVariableAddress( int variableIndex, void * );

  bool bindVariable( int, const std::string& ) {return false;}
  bool clearBindings( const std::string& ) {return false;}
  bool writeTupleRow( int, const std::string& ) {return false;}
  bool readTupleRow( int, const std::string& ) {return false;}
  bool reset( const std::string& ) {return false;}
  void* variableAddress( int, const std::string& ) {return 0;}
  const void* variableAddress( int, const std::string& ) const {return 0;}
  void setVariableAddress( int , const std::string&, void * ) { return; }

  static bool isTypeSupported(const std::string & type); 

  const std::vector<std::string> & variableNames() const { return m_varList; } 
  const std::vector<std::string> & variableTypes() const { return m_varType; }


 protected: 

  // refresh pointers for readings
  void refreshRootTreePointers(); 
  // initialize pointers for writing (call before filling)
  void initRootTreePointers(); 

  void describeTreeType(int i , const std::string & type); 

  typedef std::string ColumnType; 

  template <typename T> 
    void writeDataDescription(ColumnType col);

  void writeGenericDataDescription(ColumnType col);

  bool readVariableDescription( );


private:

  //typedef enum { DOUBLE, FLOAT, INT, BOOL, STRING, TUPLE } ColumnType;


  //std::string                 m_name;
  std::vector< std::string >                            m_varList;
  std::vector< std::string >                            m_varType;
  std::vector< bool >                                   m_objType; /* flag for objects */
  std::vector< std::pair<ColumnType, void*> >            m_cache; 
  bool                                                  m_initializedReading;
  bool                                                  m_initializedWriting;
  TTree   *                                             m_tuple; 
};

  }
}

#endif
