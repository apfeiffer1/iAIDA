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


int main()
{
  clock_t start,stop;
  start = times(0);

  std::ifstream XMLFile("test.xml");
  seal::StdInputStream file(&XMLFile);
  
  DataXML::InputXMLSealStream xml_is(file);

  const DataXML::DataObject& data = xml_is.read();
  
  std::cout << "read" << std::endl;  

  stop = times(0);
  std::cout << "CLK: " << stop-start << std::endl;

  std::ofstream XMLFile2("test2.xml");
  seal::StdOutputStream file2(&XMLFile2);


  DataXML::OutputXMLSealStream xml_os(file2);

  xml_os.write(data, file2); 

  return 0;
}

