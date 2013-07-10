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

/** implementation of a Root tree based on a root TFile 
 */ 

#include "TFile.h"
#include "AIDA_ROOT/Tree.h"

AIDA_ROOT::Tree::Tree(const std::string & storeName, bool readOnly, bool createNew, const std::string & options ) 
{ 

  std::string fileOption = "NEW"; 
  if (createNew) 
    fileOption = "RECREATE"; 
  if (readOnly) 
    fileOption = "READ";

  // parse option ( default is now compress files) 
  int icompress = 1;
  if (!options.empty() ) { 
    if (options.find("uncompress") != std::string::npos )
        icompress = 0; 
  }
  
  boost::shared_ptr<TFile> f(new TFile(storeName.c_str(),fileOption.c_str(),storeName.c_str(),icompress)); 
  rootFile = f;

  m_name = rootFile->GetName(); 

}

bool AIDA_ROOT::Tree::commit() { 

  // std::cout << "Tree::commit: writing....."  << std::endl; 
  return rootFile->Write() != 0; 
}

bool AIDA_ROOT::Tree::close() { 

  // delete objects 
  //std::cout << "closing the tree - destroy objects" << std::endl;  
  m_H1D.clear(); 
  m_H2D.clear(); 
  m_H3D.clear(); 
  m_P1D.clear(); 
  m_P2D.clear(); 

  //ls(); 
  //std::cout << "closing the tree - close root file" << std::endl;  
  rootFile->Close(); 

  return true; 
} 

bool AIDA_ROOT::Tree::ls(const std::string &, bool, std::ostream &) const { 
  rootFile->ls(); 
  return true;
}

