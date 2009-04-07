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

// Implementation file for class Cloud2DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "Cloud2DTranslator.h"
#include "AIDA/ICloud2D.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::Cloud2DTranslator::Cloud2DTranslator(const AIDA::ICloud2D * cloud, const std::string & name, const std::string & path ) : 
  m_cloud(cloud), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::Cloud2DTranslator::Cloud2DTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::Cloud2DTranslator::~Cloud2DTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::Cloud2DTranslator::Cloud2DTranslator(const iAIDA::AIDA_XMLStore::Cloud2DTranslator &)   : Translator()
{
}

iAIDA::AIDA_XMLStore::Cloud2DTranslator & iAIDA::AIDA_XMLStore::Cloud2DTranslator::operator = (const iAIDA::AIDA_XMLStore::Cloud2DTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::Cloud2DTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"cloud2d",m_name,m_cloud->title(),m_path); 
  // extra stuff for clouds (maxentries + lower/upper edge
  // for max entries a dev 
  AIDA::Dev::IDevCloud2D* dev_cloud = 
    dynamic_cast< AIDA::Dev::IDevCloud2D* >( const_cast<AIDA::ICloud2D * > (m_cloud ));
  
  if ( !dev_cloud ) return false;     
  m_element.setAttribute("maxEntries", toString(static_cast<int>(dev_cloud->cacheSize() ))); 
  // lower and uppper edges			 			 
  m_element.setAttribute("lowerEdgeX", toString(m_cloud->lowerEdgeX())); 
  m_element.setAttribute("upperEdgeX", toString(m_cloud->upperEdgeX())); 
  m_element.setAttribute("lowerEdgeY", toString(m_cloud->lowerEdgeY())); 
  m_element.setAttribute("upperEdgeY", toString(m_cloud->upperEdgeY())); 
  
  appendAnnotation(m_element,m_cloud->annotation()); 

  if (!setData () ) return false;
  return true;

}


bool iAIDA::AIDA_XMLStore::Cloud2DTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject dataElement;
  dataElement.setName("entries2d");
  const int dim = 2;
  const int n = m_cloud->entries(); 
  for (int i = 0; i < n; ++i) 
    appendCloudEntry(dataElement, dim,
		     m_cloud->valueX(i), m_cloud->valueY(i), 0,m_cloud->weight(i));

  m_element.appendChild(dataElement);
  return true;
}

////  reading methods 

AIDA::Dev::IDevCloud2D * iAIDA::AIDA_XMLStore::Cloud2DTranslator::createFromXML(AIDA::Dev::IDevHistogramFactory& factory)
{ 
  // read header 
  std::string title,options = emptyString;
  getObjectHeader(m_element,m_name,title,m_path,options); 
  int nMax = -1; 
  toValue(m_element.getAttributeValue("maxEntries"), nMax);


  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // Now I can create the cloud 

  AIDA::Dev::IDevCloud2D * cloud  = 
    factory.createCloud2D( title, nMax,options );

  if (!cloud) { 
    std::cerr << " AIDA_XMLStore::Cloud2DTranslator - Cannot create Cloud2D " << std::endl; 
    return 0; 
  }

  //no need to  get statistics since for AIDA 

  // set annotation
  AIDA::IAnnotation & anno = cloud->annotation(); 
  setAnnotation(&anno,annoData); 

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("entries2d");
  if (!dataElement) return cloud; 
  for (std::vector<DataXML::DataObject>::const_iterator entryElement = dataElement->children().begin(); entryElement != dataElement->children().end(); ++entryElement) {  
    if (entryElement->name() == "entry2d") { 
      double xval,yval,zval = 0;    
      double weight = 1; 
      int dim = 2;
      getCloudEntryData(*entryElement,dim,xval,yval,zval,weight);
      // fill the cloud with the contents 
      cloud->fill(xval,yval,weight); 
    }
  }

  return cloud; 
}

