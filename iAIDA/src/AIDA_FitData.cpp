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

// Implementation file for class AIDA_FitData
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 14:53:45 2002
// 
// Last update: Fri Oct  4 14:53:45 2002
// 


#include "AIDA_FitData.h"
#include "AIDA_Dev/IDevFitDataIterator.h"
#include "AIDA_Dev/IDevFitter.h"

#include "AIDA/IHistogram1D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IDataPointSet.h"

#include "AIDA_Histogram1DIterator.h"
#include "AIDA_Histogram2DIterator.h"
#include "AIDA_Histogram3DIterator.h"
#include "AIDA_Profile1DIterator.h"
#include "AIDA_Profile2DIterator.h"
#include "AIDA_Cloud1DIterator.h"
#include "AIDA_Cloud2DIterator.h"
#include "AIDA_Cloud3DIterator.h"
#include "AIDA_TupleIterator.h"
#include "AIDA_DataPointSetIterator.h"
#include "AIDA_Function/AIDA_RangeSet.h"

//# include "AIDA/ITuple.h"
#include "AIDA/IEvaluator.h"

#include "FML/Util/debug.h"


namespace iAIDA  { 
  namespace AIDA_Fitter { 


    AIDA_FitData::AIDA_FitData() 
    {
      // initialize pointer 
      m_data_itr = 0; 
      m_fitType = 0; 
    }

    AIDA_FitData::~AIDA_FitData() 
    {
      // call reset before 
      reset(); 
    }

    AIDA_FitData::AIDA_FitData(const AIDA_FitData &) 
    {
    }

    AIDA_FitData & AIDA_FitData::operator = (const AIDA_FitData &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }


    bool AIDA_FitData::create1DConnection(const AIDA::IHistogram1D & h)
    {
      // reset existing connections before creating new one
      reset();
      
      m_dim = 1;
      m_data_itr =  new AIDA_Histogram1DIterator(h); 

      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;
 
      m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 

      return true; 
    }

    bool AIDA_FitData::create1DConnection(const AIDA::ICloud1D & c)
    { 
      // reset existing connections before creating new one
      reset();
      
      m_dim = 1;
      m_data_itr =  new AIDA_Cloud1DIterator(c); 

      m_fitType = AIDA::Dev::IDevFitter::UNBINNED_FIT;
 
      m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 

      return true; 
    }

    bool AIDA_FitData::create1DConnection(const AIDA::IProfile1D & p)
    { 
      // reset existing connections before creating new one
      reset();
      
      m_dim = 1;
      m_data_itr =  new AIDA_Profile1DIterator(p); 
      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;
 
      m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet());      
      return true; 
    }

    bool AIDA_FitData::create1DConnection(const AIDA::IDataPointSet & dp, int xIndex, int valIndex)
    {
      std::vector<int> indeces(1); 
      indeces[0] = xIndex; 
      return createConnection(dp,indeces,valIndex); 
    }
    
    bool AIDA_FitData::create2DConnection(const AIDA::IHistogram2D & h)
    { 
      return create2DConnection(h,0,1); 
    }
    bool AIDA_FitData::create2DConnection(const AIDA::IHistogram2D & h, int xIndex, int yIndex)
    { 
      // reset existing connections before creating new one
      reset();
      
      m_dim = 2;
      m_data_itr =  new AIDA_Histogram2DIterator(h, xIndex, yIndex); 
      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 

    }
    bool AIDA_FitData::create2DConnection(const AIDA::ICloud2D & c)
    {
      return create2DConnection(c,0,1); 
    }
    bool AIDA_FitData::create2DConnection(const AIDA::ICloud2D & c, int xIndex, int yIndex)
    { 
      reset();
      
      m_dim = 2;
      m_data_itr =  new AIDA_Cloud2DIterator(c, xIndex, yIndex); 
      m_fitType = AIDA::Dev::IDevFitter::UNBINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    bool AIDA_FitData::create2DConnection(const AIDA::IProfile2D & p)
    { 
      return create2DConnection(p,0,1); 
    }
    bool AIDA_FitData::create2DConnection(const AIDA::IProfile2D & p, int xIndex, int yIndex)
    {
      reset();
      
      m_dim = 2;
      m_data_itr =  new AIDA_Profile2DIterator(p, xIndex, yIndex); 
      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    bool AIDA_FitData::create2DConnection(const AIDA::IDataPointSet & dp, int xIndex, int yIndex, int valIndex)
    {
      std::vector<int> indeces(2); 
      indeces[0] = xIndex; 
      indeces[1] = yIndex; 
      return createConnection(dp,indeces,valIndex); 
    }
    
    bool AIDA_FitData::create3DConnection(const AIDA::IHistogram3D & h)
    { 
      return create3DConnection(h,0,1,2); 
    }
    bool AIDA_FitData::create3DConnection(const AIDA::IHistogram3D & h, int xIndex, int yIndex, int zIndex)
    { 
      reset();
      
      m_dim = 3;
      m_data_itr =  new AIDA_Histogram3DIterator(h, xIndex, yIndex, zIndex); 
      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    bool AIDA_FitData::create3DConnection(const AIDA::ICloud3D & c)
    { 
      return create3DConnection(c,0,1,2); 
    }
    bool AIDA_FitData::create3DConnection(const AIDA::ICloud3D & c, int xIndex, int yIndex, int zIndex)
    {
      reset();
      
      m_dim = 3;
      m_data_itr =  new AIDA_Cloud3DIterator(c, xIndex, yIndex, zIndex); 
      m_fitType = AIDA::Dev::IDevFitter::UNBINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    bool AIDA_FitData::create3DConnection(const AIDA::IDataPointSet & dp, int xIndex, int yIndex, int zIndex, int valIndex)
    {  
      std::vector<int> indeces(3); 
      indeces[0] = xIndex; 
      indeces[1] = yIndex; 
      indeces[2] = zIndex; 
      return createConnection(dp,indeces,valIndex); 
    }
      
    
    bool AIDA_FitData::createConnection(const AIDA::ITuple & t, std::vector<std::string>  colNames)
    { 
      reset();
      
      m_dim = colNames.size();
      m_data_itr =  new AIDA_TupleIterator(t, colNames); 
      m_fitType = AIDA::Dev::IDevFitter::UNBINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    bool AIDA_FitData::createConnection(const AIDA::ITuple & t, std::vector<AIDA::IEvaluator * > &  evals)
    { 
      reset();
      
      m_dim = evals.size();

      m_data_itr =  new AIDA_TupleIterator(t, evals); 
      m_fitType = AIDA::Dev::IDevFitter::UNBINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }

    bool AIDA_FitData::createConnection(const AIDA::IDataPointSet & dp, std::vector<int>  indeces, int valIndex)
    { 
      // reset existing connections before creating new one
      reset();
      
      m_dim = indeces.size();
      m_data_itr =  new AIDA_DataPointSetIterator(dp,indeces,valIndex); 
      m_fitType = AIDA::Dev::IDevFitter::BINNED_FIT;

      for (int i = 0; i < m_dim; ++i) 
	m_ranges.push_back(new iAIDA::AIDA_Function::AIDA_RangeSet()); 
     
      return true; 
    }
    
    void AIDA_FitData::reset()
    {  
      // delete ranges
      for (std::vector<iAIDA::AIDA_Function::AIDA_RangeSet *>::iterator i = m_ranges.begin(); i != m_ranges.end(); ++i)
	delete *i; 

      m_ranges.clear(); 

      //delete data iterator 
      if (m_data_itr) delete m_data_itr;  

    }
    
    int AIDA_FitData::dimension() const
    { return m_dim; }
    
    std::string AIDA_FitData::dataDescription() const
    { 
      return ""; 
    }
    
    // return range 
    AIDA::IRangeSet & AIDA_FitData::range(int index) const
    {
      if(index <0 || index >= dimension())
	{
	  INFO_MSG("AIDA_FitData: warning : range index out of bounds ");
	  // here I need to return something - return the first 
	  index = 0; 
	}

      return *m_ranges[index];

    }

    // methods from Developer interface
    
    // return data iterator

    AIDA::Dev::IDevFitDataIterator * AIDA_FitData::dataIterator() const
    {
      if (!m_data_itr) {   
	INFO_MSG("AIDA_FitData is empty - returning a null pointer");
	return 0; 
      } 
      // fill data (here I have to do) 
      // I might to do a check if ranges have changed and is not empty...
      // to avoid to copy the data again.  
      m_data_itr->fillData(m_ranges); 

      return m_data_itr;  
    }

    int AIDA_FitData::fitType() const { return m_fitType; }
    


  }     // end of namespace iAIDA::AIDA_Fitter
} 
