/**
 * \anchor dxml_copy_example
 * \example dxml_copy_example.cpp
 *
 * Read XML data from cin and print it back to cout.
 */

# include "DataXML/DataObject.h"
# include "DataXML/XMLStream.h"
# include "fstream"
# include "sys/times.h"

using DataXML::DataObject;
using DataXML::InputXMLStream;
using DataXML::OutputXMLStream;

int main()
{
  clock_t start,stop;
  start = times(0);

  std::ifstream XMLFile("test.xml");
  
  InputXMLStream xml_is;

  const DataObject& data = xml_is.read(XMLFile);
  
  std::cout << "read" << std::endl;  

  stop = times(0);
  std::cout << "CLK: " << stop-start << std::endl;
  //OutputXMLStream xml_os;
  //xml_os.write(data,cout); 

  return 0;
}

