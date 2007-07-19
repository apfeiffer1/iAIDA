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
