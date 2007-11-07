# include "DataXML/DataObject.h"
# include "DataXML/XMLStream.h"

using namespace DataXML;

int main()
{
  DataObject fit_e;

  fit_e.setName("fit");
  fit_e.setAttribute("version","1.0");

  int n = 3;

  DataObject par_e;

  par_e.setName("start");
  par_e.setAttribute("par_number", to_string(n));

  DataObject var_e;

  for(int i=0; i<n; i++)
    {
      var_e.setName(std::string("v") + to_string(i));
      var_e.setAttribute("value", to_string(1.5+i));

      par_e.appendChild(var_e);
    }

  fit_e.appendChild(par_e);

  DataObject data_e;
  
  data_e.setName("data");
  data_e.setAttribute("annotations","VERBATIM_DATA");
  data_e.setAttribute("points", "VERBATIM_DATA");

  fit_e.appendChild(data_e);

  // write on cout 

  
  //fit_e.write(std::cout);


  // output 
  OutputXMLStream xml_os(std::cout);
  xml_os.write(fit_e); 
  
#ifdef TEST_READ_XML 

  InputXMLStream xml_is;

  for(int i=0; i<2; i++)
    {
      std::cout << std::endl << "----------------- " << i << " ----------" << std::endl;

      const DataObject& empty = xml_is.read(std::cin);
      
      xml_os.write(empty); 
    }
  std::cout << std::endl << "------ rest of data ------" << std::endl;

  std::cout << std::cin.rdbuf();

#endif

  return 0;
}
