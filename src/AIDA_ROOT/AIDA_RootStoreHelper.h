#include  <string>
#include <vector>
namespace AIDA_RootStore_Helper {
  static const std::string emptyString="<undefined>";

  extern int indexAida(int iRoot, int nBin);
  extern int getEntries(double height, double error); 


  extern std::string aidaType(const std::string & rootType);
 
  static const std::vector<double> emptyVectorDouble;
  static const std::vector<std::string> emptyVectorString;


}
