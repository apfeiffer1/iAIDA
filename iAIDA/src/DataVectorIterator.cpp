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
