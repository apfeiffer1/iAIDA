#include <fstream>


int main()
{
  std::ofstream XMLFile("test.xml");

  XMLFile << "<root>" << std::endl;


  
  for (int i = 0; i < 10000; i++)
    {
      XMLFile << "<line col1=\"1.123456+e10\" col10=\"1.123456+e10\" col11=\"1.123456+e10\" col12=\"1.123456+e10\" col13=\"1.123456+e10\" col14=\"1.123456+e10\" col15=\"1.123456+e10\" col16=\"1.123456+e10\" col17=\"1.123456+e10\" col18=\"1.123456+e10\" col19=\"1.123456+e10\" col2=\"1.123456+e10\" col20=\"1.123456+e10\" col3=\"1.123456+e10\" col4=\"1.123456+e10\" col5=\"1.123456+e10\" col6=\"1.123456+e10\" col7=\"1.123456+e10\" col8=\"1.123456+e10\" col9=\"1.123456+e10\"/>" << std::endl;
    }



  XMLFile << "</root>" << std::endl;
  return 0;
} 
