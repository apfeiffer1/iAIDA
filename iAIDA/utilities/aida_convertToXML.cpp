//
// convert file to XML format
//

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITuple.h>
#include <iostream>
#include <fstream>

// #define NDEBUG 

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
   }
   return result;
}


bool process_file(AIDA::IAnalysisFactory& aAIDA, 
                  const std::string& inFile, 
                  const std::string& inType, 
                  const std::string& outFile) {

   AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
   if(!treeFactory) {
      std::cout << "process_file : can't get a TreeFactory." << std::endl;
      return false;
   }
   
   // first create an in-memory tree to act as a common handle.
   // we then mount the input and output trees and use the cp() method to get the objects copied over

   AIDA::ITree* memTree = treeFactory->create();
   if( !memTree ) {
      std::cout << "process_file : can't create in-memory tree !" << std::endl;
      return false;
   }
   
   // open input file readonly, do not create new one if it doesn't exist. 
   // check type of file if we have a xml file derivative
   std::string options("");
   if ( inType == "xml" ) {
      options = findFileType(inFile);
      std::cout << "found file " << inFile << " to be of type " << options << std::endl;
   }
   AIDA::ITree* inTree = treeFactory->create(inFile, inType, true, false, options);
   if( ! inTree ) {
      std::cout << "process_file : can't open input file " << inFile << std::endl;
      return false;
   }
   
   // open output file in read/write, create new one if it doesn't exist
   options = "uncompress"; // use uncompressed xml for output
   AIDA::ITree* outTree = treeFactory->create(outFile, "xml", false, true, options);
   if( !outTree ) {
      std::cout << "process_file : can't open output file " << outFile << std::endl;
      return false;
   }

   bool recursive = true;

   memTree->mkdir("/in");
   memTree->mkdir("/out");
   
   if ( ! memTree->mount("/in" , *inTree , "/") ) std::cout << "Error mounting inTree" << std::endl;
   if ( ! memTree->mount("/out", *outTree, "/") ) std::cout << "Error mounting outTree" << std::endl;

#ifdef NDEBUG
   std::cout << "\n inTree :" << std::endl;
   inTree->ls("/", recursive);

   std::cout << "\n memTree (before):" << std::endl;
   memTree->ls("/", recursive);
#endif

   if ( ! memTree->cp("/in", "/out", recursive) ) std::cout << "Error copying /in to /out" << std::endl;

#ifdef NDEBUG
   std::cout << "\n memTree (after):" << std::endl;
   memTree->ls("/", recursive);
#endif

   if ( ! memTree->unmount("/in")  ) std::cout << "Error unmounting /in " << std::endl;
   if ( ! memTree->unmount("/out") ) std::cout << "Error unmounting /out" << std::endl;

#ifdef NDEBUG
   std::cout << "\n outTree :" << std::endl;
   outTree->ls("/", recursive);
#endif

   if ( ! outTree->commit() ) std::cout << "Error committing /out" << std::endl;
   if ( ! outTree->close()  ) std::cout << "Error closing /out" << std::endl;

   inTree->close();

   memTree->close();

   // delete outTree;
   delete inTree;
   delete memTree;

   delete treeFactory; // not needed any more

   std::cout << std::endl;

   return true;
}
//////////////////////////////////////////////////////////////////////////////

#include <fstream>

   int main(int argc,char* argv[]) {

      std::string prgName = std::string(argv[0]);

      std::string inFile("../exatup.aida");
      std::string inType("xml");
      std::string outFile("../exatup.xml");
      if ( argc != 4 ) {
         std::cout << prgName << ": three arguments expected : "
            << "<inputFile> <inputType> <outputFile> "
            << "-- but none given. Will assume: '" << inFile << " " << inType << " " << outFile <<"' "
            << std::endl;
      } else { // override defaults from args
         inFile  = std::string(argv[1]);
         inType  = std::string(argv[2]);
         outFile = std::string(argv[3]);
      }

      AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
      if(!aida) {
         std::cout << prgName << ": AIDA not found." << std::endl;
         return 1;
      }

      bool result = process_file(*aida, inFile, inType, outFile);
      // clean up 
      delete aida;

      if(!result){
         std::cout << prgName << ": "
            << "converting file " << inFile << " to " << outFile << " failed."
            << std::endl;
         return -1;
      } else {
         std::cout << prgName << ": "
            << "file " << inFile << " to " << outFile << " converted successfully."
            << std::endl;
      }


      if (!result) {
      }

      std::cout << "That's it !" << std::endl;
      return 0;
   }
