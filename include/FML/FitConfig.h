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

#ifndef FML_FITCONFIG_H
#define FML_FITCONFIG_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <string>
# include <vector>
# include <map>
# include <memory>

# include "FML/FitParameterSettings.h"
# include "FML/DomainConstraints.h"

# include "FML/Minimizer.h"
//# include "FML/IFMLFunction.h"

namespace fml
{
 // namespace FML
 // {
   class IFMLDataIterator;
   class IFMLFunction;
   class Minimizer;
   class DomainConstraints;

   /**
    * FitConfig holds a fitter configuration and is a simple collection
    * of values (may be copied, default constructed etc).
    */
   class FitConfig
   {
   public:
     FitConfig();

     bool setEngine(std::string name);
     std::string engineName() const;

     /// If the name does not exist it is automatically created.
     const FitParameterSettings & fitParameterSettings(std::string name) const;

     /// If the name does not exist it is automatically created.
     FitParameterSettings & fitParameterSettings(std::string name);

     const std::vector<std::string> & listParameterSettings() const;
     void resetParameterSettings();

     void setConstraints(const DomainConstraints& c);
     DomainConstraints& constraints();
     const DomainConstraints& constraints() const;

     void resetConstraints(); 

     /// "" is a synonim for default method (Chi2).
     bool setFitMethod(std::string name);
  
     /// "" is a synonim for default method (Chi2).
     std::string fitMethodName() const;

     /// trim internal list of parameter names to the ones given as argument.
     void trim(const std::vector<std::string>& parameter_names);

     /// create minimizer with current engine.
     std::auto_ptr<Minimizer> createMinimizer() const;

     /// create fittable function using model and data.
     /// model and data are external objects so they must not be deleted
     /// when fit function is still using them.
     std::auto_ptr<IFMLFunction> createFitFunction(IFMLFunction & model, IFMLDataIterator& it) const;
     
     /// set minimizer print level (-1 = no output,0,1,2,3 = most verbose).
     void setPrintLevel(int level); 

     
     /// Switch minos errors computation.
     void switchMinosErrors(bool flag);
     
     /// Set errorUp  - 
     /// Should be 1.0 for Chi2 and 0.5 for negative log likelihood.
     void setErrorUP(double eUp); 

     /// retrieve a non const minimizer control parameters
     MinimizerControlParams & minimizerParams(); 

     /// retrieve minimizer parameters
     const MinimizerControlParams & minimizerParams() const; 

     /// set control parameters
     void setMinimizerParams(const MinimizerControlParams & mcp); 

     /// getters 
     int printLevel() const; 
     bool useMinosErrors() const;  
     double errorUP() const; 


   private:
     std::string m_engine_name, m_method_name;
     int m_printLevel;
     double m_errorUP; 
     bool m_useMinos;
     DomainConstraints m_constr;
	 // make mutable : fitconfig is constant but minimizer param can change 
     mutable MinimizerControlParams m_minimizer_par; 
     typedef std::map<std::string, FitParameterSettings> FitParMap;
     mutable FitParMap m_fitpar;
     mutable std::vector<std::string> m_fitpar_names;
   };
 // }
}

#endif
