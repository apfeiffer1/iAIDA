# ifndef FMLLIB_FitMethod_h__included__
# define FMLLIB_FitMethod_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"

# include "FML/IFMLDataIterator.h"

# include <memory>
# include <assert.h>

namespace fml
{
// namespace FML {

/**
 * Fit method computes the quality of the fit.
 *
 * Examples: CHI2, Maximum likelihood.
 * 
 * Compute the quality of the fit between vector of points v and model
 * function m. Fixed vector of points v = [ {x1,y1},...,{xN,yN} ].
 * Fixed model function m = m(x,p), where p is vector of parameters.
 * The fitting method is denoted as F(p,m(x,p)) or F(p,m).  F is a
 * function of function m, hence free variable x vanishes and F is not
 * a function of x. F directly depends on p, i.e. its domain space is
 * model function parameter space.
 *
 * Derivative with respect to p in point p0 is:
 * dF(p0,m)/dp = dF(p0,dm(x,p0)/dp)/dm.
 *
 * Error policy:
 *
 * The value of error is the *half-lenght* of Y error bar.
 * In case of symmetric errors, the bottom part of error bar is
 * equal to top part of error bar and is equal to error.
 *
 * The bin width in histogram is mapped to the *whole-lenght* of X error bar.
 *
 * \ingroup FML_LIB
000 */

  class FitMethodFunction : public Function
  {
  public:
    /// Model must be connected before the FitMethodFunction may be used.
    void connectModel(IFMLDataIterator& dataIt, IFMLFunction &model);

    virtual double value(const std::vector<double>& x) const = 0;

    /// non-copyable.
    std::auto_ptr<IFMLFunction> clone() const 
    { 
      return std::auto_ptr<IFMLFunction>(); 
    }

    IFMLFunction *modelFunction() { return m_model; }
    IFMLFunction *modelFunction() const { return m_model; }
    
    IFMLDataIterator *dataIterator() { return m_data; }
    IFMLDataIterator *dataIterator() const { return m_data; }

    bool isStreamable() const { return false; }

  protected:

    FitMethodFunction(bool provides_grad, std::string typename_id);

  private:
    mutable IFMLFunction *m_model;
    mutable IFMLDataIterator *m_data;
  };


// }
}

# endif

