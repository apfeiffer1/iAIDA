# ifndef MINFIT_MinimizationVariable_h__included__
# define MINFIT_MinimizationVariable_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include <iosfwd>

# include "FML/patches.h"

# include "FML/Variable.h"
# include "FML/Bound.h"


namespace fml
{

// namespace FML {

/**
 * Implementation of MinimizationVariable.
 *
 * MinimizationVariable is used on input for the minimizer.
 * Its value is a starting point for minimization, it has bounds and step size.
 *
 */
class MinimizationVariable : public Variable
{
 public:

  MinimizationVariable();
  MinimizationVariable(double v);
  MinimizationVariable(double v, const Bound& b, double sz);

  Bound& bound() { return m_bound; }
  const Bound& bound() const { return m_bound; }

  double stepSize() const { return m_stepSize; }
  void setStepSize(double s) { m_stepSize = s; }

  static const double defaultStepSize;// = 1.0; // PENDING: works on Linux but not on Solaris -> definition moved to .cpp

  /// read in the contents in XML from istream
  bool fromXML(std::istream& is);

  /// save the contents in XML to ostream
  void toXML(std::ostream& os);

  void tabPrint(std::ostream& os) const;
  static void tabPrintNames(std::ostream& os);

 private:
  Bound m_bound;
  double m_stepSize;
};

inline MinimizationVariable::MinimizationVariable() : Variable(1.0), m_bound(Bound()), m_stepSize(defaultStepSize) {}

 inline MinimizationVariable::MinimizationVariable(double v) : Variable(v), m_bound(Bound()), m_stepSize(defaultStepSize) {}

inline MinimizationVariable::MinimizationVariable(double v, const Bound& b, double sz) : Variable(v), m_bound(b), m_stepSize(sz) {}

std::ostream& operator << (std::ostream& os, const MinimizationVariable& v);

// }

}

# endif
