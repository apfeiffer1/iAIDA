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

// Header file for class AIDA_Cloud3DIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:30:41 2002
// 
// Last update: Fri Oct  4 16:30:41 2002
// 
#ifndef IAIDA_AIDA_FITTER_AIDA_Cloud3DITERATOR
#define IAIDA_AIDA_FITTER_AIDA_Cloud3DITERATOR 1

#include "AIDA_FitDataIterator.h"

namespace AIDA { 
   class ICloud3D; 
 }


namespace iAIDA { 
   namespace AIDA_Function { 
    class AIDA_RangeSet; 
   }
 }

namespace iAIDA  { 
  namespace AIDA_Fitter { 


    class AIDA_Cloud3DIterator : public AIDA_FitDataIterator {

    public: 
      AIDA_Cloud3DIterator(const AIDA::ICloud3D& h,  int ix, int iy, int iz ); 
      virtual ~AIDA_Cloud3DIterator(); 

    private:
      // usually copying is non trivial, so we make this unaccessible
      AIDA_Cloud3DIterator(const AIDA_Cloud3DIterator & h); 
      AIDA_Cloud3DIterator & operator = (const AIDA_Cloud3DIterator &); 
      
    public: 

      // copy data in FML vector
      void fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges ); 

    protected: 
      
      
    private: 
      
      const AIDA::ICloud3D * m_hist; 
      // dimension of the data 
      int m_dim;
      int xIndex, yIndex, zIndex; 
      
    }; 

  }     // end of namespace iAIDA::AIDA_Fitter
 } 
#endif /* AIDA_Cloud3DITERATOR */
