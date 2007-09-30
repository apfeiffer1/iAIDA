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

# include "FML/ExponentialModelFunction.h"
# include "FML/Util/Plugin.h"

namespace fml {
  // namespace FML {

  /// f(x) = exp(x), that is: amp = 0.0, slope = 1.0.
  ExponentialModelFunction::ExponentialModelFunction() : Function(1,2,true,true, false, ExponentialModelFunction::functionTypename())
    {
      std::vector<double> p(2);

      p[AMP_IDX] = 0.0;
      p[SLOPE_IDX] = 1.0;

      // virtual not overriden so safe in constructor 
      // but explicit name scope anyway
      Function::setParameters(p);

      constructor_init();
    }

  /// f(x) = exp(amp+slope*x)
  ExponentialModelFunction::ExponentialModelFunction(double amp, double slope) : Function(1,2,true,true, false, ExponentialModelFunction::functionTypename())
    {
      std::vector<double> p(2);

      p[AMP_IDX] = amp;
      p[SLOPE_IDX] = slope;

      // virtual not overriden so safe in constructor 
      // but explicit name scope anyway
      Function::setParameters(p);  
      constructor_init();   
    }

    void ExponentialModelFunction::constructor_init()
    {
      setParName(AMP_IDX,"amp");
      setParName(SLOPE_IDX,"slope");
    }

  double ExponentialModelFunction::value(const std::vector<double>& x_vec) const
    {
      assert(x_vec.size()>0);
      return exp(slope()*x_vec[0]+amp());
    }

  const std::vector<double>& ExponentialModelFunction::gradient(const std::vector<double>& x_vec) const
  {
    assert(x_vec.size()>0);
    m_grad_buf[0] = exp(slope()*x_vec[0]+amp())*slope();
    return m_grad_buf;
  }

  const std::vector<double>& ExponentialModelFunction::parameterGradient(const std::vector<double>& x_vec) const
  {
    assert(x_vec.size()>0);
    double x = x_vec[0];
    
    double v = exp(slope()*x+amp());
    
    m_param_grad_buf[0] = v;
    m_param_grad_buf[1] = x*v;
    
    return m_param_grad_buf;
  }

  /// quick access to parameter: slope
  double ExponentialModelFunction::slope() const { return parameters()[SLOPE_IDX]; }

  /// quick access to parameter: amp
  double ExponentialModelFunction::amp() const { return parameters()[AMP_IDX]; }

  std::string ExponentialModelFunction::functionTypename()
  {
    return std::string("E");
  }

static Util::Plugin<IFMLFunction,ExponentialModelFunction> plugin(ExponentialModelFunction::functionTypename()); 
  // }
}
