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

#include "AIDA/IAxis.h"
#include "AIDA_RootConverterHelper.h"
#
namespace AIDA_RootConverter_Helper { 


  /** conversion from Root bin indices to AIDA 
   *
   **/

  int indexAida(int iRoot, int nBin)  {

    if (iRoot == 0) return AIDA::IAxis::UNDERFLOW_BIN; 
    if (iRoot == nBin + 1) return  AIDA::IAxis::OVERFLOW_BIN;
    return iRoot -1; 
  }

  int getEntries(double binHeight, double binError ) { 
     if (binHeight <=0 ) return 0;
      double xx =  binHeight/binError;
      return int(xx*xx+0.5);
  }

}  
