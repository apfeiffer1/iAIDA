# include "FML/MinimizerControlParams.h"

namespace fml
{

// namespace FML {

  MinimizerControlParams::MinimizerControlParams() : m_maxit(),  m_strategy() , m_tol(), m_prec(), m_up()
{

  useImplicitMaxIter();
  useImplicitTolerance();
  useImplicitPrecision();
  useImplicitErrorUP();
  useImplicitStrategy();
}


  /*
const int MinimizerControlParams::MAXITER_DEFAULT = 100;

// from Minuit (experimental) default
const double MinimizerControlParams::PRECISION_DEFAULT = 1.0e-15; 

// from NAG specification (prec^0.8)
const double MinimizerControlParams::TOLERANCE_DEFAULT = 6.3e-12;  

// PENDING: or maybe should be 0.5 ?
const double MinimizerControlParams::ERRORUP_DEFAULT = 1.0;
  */
// }
}
