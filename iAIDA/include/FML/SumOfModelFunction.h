# ifndef FMLLIB__SumOfModelFunction_h__included__
# define FMLLIB__SumOfModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/CompoundModelFunction.h"

namespace fml
{

// namespace FML
// {

class SumOfModelFunction : public CompoundModelFunction
{
public:

  SumOfModelFunction(bool members_owned);

  /// Constructing the function name "on the fly"...
  bool append(IFMLFunction *member_fun);

  void appendAmpParameters();

  double value(const std::vector<double>& x) const;
  const std::vector<double>& gradient(const std::vector<double>& x)  const;
  const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
  void normalize(bool on); 


 protected: 
 private: 

  bool m_first; 
  int m_iAmpl; 
}; 

// }

}

# endif
