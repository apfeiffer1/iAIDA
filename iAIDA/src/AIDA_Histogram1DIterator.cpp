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

// Implementation file for class AIDA_Histogram1DIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:30:41 2002
// 
// Last update: Fri Oct  4 16:30:41 2002
// 

#include "AIDA_Histogram1DIterator.h"
#include "AIDA_Function/AIDA_RangeSet.h"

# include "AIDA/IAxis.h"
# include "AIDA/IHistogram1D.h"

# include "FML/Util/debug.h"


namespace iAIDA  { 
  namespace AIDA_Fitter { 


    AIDA_Histogram1DIterator::AIDA_Histogram1DIterator(const AIDA::IHistogram1D& h) : m_hist(&h), m_dim(h.dimension())
    { 
    }

    AIDA_Histogram1DIterator::~AIDA_Histogram1DIterator() 
    {
    }

    AIDA_Histogram1DIterator::AIDA_Histogram1DIterator(const AIDA_Histogram1DIterator &a) 
      : AIDA_FitDataIterator(a)
    {
    }

    AIDA_Histogram1DIterator & AIDA_Histogram1DIterator::operator = (const AIDA_Histogram1DIterator &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }

    // fill the data in the vector

    void AIDA_Histogram1DIterator::fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges) 
    { 
      int n = ranges.size();
      if (n != m_dim) {
	INFO_MSG(" AIDA_Histogram1DIterator::fillData  invalid dimension for rangeset - cannot feed data in FNL"); 
	return; 
      }
      if (!ranges[0]) { 
	INFO_MSG(" AIDA_Histogram1DIterator::fillData  invalid rangeset - cannot feed data in FNL"); 
	return; 
      }
	
      std::auto_ptr<fml::DataVector> vptr (new fml::DataVector());

      std::vector<double> x(1);
      vptr->reserve(m_hist->axis().bins()); 

      int nPoints = 0; 
      for(int i = 0; i<m_hist->axis().bins(); ++i)
	{
	  //x[0] = m_hist->binMean(i);
	  // take bin centre
	  x[0] = m_hist->axis().binLowerEdge(i) + m_hist->axis().binWidth(i)/2;   
	  if (ranges[0]->isInRange(x[0]) ) { 
	    vptr->push_back(x, m_hist->binHeight(i), m_hist->binError(i) );
	    nPoints++;
	  }
	      
	}
      // set the FML range (size is number of points in range) 
      vptr->setRange(fml::DataRange( nPoints ));

      setVector(vptr);

    }
  }     // end of namespace iAIDA::AIDA_Fitter
} 
