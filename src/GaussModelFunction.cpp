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

# include "FML/GaussModelFunction.h"

# include "FML/CodeletStringParser.h" // codelet setting & parsing probably can be automatized in a base class

# include <math.h>

# include "FML/Util/Plugin.h"

namespace fml 
{
  // namespace FML
  // {

    GaussModelFunction::GaussModelFunction() : Function(1,3,true,true,false, GaussModelFunction::functionTypename())
    {
      std::vector<double> p(3);

      p[MEAN_IDX] = 1.0;
      p[SIGMA_IDX] = 1.0;
      p[AMP_IDX] = 1.0;

      Function::setParameters(p);

      constructor_init();
    }

    GaussModelFunction::GaussModelFunction(double mean, double sigma, double amp) : Function(1,3,true,true, false, GaussModelFunction::functionTypename())
    {
      std::vector<double> p(3);
  
      p[MEAN_IDX] = mean;
      p[SIGMA_IDX] = sigma;
      p[AMP_IDX] = amp;

      Function::setParameters(p);

      constructor_init();
    }

    void GaussModelFunction::constructor_init()
    {
      setParName(MEAN_IDX,"mean");
      setParName(SIGMA_IDX,"sigma");
      setParName(AMP_IDX,"amp");

      /*
      CodeletStringParser cp;
      cp.typenameId = GaussModelFunction::functionTypename();
      setCodeletString(cp.write());
      */
    }

const std::vector<double>& GaussModelFunction::gradient(const std::vector<double>& x) const 
{
  m_grad_buf[0] = -value(x)*(x[0]-mean())/(sigma()*sigma());
  return m_grad_buf; 
}

const std::vector<double>& GaussModelFunction::parameterGradient(const std::vector<double>& x) const 
{
  double y = (x[0]-mean())/sigma();

  m_param_grad_buf[AMP_IDX] = exp(-0.5*y*y);
  m_param_grad_buf[MEAN_IDX] = amp()*m_param_grad_buf[AMP_IDX]*y/sigma();
  m_param_grad_buf[SIGMA_IDX] = m_param_grad_buf[MEAN_IDX]*y;

  return m_param_grad_buf;
}

double GaussModelFunction::value(const std::vector<double>& x) const
{
  double y = (x[0]-mean())/sigma();
  return amp()*exp(-0.5*y*y);
}

std::string GaussModelFunction::functionTypename()
{
  return std::string("G");
}

static Util::Plugin<IFMLFunction,GaussModelFunction> plugin(GaussModelFunction::functionTypename()); 

// }
}
