//
// convert file to XML format
//

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITuple.h>
#include <AIDA/ITupleFactory.h>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/filesystem.hpp>

// #define DEBUG_PRINT ON 

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
      // assume we have zlib compression (needs to uncompress to determine if things are ok ... )
      result = "zlib";
   }
   return result;
}

class FileConverter {

public:
   FileConverter();
   ~FileConverter();

   void setup();

   bool processTuples(std::string inPath, std::string outPath, AIDA::ITree *outTree);
   bool convertTuple(std::string inPath, std::string outPath, std::string tupName, AIDA::ITree *outTree);

   bool processFile(const std::string& inFile, 
                    const std::string& inType, 
                    const std::string& outFile);

private:
   bool isInitialized;
   AIDA::IAnalysisFactory *anaFactory;
   AIDA::ITreeFactory     *treeFactory;
   AIDA::ITree            *memTree;
};

FileConverter::FileConverter() : isInitialized(false), anaFactory(0), treeFactory(0), memTree(0) {
   if ( !isInitialized ) setup();
}

void FileConverter::setup() {

   anaFactory = AIDA_createAnalysisFactory();
   if ( !anaFactory ) {
      std::cout << "ERROR: AIDA not found." << std::endl;
      exit(-1);
   }
   treeFactory = anaFactory->createTreeFactory();
   if ( !treeFactory ) {
      std::cout << "ERROR : can't get a TreeFactory." << std::endl;
      exit(-1);
   }
   // create an in-memory tree to act as a common handle.
   memTree = treeFactory->create();
   if( !memTree ) {
      std::cout << "process_file : can't create in-memory tree !" << std::endl;
      exit(-1);
   }
   isInitialized = true;
}

FileConverter::~FileConverter() {
   
   // clean up ...
   
   memTree->close();
   delete memTree;

   delete treeFactory;
   delete anaFactory;
}


bool FileConverter::convertTuple(std::string inPath, std::string outPath, std::string tupName, AIDA::ITree *outTree) {
   
#ifdef DEBUG_PRINT
   std::cout << "convertTuple : from " << inPath << " to " << outPath << std::endl;
#endif
   
   AIDA::IManagedObject* object = memTree->find(inPath);
   if(!object) {
     std::cout << "FATAL> convertTuple : object " << inPath << " not found in tree." << std::endl;
     return false;
   }
   
   AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
   if(!tuple) {
     std::cout << "convertTuple : object at " << inPath << " is not an AIDA::ITuple." << std::endl;
     return false;
   }

   int cols = tuple->columns();
#ifdef DEBUG_PRINT
   for(int index=0; index<cols; index++) {
       std::cout << "convertTuple :"
                 << " icol = " << index
                 << ", label = " << tuple->columnName(index) 
                 << ", type = " << tuple->columnType(index) 
                 << std::endl;
   }
   std::cout << "process_file : rows = " << tuple->rows() << std::endl;
#endif

   std::string namesAndTypes;
   for(int index=0; index<cols; index++) {
      namesAndTypes += tuple->columnType(index) + " " + tuple->columnName(index) +"; ";
   }
#ifdef DEBUG_PRINT
   std::cout << "INFO> namesAndTypes: " << namesAndTypes << std::endl;
#endif

   boost::shared_ptr<AIDA::ITupleFactory> tupF ( anaFactory->createTupleFactory(*outTree) ) ;
   std::string tupleOptions = "--RWN";
   AIDA::ITuple * outTup = tupF->create(outPath, tupName, namesAndTypes, tupleOptions);
   if ( !outTup ) {
     std::cerr << "ERROR creating tuple !!" << std::endl;
     return -1;
   }
   
   std::vector<std::string> types = tuple->columnTypes();
   tuple->start();
   int index=0;
   while( tuple->next() ) {
      index++;
      for( int col=0; col<cols; col++) {
         if(types[col]=="float") {
            float v = tuple->getFloat(col);
            outTup->fill(col, v);
         } else if(types[col]=="double") {
            double v = tuple->getDouble(col);
            outTup->fill(col, v);
         } else if(types[col]=="char") {
            char v = tuple->getChar(col);
            outTup->fill(col, v);
         } else if(types[col]=="short") {
            short v = tuple->getShort(col);
            outTup->fill(col, v);
         } else if(types[col]=="int") {
            int v = tuple->getInt(col);
            outTup->fill(col, v);
         } else if(types[col]=="long") {
            long v = tuple->getLong(col);
            outTup->fill(col, v);
         } else if(types[col]=="boolean") {
            bool v = tuple->getBoolean(col);
            outTup->fill(col, v);
         } else if(types[col]=="string") {
   	      std::string v = tuple->getString(col);
            outTup->fill(col, v);
         } else {
            std::cout << "process_file : unknown type." << types[col] << std::endl;
            return false;
         }
     } // end loop all columns
     if ( ! outTup->addRow() ) {
        std::cerr << "ERROR: Could not add tuple row " << index << std::endl;
     }
   } // end looping all rows

#ifdef DEBUG_PRINT
   std::cout << "convertTuple : converted " << index << " rows " << std::endl;
#endif


   return true;
}

bool FileConverter::processTuples(std::string inPath, std::string outPath, AIDA::ITree *outTree) {

   bool recursive = true;
   std::vector<std::string> namesIn = memTree->listObjectNames(inPath, recursive);
   
   std::vector<std::string>::const_iterator inItr;
   std::string outName;
   std::string inName;
   for ( inItr=namesIn.begin(); inItr != namesIn.end(); inItr++) {
      inName = inPath+"/"+*inItr;
      AIDA::IManagedObject* object = memTree->find( inName );
      if ( !object ) { 
         std::cout << "FATAL> processTuples : object " << inName << " not found in tree." << std::endl;
         return false;
      }

      AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
      if( !tuple )  continue;  // object is not a tuple, ignore and continue

      // here we have found a tuple in the input tree, fix name for output and copy over
      outName = inName;
      boost::replace_all(outName, inPath, outPath);
      
      // create the full name of the output object, then get the "dir" part
      std::vector<std::string> splitPath;
      boost::split(splitPath, outName, boost::is_any_of("/"));
      std::vector<std::string> foo(splitPath.end()-1, splitPath.end()); // get last element (name of object)
      std::string tupName = boost::algorithm::join(foo, "");
      
      std::vector<std::string> newOut(splitPath.begin(), splitPath.end()-1); // ignore last element (name of object)
      std::string outDir = boost::algorithm::join(newOut, "/");
#ifdef DEBUG_PRINT
      std::cout << "INFO> " << outPath << " splits into ";
      for (std::vector<std::string>::const_iterator itr=splitPath.begin(); itr != splitPath.end(); itr++){
         std::cout << "'" << *itr << "' ,";
      }
      std::cout << std::endl;
      std::cout << "INFO> outDir " << outDir << std::endl;
#endif
      memTree->mkdirs( outDir );

      if ( ! convertTuple( inName, outPath, tupName, outTree ) ) {
         std::cout << "FATAL> processTuples : error when copying tuple from "<< inName << " to " << outName << std::endl;
      }
   }
   return true;
}

bool FileConverter::processFile(const std::string& inFile, 
                                const std::string& inType, 
                                const std::string& outFile) {

   if ( !isInitialized ) {
      std::cout << "hmm ..." << std::endl;
      setup();
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

   memTree->mkdir("/in");
   memTree->mkdir("/out");
   
   bool recursive = true;

   // mount the input and output trees and use the cp() method to get the objects copied over
   if ( ! memTree->mount("/in" , *inTree , "/") ) std::cout << "Error mounting inTree" << std::endl;
   if ( ! memTree->mount("/out", *outTree, "/") ) std::cout << "Error mounting outTree" << std::endl;

#ifdef DEBUG_PRINT
   std::cout << "\n inTree :" << std::endl;
   inTree->ls("/", recursive);
   std::cout << "\n memTree (before):" << std::endl;
   memTree->ls("/", recursive);
#endif

   if ( ! memTree->cp("/in", "/out", recursive) ) std::cout << "Error copying all from /in to /out" << std::endl;
   if ( ! processTuples("/in", "/out", outTree) ) std::cout << "Error copying tuples from /in to /out" << std::endl;

#ifdef DEBUG_PRINT
   std::cout << "\n memTree (after):" << std::endl;
   memTree->ls("/", recursive);
#endif

   if ( ! memTree->unmount("/in")  ) std::cout << "Error unmounting /in " << std::endl;
   if ( ! memTree->unmount("/out") ) std::cout << "Error unmounting /out" << std::endl;

#ifdef DEBUG_PRINT
   std::cout << "\n outTree :" << std::endl;
   outTree->ls("/", recursive);
#endif

   if ( ! outTree->commit() ) std::cout << "Error committing /out" << std::endl;
   if ( ! outTree->close()  ) std::cout << "Error closing /out" << std::endl;

   inTree->close();

   delete outTree;
   delete inTree;

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


   if ( ! boost::filesystem::exists( inFile ) ) {
      std::cerr << "\nERROR: requested input file " << inFile << " does not exist. Aborting.\n" << std::endl;
      return -1;
   }

   if ( boost::filesystem::exists( outFile ) ) {
      std::cerr << "\nERROR: requested output file " << outFile << " already existing. Aborting to not overwrite ...\n" << std::endl;
      return -1;
   }

   FileConverter fc;
   bool result = fc.processFile(inFile, inType, outFile);

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
   
   // std::cout << "That's it !" << std::endl;
   
   return 0;
}
