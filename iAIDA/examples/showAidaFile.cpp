#include <fstream>
#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

#include "AIDA_CPP/Exceptions.h"
#include <iostream>
#include <string>
#include <typeinfo>
#include <boost/shared_ptr.hpp>


void showHex(const char * buf) {
   typedef unsigned int uint;
   std::cout << "got (buf) : '0x" 
             << std::hex << uint(buf[0]&0xff) << " "
             << std::hex << uint(buf[1]&0xff) << " " 
             << std::hex << uint(buf[2]&0xff) << " " 
             << std::hex << uint(buf[3]&0xff) << " " 
             << "'. " << std::endl;   
}

std::string findFileType(std::string fileName) {

   typedef unsigned int uint;

   char buf[5];

   std::ifstream is;
   is.open (fileName.c_str(), std::ios::in | std::ios::binary );
   is.read ( buf, 4 ) ; // read first 4 bytes
   is.close();
   buf[4] = '\0'; // "close" string, just in case ...

   // values from /usr/share/file/magic/compress on Mac OS X
   std::string result = "None";
   if ( (char(buf[0]&0xff) == '<' ) && ( char(buf[1]&0xff) == '?' ) ) {  // xml
      result = "uncompress";
   } else if ( ( uint(buf[0]&0xff) == 0x1f ) && 
               ( uint(buf[1]&0xff) == 0x9d ) ) {  // compress'd 
      result = "zlib";                        
   } else if ( ( uint(buf[0]&0xff) == 0x1f ) && 
               ( uint(buf[1]&0xff) == 0x8b ) ) {  // gzip'd 
      result = "gzip";
   } else if ( ( char(buf[0]&0xff) == 'B' ) && 
               ( char(buf[1]&0xff) == 'Z' ) && 
               ( char(buf[2]&0xff) == 'h' ) ) {  // bzip2'd ("BZh")
      result = "bzip";
   } else {
      // assume zlib compression (needs to uncompress to determine if things are ok ... :( )
      result = "zlib";
      // std::cout << "unknown file type. First four bytes are: " ;
      // showHex(buf);
   }
   return result;
}

int main( int argc, char** argv ) {
   
   std::string fileName("exaTree.aida");
   if (argc < 2) {
      std::cerr << "No file name given. Will use 'exaTree.aida' as default." << std::endl;
   } else {
      fileName = argv[1];
   }
   
   boost::shared_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );
   boost::shared_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() ); 
   bool readOnly  = true;
   bool createNew = false;
   
   std::string options = findFileType(fileName);
   std::cout << "found file " << fileName << " to be of type " << options << std::endl;
   boost::shared_ptr<AIDA::ITree> tree(tf->create(fileName,"XML",readOnly,createNew, options)); 

   std::cout << "tree listing : " << std::endl; 
  // show the tree
   tree->ls(); 

//   bool recursive = true;
//   std::vector<std::string> objectNames = tree->listObjectNames("/", recursive);
//   std::vector<std::string> objectTypes = tree->listObjectTypes("/", recursive);
//   
//   std::vector<std::string>::const_iterator objNItr = objectNames.begin();
//   std::vector<std::string>::const_iterator objTItr = objectTypes.begin();
//   for (; objNItr != objectNames.end(); objNItr++ , objTItr++) {
//      std::cout << " name " << *objNItr << "\t of type " << *objTItr << std::endl;
//   }

   tree->close(); 

   std::cout << "That's it !" << std::endl;

   return 0;
}
