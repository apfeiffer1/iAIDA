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

# include "FML/MinimizationVariable.h"

# include <iostream>
# include <iomanip>

namespace fml
{

// namespace FML {

  // minuit's default is 1.0 
const double MinimizationVariable::defaultStepSize = 1.0;

std::ostream& operator << (std::ostream& os, const MinimizationVariable& v)
{
  return os << "start_value = " << v.value() << " step_size = " << v.stepSize()
	    << " " << v.bound();
}

void MinimizationVariable::tabPrintNames(std::ostream& os)
{
  int COL_WIDTH = os.width();

  os      
    << std::setw(COL_WIDTH) << "Value"
    << std::setw(COL_WIDTH) << "Lower"
    << std::setw(COL_WIDTH) << "Upper"
    << std::setw(COL_WIDTH) << "Step";

  os.width(COL_WIDTH);
}

void MinimizationVariable::tabPrint(std::ostream& os) const
{
  int COL_WIDTH = os.width();

  os << std::setw(COL_WIDTH) << value();
      
  if(bound().isBound())
    if(bound().isFixed())
      os << std::setw(COL_WIDTH) << "fixed at" 
	 << std::setw(COL_WIDTH) << bound().upperBound();
    else
      os << std::setw(COL_WIDTH)  << bound().upperBound()
	 << std::setw(COL_WIDTH)  << bound().lowerBound();
  else
    os << std::setw(COL_WIDTH) << "no"
       << std::setw(COL_WIDTH) << "no";
  
  os << std::setw(COL_WIDTH) << stepSize();
  
  os.width(COL_WIDTH);
}

// }
}
