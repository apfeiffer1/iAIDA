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
//# include "SealIOTools/StdOutputStream.h"
#include "SealZip/GZIPOutputStream.h"
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealIOTools/StorageOutputStream.h"
#include "SealIOTools/BufferOutputStream.h"
#include "SealUtil/SealTimer.h"


int main()
{
  //clock_t start,stop;
  //start = times(0); 
  seal::SealTimer * timer = new seal::SealTimer(); 

  std::ifstream XMLFile("test.xml");
  seal::StdInputStream file(&XMLFile);
  
  
  DataXML::InputXMLSealStream xml_is(file);

  const DataXML::DataObject& data = xml_is.read();
    
  std::cout << "read" << std::endl;  

  delete timer; 

  
  // measure writing time
  timer = new seal::SealTimer(); 


  seal::File       output ("test.gz", seal::IOFlags::OpenWrite
			   | seal::IOFlags::OpenCreate | seal::IOFlags::OpenTruncate );
  seal::StorageOutputStream soutput (&output);
  seal::BufferOutputStream  boutput (&soutput);
  //seal::StorageOutputStream * soutput = new seal::StorageOutputStream(&output);
  //seal::BufferOutputStream  * boutput = new seal::BufferOutputStream (soutput);
  seal::GZIPOutputStream   *   zoutput= new seal::GZIPOutputStream(&boutput);
  

  DataXML::OutputXMLSealStream xml_os(*zoutput);

  xml_os.write(data); 

  zoutput->close();

  delete zoutput;
  //delete boutput;
  //delete soutput;

  delete timer;

  return 0;
}

