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

#ifndef FITRESULT_H
#define FITRESULT_H

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
# include <memory>

# include "FML/FitConfig.h"

//#include "FML/CLHEP/Matrix/Matrix.h"

#include "FML/FMLMatrix.h"

namespace fml
{
  // namespace FML
  // {
    class FitResult
    {

    public:

      /// True if fit successful, otherwise false.
      bool isValid() const;

      /// Status of the fit. PENDING: what are the suggested values?
      int fitStatus() const;

      /// Function with fitted parameter values.
      /// Undefined behaviour if isValid()==false.
      IFMLFunction &fittedFunction() const;

      /// Function with initial parameter values.
      /// Undefined behaviour if isValid()==false.
      IFMLFunction &initialFunction() const;

      /// Quality of the fit. Depending on the fit method this may be the value
      /// of Chi2 or Maximu Likelihood.
      double quality() const;

      /// Number of degrees of freedom.
      int ndf() const;

      /// Covariance matrix. 
      /// NOTE: interface for matrix to be well defined.
      /// use Minuit convention: upper left sub-matrix is for free parameters
      ///  while rest is filled with zero's
      double covMatrixElement(int i, int j) const;


      /// Parabolic errors 
      const std::vector<double> & errors() const; 

      /// Minos errors  (minus and plus) 
      const std::vector<double> & errorsMinus() const; 
      const std::vector<double> & errorsPlus() const; 

      /// fitted parameters (result)  
      const std::vector<double> & parameters() const; 

      // name of parameters
      const std::vector<std::string> & parameterNames() const; 

      /// Configuration used to create the fit.

      const FitConfig& fitConfig() const { return m_config; }
      
      FitConfig& fitConfig() { return m_config; }

      /// Textual description of data - this is a hint for the user how to find
      /// the data in the tree if he wanted to repeat the fit later and lost
      /// the pointer to his orignal data object.
      /// PENDING: the contents and format of this string to be defined.
      /// PENDING: XML? stor name and path? ranges of DataPipe?
      std::string dataDescription() const;

      /// print the result 
      void printOn(std::ostream&) const;

     
    private:

      /// Limit access to the setters.
      /// The main user: fitter.
      friend class FitResultSetter;

      /// Create invalid result (isValid()==false).
      FitResult();

      /// Setters.
      //@{

      /// Set state and status.
      void setFlags(bool valid, int status);

      /// make a clone of the function, false if clone failed
      bool setConfig(const FitConfig& cfg, IFMLFunction &f, std::string data_descr);
      
      /// set fit result 
      void setResult(const std::vector<double>& fitpars, double quality, int ndf);
		     
      // set errors 
      void setErrors(const std::vector<double>& errors, const std::vector<double>& eMinus, const std::vector<double>& ePlus); 
      // set covariance error  matrix
      void setMatrix(const FMLMatrix& cv); 
     
 
           
      std::auto_ptr<IFMLFunction> m_init_fun, m_fitted_fun;
      
      FitConfig m_config;
      double m_quality;
      int m_ndf;
      bool m_valid;
      int m_status;
      std::string m_data_descr;
      std::vector<double> m_errors; 
      std::vector<double> m_errors_minus; 
      std::vector<double> m_errors_plus; 
      FMLMatrix  m_cov_matrix; 
    };

    /// Give access to set state of fit result. This class is intended for
    /// internal use (fitter).
    /// user must delete  object
    class FitResultSetter
    {
    public:
      FitResultSetter() : m_r(new FitResult()), m_not_own(false) {}

      explicit FitResultSetter(FitResult &r) : m_r(&r), m_not_own(true)
      {
      }

      ~FitResultSetter() 
      { 
	if(m_not_own) 
	  m_r.release(); 
      }

      /// return false if object allocation failed or object is frozen
      bool ok() { return m_r.get(); }

      /// after calling freeze this object may NOT be used.
      std::auto_ptr<FitResult> freeze() 
      {
	std::auto_ptr<FitResult> r(m_r), empty;
	m_r = empty; 
	return r;
      }

      FitResult * get() { return m_r.get(); }

      void setFlags(bool valid, int status) { m_r->setFlags(valid,status); }

      bool setConfig(const FitConfig& cfg, IFMLFunction &f, std::string data_descr) 
      {
	return m_r->setConfig(cfg,f,data_descr);
      }
      
      void setResult(const std::vector<double>& fitpars, double quality, int ndf)
      {
	m_r->setResult(fitpars, quality, ndf);
      }
		     
      void setErrors(const std::vector<double>& errors, const std::vector<double>& eMinus, const std::vector<double>& ePlus) { 
	m_r->setErrors(errors,eMinus,ePlus); 
      }

      void setMatrix(const FMLMatrix & cv)
      {
	m_r->setMatrix(cv);
      } 


    private:
      std::auto_ptr<FitResult>  m_r;
      bool m_not_own;
    };

  // }
}

#endif
