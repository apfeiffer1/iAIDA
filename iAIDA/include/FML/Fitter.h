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
