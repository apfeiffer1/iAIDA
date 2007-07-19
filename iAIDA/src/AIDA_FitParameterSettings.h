 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Header file for class AIDA_FitParameterSettings
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:14:29 2002
// 
// Last update: Fri Oct  4 16:14:29 2002
// 
#ifndef AIDANATIVE_AIDA_FITTER_AIDA_FITPARAMETERSETTINGS
#define AIDANATIVE_AIDA_FITTER_AIDA_FITPARAMETERSETTINGS 1

# include "AIDA_Dev/IDevFitParameterSettings.h"
# include "FML/FitParameterSettings.h"

namespace fml { 
     class FitParameterSettings; 
 }
     
namespace iAIDA  { 
  namespace AIDA_Fitter { 

    /**
     * This class may serve as a proxy (adapter) or as a stanalone 
     * implementation. The usage depends on the constructor.
     */

    class AIDA_FitParameterSettings : virtual public AIDA::Dev::IDevFitParameterSettings { 

    public: 
      //AIDA_FitParameterSettings(); 
      /// This creates a proxy to an external FML implementation.
      AIDA_FitParameterSettings(fml::FitParameterSettings* ptr);
      virtual ~AIDA_FitParameterSettings(); 

    private:
      // usually copying is non trivial, so we make this unaccessible
      AIDA_FitParameterSettings(const AIDA_FitParameterSettings &); 
      AIDA_FitParameterSettings & operator = (const AIDA_FitParameterSettings &); 
      
    public: 

      std::string name() const;
      double stepSize() const;
      double upperBound() const;
      double lowerBound() const;
      bool isBound() const;
      bool isFixed() const;
      void setStepSize(double step);
      void setBounds(double lo, double up);
      void removeBounds();
      void setFixed(bool isFixed);
      void setLowerBound(double lowerBound);
      void setUpperBound(double upperBound);
      void reset();


    protected: 


    private: 

      fml::FitParameterSettings *m_settings;      

    }; 

  }    // end of namespace iAIDA::AIDA_Fitter
}
#endif /* AIDA_FITPARAMETERSETTINGS */
