//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

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
