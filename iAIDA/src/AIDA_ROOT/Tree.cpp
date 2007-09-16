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

