/**
 * \anchor dxml_copy_example
 * \example dxml_copy_example.cpp
 *
 * Read XML data from cin and print it back to cout.
 */

# include "DataXML/DataObject.h"
# include "DataXML/XMLSealStream.h"
# include "DataXML/XMLStream.h"
# include "fstream"
# include "sys/times.h"
# include "SealIOTools/StdInputStream.h"
# include "SealIOTools/StdOutputStream.h"
#include "SealZip/BZIPInputStream.h"
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealIOTools/StorageInputStream.h"
#include "SealIOTools/BufferInputStream.h"
#include "SealUtil/SealTimer.h"


int main()
{
  //clock_t start,stop;
  //start = times(0); 
  seal::SealTimer * timer = new seal::SealTimer(); 


  seal::File       input ("test.bzip"); 

  seal::StorageInputStream sinput (&input);
  seal::BufferInputStream  binput (&sinput);
  seal::BZIPInputStream    zinput(&binput);

  
  DataXML::InputXMLSealStream xml_is(zinput);

  const DataXML::DataObject& data = xml_is.read();
    
  std::cout << "read" << std::endl;  

  delete timer; 

  
  // measure writing time
  timer = new seal::SealTimer(); 

  std::ofstream XMLFile2("test3.xml");
  seal::StdOutputStream file2(&XMLFile2);


  DataXML::OutputXMLSealStream xml_os(file2);

  xml_os.write(data, file2); 


  delete timer;

  return 0;
}

