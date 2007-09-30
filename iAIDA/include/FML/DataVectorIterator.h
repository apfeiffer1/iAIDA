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

#ifndef FML_DATAVECTORITERATOR_H
#define FML_DATAVECTORITERATOR_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include "FML/DataVector.h"
# include "FML/IFMLDataIterator.h"

namespace fml
{

// namespace FML
// {
  class DataVectorIterator : virtual public IFMLDataIterator
  {

  public:
    explicit DataVectorIterator(const DataVector& v);

    void start();
    bool current() const;
    void next();
    int numberOfEntries() const;
    const std::vector<double> &coords() const;
    double value() const;
    double vErrorPlus() const;
    double vErrorMinus() const;

    const DataVector * dataVec() const { return m_vector; } 

  private:

    const DataVector *m_vector;
    int m_idx;
    int m_size; 

  };
// }
}


#endif
