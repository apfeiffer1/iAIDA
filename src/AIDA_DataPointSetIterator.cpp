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

// Implementation file for class AIDA_DataPointSetIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:30:41 2002
// 
// Last update: Fri Oct  4 16:30:41 2002
// 

#include "AIDA_DataPointSetIterator.h"
#include "AIDA_Function/AIDA_RangeSet.h"

# include "AIDA/IAxis.h"
# include "AIDA/IDataPointSet.h"
# include "AIDA/IDataPoint.h"
# include "AIDA/IMeasurement.h"

# include "FML/Util/debug.h"

#include <assert.h>



namespace iAIDA  { 
  namespace AIDA_Fitter { 


    AIDA_DataPointSetIterator::AIDA_DataPointSetIterator(const AIDA::IDataPointSet& dps, std::vector<int>  indeces, int valIndex) : 
      m_data(&dps) , 
      m_indeces(indeces), 
      m_val_index(valIndex)
    { 
      m_dim = indeces.size();
      assert (m_dim < m_data->dimension() ); 
    }

    AIDA_DataPointSetIterator::~AIDA_DataPointSetIterator() 
    {
    }

    AIDA_DataPointSetIterator::AIDA_DataPointSetIterator(const AIDA_DataPointSetIterator &a) 
      : AIDA_FitDataIterator(a)
    {
    }

    AIDA_DataPointSetIterator & AIDA_DataPointSetIterator::operator = (const AIDA_DataPointSetIterator &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }

    // fill the data in the vector

    void AIDA_DataPointSetIterator::fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges) 
    { 
      int n = ranges.size();
      if (n != m_dim) {
	INFO_MSG(" AIDA_DataPointSetIterator::fillData  invalid dimension for rangeset - cannot feed data in FNL"); 
	return; 
      }
      // check calidity of range pointers  
      for (int i = 0; i < m_dim; ++i) { 
	if (!ranges[i]) { 
	  INFO_MSG(" AIDA_DataPointSetIterator::fillData  invalid rangeset - cannot feed data in FNL"); 
	  return; 
	}
      }
	
      std::auto_ptr<fml::DataVector> vptr (new fml::DataVector());

      std::vector<double> x(m_dim);

	  
      // loop on points in the set 
      int nPointsInRange = 0; 
      for(int ipoint = 0; ipoint < m_data->size(); ++ipoint)
	{
	  const AIDA::IDataPoint* p = m_data->point( ipoint ); 
	  // loop on dimension to get coordinates 
	  bool validPoint = true; 
	  for (int idim = 0; idim < m_dim; ++idim) 
	    {
	      const AIDA::IMeasurement* m = p->coordinate( m_indeces[idim] );
	      x[idim] = m->value(); 
	      validPoint = validPoint && ranges[idim]->isInRange(x[idim]);
	    }
	  // if range is OK for all points ...
	  if (validPoint) { 
	    // now get values to be fitter and errors 
	    const AIDA::IMeasurement* m = p->coordinate(m_val_index); 
	    double value = m->value(); 
	    double eplus = m->errorPlus(); 
	    double eminus = m->errorMinus(); 
	    double error = (eplus + eminus)/2.;
	    
	    vptr->push_back(x, value, error);
	    nPointsInRange++;
	  }
	      
	}

      // set the FML range (size is number of points in range) 
      vptr->setRange(fml::DataRange( nPointsInRange ));

      setVector(vptr);

    }
  }     // end of namespace iAIDA::AIDA_Fitter
} 
