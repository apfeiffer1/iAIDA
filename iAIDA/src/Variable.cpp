# include "FML/Variable.h"

namespace fml
{

// namespace FML
// {

void Variable::tabPrint(std::ostream& os) const
{
  int COL_WIDTH = os.width();

  os << std::setw(COL_WIDTH) << value();

  os.width(COL_WIDTH);
}

void Variable::tabPrintNames(std::ostream& os)
{
  int COL_WIDTH = os.width();

  os
    << std::setw(COL_WIDTH) << "Value";
  
  os.width(COL_WIDTH);
}

// }

}
