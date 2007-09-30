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

#ifndef IAIDA_AIDA_ROOTSTORE_AIDA_ROOTSTORE_IPERSISTENTTUPLE_H
#define IAIDA_AIDA_ROOTSTORE_AIDA_ROOTSTORE_IPERSISTENTTUPLE_H

#include <string>
#include <vector>

namespace iAIDA {
  namespace AIDA_RootStore {

class IPersistentTuple
{
public:
  virtual ~IPersistentTuple() {}
  virtual bool bindVariable( int variableIndex ) = 0;
  virtual bool clearBindings() = 0;
  virtual bool writeTupleRow( int rowNumber ) = 0;
  virtual bool readTupleRow( int rowNumber ) = 0;
  virtual bool reset() = 0;
  virtual void* variableAddress( int variableIndex ) = 0;
  virtual const void* variableAddress( int variableIndex ) const = 0;
  virtual  void setVariableAddress( int variableIndex, void * ) = 0;

  virtual bool bindVariable( int variableIndex, const std::string& path ) = 0;
  virtual bool clearBindings( const std::string& path) = 0;
  virtual bool writeTupleRow( int rowNumber, const std::string& path ) = 0;
  virtual bool readTupleRow( int rowNumber, const std::string& path ) = 0;
  virtual bool reset( const std::string& path) = 0;
  virtual void* variableAddress( int variableIndex, const std::string& path ) = 0;
  virtual const void* variableAddress( int variableIndex, const std::string& path ) const = 0;
  virtual void setVariableAddress( int variableIndex, const std::string& path, void * ) = 0;
  virtual const std::vector<std::string> & variableNames() const = 0; 
  virtual const std::vector<std::string> & variableTypes() const = 0; 
};

  }
}


#endif
