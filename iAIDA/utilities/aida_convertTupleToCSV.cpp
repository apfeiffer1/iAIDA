
// AIDA tuple to CSV format file
//
// from Guy Barrands code published at:
//    http://hypernews.slac.stanford.edu/HyperNews/geant4/get/analysis/392/3.html
// 

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITuple.h>
#include <iostream>

bool process_file(AIDA::IAnalysisFactory& aAIDA,
                  const std::string& a_file,
                  const std::string& a_format,
                  const std::string& a_path,
                  std::ostream& a_out) {

  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) {
    std::cout << "process_file : can't get a TreeFactory." << std::endl;
    return false;
  }
  AIDA::ITree* tree = treeFactory->create(a_file,a_format,true,false);
  delete treeFactory;
  if(!tree) {
    std::cout << "process_file : can't open data file." << std::endl;
    return false;
  }
  std::string dir;
  std::string name;
 {std::string::size_type pos = a_path.rfind('/');
  if(pos==std::string::npos) {
    name = a_path;
  } else {
    dir = a_path.substr(0,pos);pos++;
    name = a_path.substr(pos,a_path.size()-pos);  
  }}
  if(dir.size()){
    if(!tree->cd(dir)) {
      std::cout << "process_file :"
                << " can't cd to " << dir << "."
                << std::endl;
      delete tree;
      return false;
    }
  }
  AIDA::IManagedObject* object = tree->find(name);
  if(!object) {
    std::cout << "process_file : "
              << " object " << name << " not found in tree." 
              << std::endl;
    delete tree;
    return false;
  }
  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
  if(!tuple) {
    std::cout << "process_file : object not an AIDA::ITuple." << std::endl;
    delete tree;
    return false;
  }
  //bool verbose = false;
  bool verbose = true;
  if(verbose) {
    int coln = tuple->columns();
    for(int index=0;index<coln;index++) {
      std::cout << "process_file :"
                << " icol = " << index
                << ", label = " << tuple->columnName(index) 
                << ", type = " << tuple->columnType(index) 
                << std::endl;
    }
    std::cout << "process_file : rows = " << tuple->rows() << std::endl;
  }
  int coln = tuple->columns();
  std::vector<std::string> types = tuple->columnTypes();
  char sep = ',';
  tuple->start();
  while(tuple->next()) {
    for(int col=0;col<coln;col++) {
      if(col) a_out << sep;
      if(types[col]=="float") {
        float v = tuple->getFloat(col);a_out << v;
      } else if(types[col]=="double") {
        double v = tuple->getDouble(col);a_out << v;
      } else if(types[col]=="char") {
        char v = tuple->getChar(col);a_out << v;
      } else if(types[col]=="short") {
        short v = tuple->getShort(col);a_out << v;
      } else if(types[col]=="int") {
        int v = tuple->getInt(col);a_out << v;
      } else if(types[col]=="long") {
        long v = tuple->getLong(col);a_out << v;
      } else if(types[col]=="boolean") {
        bool v = tuple->getBoolean(col);a_out << v;
      } else if(types[col]=="string") {
	std::string v = tuple->getString(col);a_out << v;
      } else {
        std::cout << "process_file : unknown type." << types[col] << std::endl;
        delete tree;
        return false;
      }
    }
    a_out << std::endl;
  }
  delete tree;
  return true;
}
//////////////////////////////////////////////////////////////////////////////

#include <fstream>

int main(int argc,char* argv[]) {

  std::string file("../examples/exatup.aida");
  std::string format("xml");
  std::string tuple("100");          
  if(argc!=4) {
      std::cout << "main : three arguments expected :"
              << " <file>, <format>, <tuple path>"
              << " -- but none given. Will assume: '" << file << " "<< format << " " << tuple <<"' "
              << std::endl;
  } else { // override defaults from args
      file   = std::string(argv[1]);
      format = std::string(argv[2]);
      tuple  = std::string(argv[3]);
  }
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "main : AIDA not found." << std::endl;
    return 1;
  }
  std::ofstream out("out.csv");
  if(out.fail()) {
    std::cout << "main : can't open out.csv." << std::endl;
    return 1;
  }

  bool result = process_file(*aida,file,format,tuple,out);
  if(!result){
    std::cout << "main :"
              << " read_file " << file << " failed."
              << std::endl;
  }
  out.close();
  delete aida;

  if (!result) {
      return -1;
  }

  std::cout << "That's it !" << std::endl;
  return 0;
}
