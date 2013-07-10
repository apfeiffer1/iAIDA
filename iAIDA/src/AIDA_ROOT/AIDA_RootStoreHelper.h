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
