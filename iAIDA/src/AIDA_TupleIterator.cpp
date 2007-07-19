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

// Implementation file for class AIDA_TupleIterator
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 16:30:41 2002
// 
// Last update: Fri Oct  4 16:30:41 2002
// 

#include "AIDA_TupleIterator.h"
#include "AIDA_Function/AIDA_RangeSet.h"

# include "AIDA/ITuple.h"
# include "AIDA/IEvaluator.h"

# include "FML/Util/debug.h"


namespace iAIDA  { 
  namespace AIDA_Fitter { 


    AIDA_TupleIterator::AIDA_TupleIterator(const AIDA::ITuple& t, std::vector<std::string> & colNames) : 
      m_cols(colNames)
    { 
      m_tuple = const_cast<AIDA::ITuple * > (&t); 
      m_dim = colNames.size(); 
    }

    AIDA_TupleIterator::AIDA_TupleIterator(const AIDA::ITuple& t, std::vector<AIDA::IEvaluator * > &  evaluators)  
    { 
      m_tuple = const_cast<AIDA::ITuple * > (&t); 
      m_dim = evaluators.size();       
      m_evals = evaluators; 
    }

    AIDA_TupleIterator::~AIDA_TupleIterator() 
    {
    }

    AIDA_TupleIterator::AIDA_TupleIterator(const AIDA_TupleIterator &a) 
      : AIDA_FitDataIterator(a)
    {
    }

    AIDA_TupleIterator & AIDA_TupleIterator::operator = (const AIDA_TupleIterator &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }

    // fill the data in the vector

    void AIDA_TupleIterator::fillData(const std::vector<iAIDA::AIDA_Function::AIDA_RangeSet * > & ranges) 
    { 
      int n = ranges.size();
      if (n != m_dim) {
	INFO_MSG(" AIDA_TupleIterator::fillData  invalid dimension for rangeset - cannot feed data in FNL"); 
	return; 
      }
      if (!ranges[0]) { 
	INFO_MSG(" AIDA_TupleIterator::fillData  invalid rangeset - cannot feed data in FNL"); 
	return; 
      }
	
      std::auto_ptr<fml::DataVector> vptr (new fml::DataVector());

      std::vector<double> x(m_dim);

      int nCols = m_cols.size();
      int nEvals = m_evals.size(); 

      int nPoints = 0; 
      m_tuple->start(); 
      while (m_tuple->next() ) { 
	bool validPoint = false; 

	// case of columns 
	if (nCols == m_dim ) { 
	  validPoint = true; 
	  for(int i = 0; i<m_dim; ++i)
	    { 
	      int icol = m_tuple->findColumn(m_cols[i]); 
	      std::string type = m_tuple->columnType(icol); 
	      // use only float or double 
	      if (type == "float" )   x[i] = m_tuple->getFloat(icol);  
	      if (type == "double" )  x[i] = m_tuple->getDouble(icol);  

	      validPoint = validPoint && ranges[i]->isInRange(x[i]);
	    }
	}
	// case of evaluators 
	if (nEvals == m_dim) { 
	  validPoint = true; 
	  for(int i = 0; i<m_dim; ++i)
	    { 
	      x[i] = m_evals[i]->evaluateDouble(); 

	      validPoint = validPoint && ranges[i]->isInRange(x[i]);
	    }	    
	}
	
	if (validPoint) {       
	  vptr->push_back(x );
	  nPoints++; 
	}

      }
      // set the range (size is number of pointss !) - needed by FML
      vptr->setRange(fml::DataRange(nPoints) );

      setVector(vptr);

    }

  }     // end of namespace iAIDA::AIDA_Fitter
} 
