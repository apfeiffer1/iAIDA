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

// Header file for class AIDA_Cloud2DIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:30:41 2002
// 
// Last update: Fri Oct  4 16:30:41 2002
// 
#ifndef AIDANATIVE_AIDA_FITTER_AIDA_Cloud2DITERATOR
#define AIDANATIVE_AIDA_FITTER_AIDA_Cloud2DITERATOR 1

#include "AIDA_FitDataIterator.h"

namespace AIDA { 
   class ICloud2D; 
 }


namespace iAIDA { 
   namespace AIDA_Function { 
    class AIDA_RangeSet; 
   }
 }

namespace iAIDA  { 
  namespace AIDA_Fitter { 


    class AIDA_Cloud2DIterator : public AIDA_FitDataIterator {

    public: 
      AIDA_Cloud2DIterator(const AIDA::ICloud2D& h, int ix, int iy); 
      virtual ~AIDA_Cloud2DIterator(); 

    private:
      // usually copying is non trivial, so we make this unaccessible
      AIDA_Cloud2DIterator(const AIDA_Cloud2DIterator & h); 
      AIDA_Cloud2DIterator & operator = (const AIDA_Cloud2DIterator &); 
      
    public: 

      // copy data in FML vector
      void fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges); 

    protected: 
      
      
    private: 
      
      const AIDA::ICloud2D * m_hist; 
      // dimension of the data 
      int m_dim;
      int xIndex, yIndex; 
      
    }; 

  }     // end of namespace iAIDA::AIDA_Fitter
 } 
#endif /* AIDA_Cloud2DITERATOR */
