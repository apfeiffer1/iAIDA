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
