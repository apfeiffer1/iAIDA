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


#include "RootTree.h"
#include "TTree.h"
#include "TFile.h"
#include "TLeaf.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

iAIDA::AIDA_RootStore::RootTree::RootTree( const std::string& name, 
					  const std::string & title,
					  const std::vector<std::string> & varList, 
					  const std::vector<std::string> & varTypes, 
					  TFile * rootStore) : 

  // m_dir( memoryDirectory ),
  //m_name( name ),
  m_varList(varList),
  m_varType(varTypes),
  m_initializedReading( false ),
  m_initializedWriting( false )
{


  // CREATE ROOT OBJECT
  m_tuple = new TTree(name.c_str(), title.c_str() );
  // attach to our created store: 
  m_tuple->SetDirectory(rootStore);

  // create cache with type and pointers (write descriptions)
  // create branches and write description 
  m_objType.reserve( m_varList.size() ); 
  for (unsigned int i = 0; i < m_varList.size(); ++i) { 
    m_objType.push_back( false);
    describeTreeType( i, varTypes[i] ); 
  }


}

// create from an existing ROOT TTree

iAIDA::AIDA_RootStore::RootTree::RootTree( TTree *  t ) : 
  //  m_varType(varType), not needed
  //  m_cache( varList.size(), 0 ),
  m_initializedReading( false ), 
  m_initializedWriting( false ),
  m_tuple(t)
{
  readVariableDescription();
 
}  


iAIDA::AIDA_RootStore::RootTree::~RootTree() 
{ 
  // delete yourself or let ROOT do that ? 
  //if (m_tuple) delete m_tuple; 
}  

bool
iAIDA::AIDA_RootStore::RootTree::bindVariable( int )
{
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTree::clearBindings()
{
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTree::writeTupleRow( int /* i */ )
{
  if (!m_initializedWriting) initRootTreePointers(); 
  //void  * p=  variableAddress(0); 
  //double * dp = reinterpret_cast<double *>(p);
  //if (dp) std::cout << " RT val= " << *dp << std::endl;   
 //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  //iAIDA::AIDA_RootStore::Root::fillRootTree( m_id, m_cache );
  m_tuple->Fill(); 
  m_initializedReading = false;
  return true;
}


bool
iAIDA::AIDA_RootStore::RootTree::readTupleRow( int rowNumber )
{
  //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  
  if ( ! m_initializedReading ) {
    refreshRootTreePointers();
    m_initializedReading = true;
  }
  int size = m_tuple->GetEntry(rowNumber); 
  if (size > 0) return true; 
  return false;
  // probably I could use also 
  //  m_tuple->GetArgs() and set the adres to m_cache (is it faster ??) 
}

bool
iAIDA::AIDA_RootStore::RootTree::reset()
{
  //iAIDA::AIDA_RootStore::Root::changeDirectory( m_dir );
  //iAIDA::AIDA_RootStore::Root::resetTree( m_id );
  m_initializedReading = false;
  return true;
}

void
iAIDA::AIDA_RootStore::RootTree::initRootTreePointers() 
{  
  // create branches and write description 
  for (unsigned int i = 0; i < m_varList.size(); ++i) { 
    std::string branchSpec =   m_cache[i].first; 
    //std::cout << " set branch " << i <<" name " <<  m_varList[i] << " type " << branchSpec << " address " << (int) variableAddress(i) << std::endl; 
    // in case of objects signature is different
    if ( m_objType[i] ) {
      m_tuple->Branch(m_varList[i].c_str(),branchSpec.c_str(), variableAddress(i) );
    }
    else
      m_tuple->Branch(m_varList[i].c_str(),variableAddress(i), branchSpec.c_str());
	
  }
  m_initializedWriting = true; 
}

void 
iAIDA::AIDA_RootStore::RootTree::refreshRootTreePointers() 
{
  // set adress for tuples (bind all variables) 
  for (unsigned int i = 0; i <  m_varList.size() ; ++i) { 
    if (m_objType[i] ) { 
      TBranch * branch = m_tuple->GetBranch(m_varList[i].c_str());
      branch->SetAddress(variableAddress(i) );
    }
    else 
      m_tuple->SetBranchAddress(m_varList[i].c_str(), variableAddress(i) );
  }
}


void*
iAIDA::AIDA_RootStore::RootTree::variableAddress( int variableIndex )
{
  if ( variableIndex < 0 || variableIndex >= static_cast<int>( m_cache.size() ) ) return 0;
  return m_cache[ variableIndex ].second;
}

const void*
iAIDA::AIDA_RootStore::RootTree::variableAddress( int variableIndex ) const
{
  if ( variableIndex < 0 || variableIndex >= static_cast<int>( m_cache.size() ) ) return 0;
  return m_cache[ variableIndex ].second;
}

void
iAIDA::AIDA_RootStore::RootTree::setVariableAddress( int variableIndex, void * p )
{
  if ( variableIndex < 0 || variableIndex >= static_cast<int>( m_cache.size() ) ) return ;
  m_cache[ variableIndex ].second = p;
  return;
}

// find corresponding root symbol for the type and create the corresponding cache 

void 
iAIDA::AIDA_RootStore::RootTree::describeTreeType(int i, const std::string & type) 
{ 
  std::string typeId = ""; 

  // for basic types 
  if ( type == "float")   { 
    typeId = "F"; 
    writeDataDescription<float>(  m_varList[i] + "/" + typeId );
    return;
  } 
  else if ( type == "double") {     
    typeId = "D"; 
    writeDataDescription<double>(  m_varList[i] + "/" + typeId );
    return;
  }
  else if ( type == "int")   { 
     typeId = "I";
     writeDataDescription<int>(  m_varList[i] + "/" + typeId );
    return;
  } 
  else if ( type == "short") { 
    typeId = "S";
    writeDataDescription<short>(  m_varList[i] + "/" + typeId );
    return;
  }
  else if ( type == "char") { 
    typeId = "B";
    writeDataDescription<char>(  m_varList[i] + "/" + typeId );
    return;
  }
  // for vectors (arrays)  
  else { 
    std::size_t i1 = type.find("[");  
    std::size_t i2 = type.find("]");
    // found a vector type 
    if ( i1 != std::string::npos && i2 != std::string::npos && i1 < i2) {  
      if (type.find("float") != std::string::npos) { 
	typeId="F";
	writeGenericDataDescription( m_varList[i] + type.substr(i1,i2) + "/" + typeId); 
	return;
      }
      if (type.find("double") != std::string::npos) { 
	typeId="D";
	writeGenericDataDescription( m_varList[i] + type.substr(i1,i2) + "/" + typeId); 
	return;
      }
      if (type.find("int") != std::string::npos) { 
	typeId="I";
	writeGenericDataDescription( m_varList[i] + type.substr(i1,i2) + "/" + typeId); 
	return;
      }
      if (type.find("short") != std::string::npos) { 
	typeId="S";
	writeGenericDataDescription( m_varList[i] + type.substr(i1,i2) + "/" + typeId); 
	return;
      }
      if (type.find("char") != std::string::npos) { 
	typeId="B";
	writeGenericDataDescription( m_varList[i] + type.substr(i1,i2) + "/" + typeId); 
	return;
      }
    }
  }
  // for generic data hoping they will be recognized
  typeId = type; 
  writeGenericDataDescription( typeId );
  m_objType[i] =  true; 
  return;    
}

bool 
iAIDA::AIDA_RootStore::RootTree::isTypeSupported(const std::string & type) { 
    if ( type == "float"   ||   
	 type == "double"  ||
	 type == "int"     || 
	 type == "short"   || 
	 type == "char"     
	 
	 )     return true; 
    else 
      //      return false; 
      return true; 

}


template <typename T> 
void
iAIDA::AIDA_RootStore::RootTree::writeDataDescription(ColumnType col )
{
  void* address = new T;
  m_cache.push_back( std::make_pair( col, address ) );
}


void
iAIDA::AIDA_RootStore::RootTree::writeGenericDataDescription(ColumnType col )
{
  // fill with an empty address - later setVariableAddress must be called
  m_cache.push_back( std::make_pair( col, (void *)0 ) );
}

// unpack from root tree the variable description
bool 
iAIDA::AIDA_RootStore::RootTree::readVariableDescription() { 

  m_varList.clear();
  m_varType.clear();

//   std::vector<std::string> variableNames; 
//   std::vector<std::string> variableTypes; 
//   std::vector<double> columnMin; 
//   std::vector<double> columnMax; 
  // get variables and types 
  for (int i = 0; i < (m_tuple->GetListOfBranches())->GetEntries(); ++i) { 
    std::string colName =  m_tuple->GetListOfBranches()->At(i)->GetName(); 
    m_varList.push_back( colName );
 
    // get column type 
    // support only for a leaf for each branch

    std::string rootType = std::string(m_tuple->GetBranch(colName.c_str())->GetLeaf(colName.c_str())->GetTypeName());

    // remove _t from the type and make lower case
    std::string rtype = rootType; 
    if (rootType.find("_t") != std::string::npos) { 
      rtype =  rootType.substr(0, rootType.find( "_t" ) );
      // transform to lower case
      std::transform ( rtype.begin(), rtype.end(), rtype.begin(), ::tolower ); 
    }

    //std::cout << " read branch " << i <<" name " <<  m_varList[i] << " type " << rootType << " new type " << type << std::endl; 

    if ( ! isTypeSupported(rtype) ) { 
      std::cerr << "Root TTree with columns of type" + rtype + " is not yet supported" << std::endl;
      return false; 
    }
    m_varType.push_back(rtype); 

    // describe tree type and fill cache with pointers

    m_objType.push_back( false);
    describeTreeType( i, rtype ); 

  }
  return true;
}
