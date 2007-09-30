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

#ifndef FML_IFMLDATAITERATOR_H
#define FML_IFMLDATAITERATOR_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <vector>

namespace fml
{
// namespace FML
// {

  class DataVector; 

class IFMLDataIterator
{
public:

  virtual void start() = 0;
  virtual bool current() const = 0;
  virtual void next() = 0;
  virtual int numberOfEntries() const = 0;
  virtual const std::vector<double> &coords() const = 0;
  virtual double value() const = 0;
  virtual double vErrorPlus() const = 0;
  virtual double vErrorMinus() const = 0;
  
  virtual ~IFMLDataIterator() {}
};

// }
}


#endif
