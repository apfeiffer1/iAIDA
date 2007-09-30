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

# include "FML/DataVectorIterator.h"

namespace fml
{
// namespace FML
// {

  DataVectorIterator::DataVectorIterator(const DataVector& v)
  {
    m_idx = 0;
    m_vector = const_cast<DataVector*>(&v);  //-ap: to const_cast or not to const_cast (mod .h)  ?!?!?!?
    m_size =  m_vector->nPoints();
  }

  void DataVectorIterator::start()
  {
    m_idx = 0;
  }

  bool DataVectorIterator::current() const
  {
    return m_idx < m_size;
  }

  void DataVectorIterator::next()
  {
    ++m_idx;
  }

  int DataVectorIterator::numberOfEntries() const
  {
    //return m_vector->nPoints();
    return m_size; 
  }

  const std::vector<double> &DataVectorIterator::coords() const
  {
    return m_vector->point(m_idx).coords();
  }

  double DataVectorIterator::value() const
  {
    return m_vector->point(m_idx).value();
  }

  double DataVectorIterator::vErrorPlus() const
  {
    return m_vector->point(m_idx).error();
  }

  double DataVectorIterator::vErrorMinus() const
  {
    return m_vector->point(m_idx).error();
  }
// }
}
