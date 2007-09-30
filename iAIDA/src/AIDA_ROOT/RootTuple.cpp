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

#include "RootTuple.h"
#include "TNtuple.h"
#include "TFile.h"

iAIDA::AIDA_RootStore::RootTuple::RootTuple( const std::string& name, 
					  const std::string & title,
					  const std::vector<std::string> & varList, 
					  int bufSize, 
					  TFile * rootStore) : 

  // m_dir( memoryDirectory ),
  //m_name( name ),
  m_varList(varList),
  m_cache( varList.size(), 0 ),
  m_initializedReading( false )
{
  // create string with variables
  std::string varDescr = "";
  if (varList.size() > 0) varDescr = varList[0]; 
  for (unsigned int i = 1; i < varList.size(); ++i) { 
    varDescr += ":" + varList[i];
  }
  // CREATE ROOT OBJECT
  m_tuple = new TNtuple(name.c_str(), title.c_str(), varDescr.c_str(), bufSize); 
  // attach to our created  store: 
  m_tuple->SetDirectory(rootStore);
}

// create from an existing ROOT TTuple

iAIDA::AIDA_RootStore::RootTuple::RootTuple( TNtuple *  tntuple) : 
  m_initializedReading( false ), 
  m_tuple(tntuple)
{
  // read description
  for (int i = 0; i < (m_tuple->GetListOfBranches())->GetEntries(); ++i) { 
    std::string colName =  m_tuple->GetListOfBranches()->At(i)->GetName(); 
    m_varList.push_back( colName );
    m_varType.push_back("float");// tuple contains only floats
    m_cache.push_back( 0 );
  }

}  


iAIDA::AIDA_RootStore::RootTuple::~RootTuple() 
{ 
  // delete yourself or let ROOT do that ? 
  //if (!m_tuple) delete m_tuple; 
}  

bool
iAIDA::AIDA_RootStore::RootTuple::bindVariable( int )
{
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTuple::clearBindings()
{
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTuple::writeTupleRow( int )
{
  //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  //iAIDA::AIDA_RootStore::Root::fillRootTuple( m_id, m_cache );
  m_tuple->Fill(&m_cache.front()); 
  m_initializedReading = false;
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTuple::readTupleRow( int rowNumber )
{
  //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  
  if ( ! m_initializedReading ) {
    refreshRootTuplePointers();
    m_initializedReading = true;
  }
  int size = m_tuple->GetEntry(rowNumber); 
  if (size > 0) return true; 
  return false;
  // probably I could use also 
  //  m_tuple->GetArgs() and set the adres to m_cache (is it faster ??) 
}

bool
iAIDA::AIDA_RootStore::RootTuple::reset()
{
  //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  //iAIDA::AIDA_RootStore::Root::resetTuple( m_id );
  m_initializedReading = false;
  return true;
}

void 
iAIDA::AIDA_RootStore::RootTuple::refreshRootTuplePointers() 
{
  // set adress for tuples 
  for (unsigned int i = 0; i <  m_varList.size() ; ++i) { 
    m_tuple->SetBranchAddress(m_varList[i].c_str(), variableAddress(i) );
  }
}


void*
iAIDA::AIDA_RootStore::RootTuple::variableAddress( int variableIndex )
{
  return &( m_cache[variableIndex] );
}

const void*
iAIDA::AIDA_RootStore::RootTuple::variableAddress( int variableIndex ) const
{
  return &( m_cache[variableIndex] );
}

