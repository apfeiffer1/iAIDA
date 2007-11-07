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

#ifndef FML_FITTER_H
#define FML_FITTER_H

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
# include <memory>

# include "FML/FitConfig.h"
# include "FML/FitResult.h"

namespace fml
{
 // namespace FML
 // {
   class DataVector;
   class IFMLFunction;
   class FitConfig;
   class FitResult;

   class Fitter
   {
   public:

     // just a convinient shortcut
     std::auto_ptr<FitResult> fit(const DataVector &d, IFMLFunction &f) const;

     // using an abstract iterator
     std::auto_ptr<FitResult> fit(IFMLDataIterator &it, IFMLFunction &f) const;

     std::auto_ptr<DataVector> createScan1D(const DataVector &d, IFMLFunction &f, std::string par, int npts, double pmin, double pmax) const;
     
     std::auto_ptr<DataVector> createContour(const DataVector &d, const FitResult & r, std::string par1, std::string par2, int npts, double up) const;


     FitConfig& fitConfig();
     const FitConfig& fitConfig() const;

   private:
     
     // fit config has minimizer parameter which change during fitting
     mutable FitConfig m_config;
   };
 // }
}


#endif
