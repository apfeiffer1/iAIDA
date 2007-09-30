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

# include <assert.h>

# include "FML/Fitter.h"
# include "FML/FitConfig.h"
# include "FML/FitResult.h"
# include "FML/IFMLDataIterator.h"
# include "FML/DataVector.h"
# include "FML/Minimizer.h"

# include "FML/Util/debug.h"

namespace fml
{
 // namespace FML
 // {

std::auto_ptr<FitResult> Fitter::fit(const DataVector &data, IFMLFunction &f) const
{
  std::auto_ptr<IFMLDataIterator> it = data.createIterator();

  if(!it.get()) return std::auto_ptr<FitResult>();

  return fit(*it.get(),f);
}

std::auto_ptr<FitResult> Fitter::fit(IFMLDataIterator &it, IFMLFunction &f) const
{
  FitResultSetter res;

  if(!res.ok()) return res.freeze();

  // save initial configuration
  if(!res.setConfig(fitConfig(), f, "")) return res.freeze();
  // res.get()->fitConfig().trim(f.parameterNames());

  std::vector<MinimizationVariable> vars(f.numberOfParameters());

  for(int i=0; i<f.numberOfParameters(); ++i)
    {
      const FitParameterSettings &psi = fitConfig().fitParameterSettings(f.parameterNames()[i]);

      vars[i].setValue(f.parameters()[i]);
      vars[i].setStepSize(psi.stepSize()); 
      vars[i].bound() = psi.bound();
    }




  std::auto_ptr<Minimizer> minimizer = fitConfig().createMinimizer();

  // PENDING: possible optimization is to dynamic_cast to DataVectorIterator to
  // PENDING: avoid virtual call overhead, but this needs a modification further down the 
  // PENDING: classes not only here.
  std::auto_ptr<IFMLFunction> fitfun = fitConfig().createFitFunction(f,it);

  if(!minimizer.get() || !fitfun.get()) return res.freeze();


  minimizer->reset(fitfun.get());
  minimizer->setVariables(vars);



  // configure minimizer according to fit configuration
  minimizer->setPrintLevel(fitConfig().printLevel());  
  minimizer->switchMinosErrors(fitConfig().useMinosErrors());
  // control parameter (uses default if users does not change them )   

  MinimizerControlParams & mcp = m_config.minimizerParams();
  //set error according to fitconfig
  mcp.setErrorUP(fitConfig().errorUP()); 

  minimizer->setControlParameters( mcp ); 

  // PENDING: set constraints

  if(minimizer->minimize())
    {

      const MinimizerResult &r = minimizer->result();

      assert(r.isValid());

      const std::vector<ResultVariable>& rv = r.variables();

      std::vector<double> pv(rv.size());
      std::vector<double> ev(rv.size());
      std::vector<double> ePlusv(rv.size());
      std::vector<double> eMinusv(rv.size());

      for(int unsigned i=0; i<rv.size(); ++i) { 
	pv[i] = rv[i].value();
	ev[i] = rv[i].error(); 
	if (rv[i].minosAvailable() ) { 
	  ePlusv[i] = rv[i].minosErrorPlus();  
	  eMinusv[i] = rv[i].minosErrorMinus();  
	} 
	// what to do when errors not avalable ? ( set < 0 ? ) 
	else { 
	  ePlusv[i] = -1; 
	  eMinusv[i] = -1; 
	}  
      }

      // save results

      res.setFlags(true,0);

      int ndf = it.numberOfEntries()-rv.size(); 
      res.setResult(pv, r.minimumValue(), ndf);
      // set errors 
      res.setErrors(ev,ePlusv,eMinusv); 
      // set matrix 
      res.setMatrix(r.covMatrix()); 

      return res.freeze();

    }
  else
    return res.freeze();
}

std::auto_ptr<DataVector> Fitter::createScan1D(const DataVector &d, IFMLFunction &f, std::string par, int npts, double pmin, double pmax) const
{
  /// implement Scan using current  Minimizer engine 
  INFO_MSG("FML: Fitter.createScan1D() not yet implemented");

  // needs design
  /* 
  if(minimizer->scan1D( std::string par, int npts, double pmin, double pmax)

  */ 
  return std::auto_ptr<DataVector>();
}
     
std::auto_ptr<DataVector> Fitter::createContour(const DataVector &d, const FitResult & r, std::string par1, std::string par2, int npts, double up) const
{
  /// FIXME: not yet implemented
  INFO_MSG("FML: Fitter.createContour() not yet implemented");
  return std::auto_ptr<DataVector>();
}


FitConfig& Fitter::fitConfig()
{
  return m_config;
}
     
const FitConfig& Fitter::fitConfig() const
{
  return m_config;
}


// }
}
