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

// Header file for class AIDA_FitDataIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:44:49 2002
// 
// Last update: Fri Oct  4 16:44:49 2002
// 
#ifndef IAIDA_AIDA_FITTER_AIDA_FITDATAITERATOR
#define IAIDA_AIDA_FITTER_AIDA_FITDATAITERATOR 1

#include "IDevFitDataIteratorFMLBase.h"
#include "FML/DataVector.h"
#include "FML/DataVectorIterator.h"

#include <memory>
#include <vector>

namespace iAIDA { 
   namespace AIDA_Function { 
    class AIDA_RangeSet; 
   }
 }


namespace iAIDA  { 
  namespace AIDA_Fitter { 


    class AIDA_FitDataIterator : virtual public IDevFitDataIteratorFMLBase {

    public: 

      virtual ~AIDA_FitDataIterator(); 


    public: 

      /// methods from IDevFitDataIteratr
      void start();
      bool next();
      int entries() const;
      const std::vector<double>  & vars() const;
      double value() const;
      double error() const;
      double minusError() const;

      /// from IDevFitDataIteratorFMLBase
      fml::IFMLDataIterator & getIFMLDataIterator();


      // fill vector data - must be implemented by derived classes
      virtual void fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges); 

    protected: 
  
      // protect constructor
      AIDA_FitDataIterator(); 
      AIDA_FitDataIterator(const AIDA_FitDataIterator& ) {}
      /// set source data and create internal iterator.
      /// MUST be called by the derived class!
      void setVector(std::auto_ptr<fml::DataVector> vptr);

    private: 

      std::auto_ptr<fml::DataVector> m_vector;
      std::auto_ptr<fml::DataVectorIterator> m_it;

}; 

  }     // end of namespace iAIDA::AIDA_Fitter
}
#endif /* AIDA_FITDATAITERATOR */
