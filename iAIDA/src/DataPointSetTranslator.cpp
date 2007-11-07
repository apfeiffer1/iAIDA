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

// Implementation file for class DataPointSetTranslator
// 
// Created by: moneta  at Mon Sep 23 09:54:17 2002
// 
// Last update: Mon Sep 23 09:54:17 2002
// 

#include "DataPointSetTranslator.h"
#include "AIDA/IMeasurement.h"
#include "AIDA/IDataPoint.h"
#include "AIDA/IDataPointSet.h"
#include "AIDA_Dev/IDevDataPointSet.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::DataPointSetTranslator::DataPointSetTranslator(const AIDA::IDataPointSet * dataSet, const std::string & name, const std::string & path ) : 
  m_dataset(dataSet), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::DataPointSetTranslator::DataPointSetTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::DataPointSetTranslator::~DataPointSetTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::DataPointSetTranslator::DataPointSetTranslator(const iAIDA::AIDA_XMLStore::DataPointSetTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::DataPointSetTranslator & iAIDA::AIDA_XMLStore::DataPointSetTranslator::operator = (const iAIDA::AIDA_XMLStore::DataPointSetTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::DataPointSetTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"dataPointSet",m_name,m_dataset->title(),m_path); 
  // for datapointset dimensions are needed...
  m_element.setAttribute("dimension", toString(m_dataset->dimension()));

  appendAnnotation(m_element,m_dataset->annotation()); 
  
  if (!setData () ) return false;
  return true;

}


bool iAIDA::AIDA_XMLStore::DataPointSetTranslator::setData()
{ 
  // loop on all data point in the set 
  for (int i = 0; i < m_dataset->size(); ++i) { 
    // write data element and append to the dataobject
    DataXML::DataObject dataElement;
    dataElement.setName("dataPoint");
    // get the point 
    const AIDA::IDataPoint * point = m_dataset->point(i);     
        
    for (int j = 0; j <  point->dimension(); ++j) {
      const AIDA::IMeasurement* m = point->coordinate( j );
      // write data element and app; ++i) 
      appendDataPointMeasurement(dataElement,m->value(),
				 m->errorPlus(),m->errorMinus() );  
    }
    m_element.appendChild(dataElement);
  }
  return true;
}

////  reading methods 

AIDA::Dev::IDevDataPointSet * iAIDA::AIDA_XMLStore::DataPointSetTranslator::createFromXML(AIDA::Dev::IDevDataPointSetFactory& factory)
{ 
  // read header 
  std::string title, options;
  getObjectHeader(m_element,m_name,title,m_path,options); 
  // get dimensions of the points 
  int dimension = 0; 
  toValue(m_element.getAttributeValue("dimension"), dimension);

  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // Now I can create the data point set  

  AIDA::Dev::IDevDataPointSet * dpSet  = 0; 

  if (title.empty())
    dpSet = factory.create( dimension );
  else
    dpSet = factory.create( title, dimension );

  // set annotation
  AIDA::IAnnotation & anno = dpSet->annotation(); 
  setAnnotation(&anno,annoData); 
 
  // loop on the data points in the set   
  int iPoint = 0; 
  for (std::vector<DataXML::DataObject>::const_iterator dpElement = m_element.children().begin(); dpElement != m_element.children().end(); ++dpElement) { 
    if (dpElement->name() == "dataPoint") { 
      // add point to the set 
      dpSet->addPoint(); 
      AIDA::IDataPoint* point = dpSet->point( iPoint );
      iPoint++;
      // loop on measurements
      int dim = 0; 
      // check if dimension in file is not different than what is written
      int n = dpElement->children().size(); 
      if (n != dpSet->dimension() ) { 
	std::cerr << "DataPointSetTranslator:: Invalid dataPoint dimension in XML file " << std::endl;  
	return 0; 
      }
      for (std::vector<DataXML::DataObject>::const_iterator m = dpElement->children().begin(); m != dpElement->children().end(); ++m) { 
	if (m->name() == "measurement") { 
	  if (dim >= dpSet->dimension() ) return 0; 
	  AIDA::IMeasurement* measurement = point->coordinate( dim );
	  double value,eplus,eminus = 0;  
	  getDataPointMeasurement(*m,value,eplus,eminus);
	  // set new values 
	  measurement->setValue(value); 
	  measurement->setErrorPlus(eplus); 
	  measurement->setErrorMinus(eminus); 
	  dim++; 
	}	  
      }
    }
  }
  return dpSet; 
}

