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

// Header file for class AIDA_FitData
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 14:53:45 2002
// 
// Last update: Fri Oct  4 14:53:45 2002
// 
#ifndef AIDANATIVE_AIDA_FITTER_AIDA_FITDATA
#define AIDANATIVE_AIDA_FITTER_AIDA_FITDATA 1

# include "AIDA_Dev/IDevFitData.h"

namespace iAIDA { 
   namespace AIDA_Function { 
    class AIDA_RangeSet; 
   }
 }

namespace iAIDA  { 
  namespace AIDA_Fitter { 

    class AIDA_FitDataIterator; 
    
    class AIDA_FitData : virtual public AIDA::Dev::IDevFitData {
      
    public: 
      AIDA_FitData(); 
      virtual ~AIDA_FitData(); 
      
    private:
      // usually copying is non trivial, so we make this unaccessible
      AIDA_FitData(const AIDA_FitData &); 
      AIDA_FitData & operator = (const AIDA_FitData &); 
      
    public: 
      // from AIDA interfaces  
      //////////////////////////////
      bool create1DConnection(const AIDA::IHistogram1D & h);
      bool create1DConnection(const AIDA::ICloud1D & c);
      bool create1DConnection(const AIDA::IProfile1D & p);
      bool create1DConnection(const AIDA::IDataPointSet & dp, int xIndex, int valIndex);

      bool create2DConnection(const AIDA::IHistogram2D & h);
      bool create2DConnection(const AIDA::IHistogram2D & h, int xIndex, int yIndex);
      bool create2DConnection(const AIDA::ICloud2D & c);
      bool create2DConnection(const AIDA::ICloud2D & c, int xIndex, int yIndex);
      bool create2DConnection(const AIDA::IProfile2D & p);
      bool create2DConnection(const AIDA::IProfile2D & p, int xIndex, int yIndex);
      bool create2DConnection(const AIDA::IDataPointSet & dp, int xIndex, int yIndex, int valIndex);
      
      bool create3DConnection(const AIDA::IHistogram3D & h);
      bool create3DConnection(const AIDA::IHistogram3D & h, int xIndex, int yIndex, int zIndex);
      bool create3DConnection(const AIDA::ICloud3D & c);
      bool create3DConnection(const AIDA::ICloud3D & c, int xIndex, int yIndex, int zIndex);
      bool create3DConnection(const AIDA::IDataPointSet & dp, int xIndex, int yIndex, int zIndex, int valIndex);
      

      bool createConnection(const AIDA::ITuple & t, std::vector<std::string>  colNames);
      bool createConnection(const AIDA::ITuple & t, std::vector<AIDA::IEvaluator * > &  evals);
      bool createConnection(const AIDA::IDataPointSet & dp, std::vector<int>  indeces, int valIndex);

      void reset();
      int dimension() const;
      std::string dataDescription() const;

      AIDA::IRangeSet & range(int index) const; 

      /// from developer interface
      int fitType() const;

      AIDA::Dev::IDevFitDataIterator * dataIterator() const;


protected: 

      //void m_createRange(); 

private: 

  int m_fitType;
  int m_dim;

  AIDA_FitDataIterator*  m_data_itr;
  std::vector<iAIDA::AIDA_Function::AIDA_RangeSet *>  m_ranges;


}; 

  }    // end of namespace iAIDA::AIDA_Fitter
}
#endif /* AIDA_FITDATA */
