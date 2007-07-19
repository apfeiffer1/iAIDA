# include "FML/ResultVariable.h"

# include <iomanip>

namespace fml
{
// namespace FML
// {

std::ostream& operator << (std::ostream& os, const ResultVariable& v)
{
  os << "value = " << v.value() << " error = " << v.error();

  if(v.minosAvailable())
    os << " minos_eminus = " << v.minosErrorMinus()  
       << " minos_eplus = " << v.minosErrorPlus();
  else
    os << " minos not computed";

  return os;
}


void ResultVariable::tabPrintNames(std::ostream& os)
{
  int COL_WIDTH = os.width();

  os
     << std::setw(COL_WIDTH) << "Value"
     << std::setw(COL_WIDTH) << "Error"
     << std::setw(COL_WIDTH) << "Minos+"
     << std::setw(COL_WIDTH) << "Minos-";

  os.width(COL_WIDTH);
}

void ResultVariable::tabPrint(std::ostream& os) const
{
  int COL_WIDTH = os.width();

  os << std::setw(COL_WIDTH) << value()
     << std::setw(COL_WIDTH) << error();
      
  if(minosAvailable())
    os << std::setw(COL_WIDTH) << minosErrorPlus()
       << std::setw(COL_WIDTH) << minosErrorMinus();
  else
    os << std::setw(COL_WIDTH) << "no"
       << std::setw(COL_WIDTH) << "no";
  
  os.width(COL_WIDTH);
}

// }
}
