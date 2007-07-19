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

// Implementation file for class Cloud3DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "Cloud3DTranslator.h"
#include "AIDA/ICloud3D.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::Cloud3DTranslator::Cloud3DTranslator(const AIDA::ICloud3D * cloud, const std::string & name, const std::string & path ) : 
  m_cloud(cloud), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::Cloud3DTranslator::Cloud3DTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::Cloud3DTranslator::~Cloud3DTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::Cloud3DTranslator::Cloud3DTranslator(const iAIDA::AIDA_XMLStore::Cloud3DTranslator &)   : Translator()
{
}

iAIDA::AIDA_XMLStore::Cloud3DTranslator & iAIDA::AIDA_XMLStore::Cloud3DTranslator::operator = (const iAIDA::AIDA_XMLStore::Cloud3DTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::Cloud3DTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"cloud3d",m_name,m_cloud->title(),m_path); 
  // extra stuff for clouds (maxentries + lower/upper edge
  // for max entries a dev 
  AIDA::Dev::IDevCloud3D* dev_cloud = 
    dynamic_cast< AIDA::Dev::IDevCloud3D* >( const_cast<AIDA::ICloud3D * > (m_cloud ));
  
  if ( !dev_cloud ) return false;     
  m_element.setAttribute("maxEntries", toString(static_cast<int>(dev_cloud->cacheSize() ))); 
  // lower and uppper edges			 			 
  m_element.setAttribute("lowerEdgeX", toString(m_cloud->lowerEdgeX())); 
  m_element.setAttribute("upperEdgeX", toString(m_cloud->upperEdgeX())); 
  m_element.setAttribute("lowerEdgeY", toString(m_cloud->lowerEdgeY())); 
  m_element.setAttribute("upperEdgeY", toString(m_cloud->upperEdgeY())); 
  m_element.setAttribute("lowerEdgeZ", toString(m_cloud->lowerEdgeZ())); 
  m_element.setAttribute("upperEdgeZ", toString(m_cloud->upperEdgeZ())); 
  

  appendAnnotation(m_element,m_cloud->annotation()); 

  if (!setData () ) return false;
  return true;

}


bool iAIDA::AIDA_XMLStore::Cloud3DTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject dataElement;
  dataElement.setName("entries3d");
  const int dim = 3;
  const int n = m_cloud->entries(); 
  for (int i = 0; i < n; ++i) 
    appendCloudEntry(dataElement, dim,
		     m_cloud->valueX(i), m_cloud->valueY(i), m_cloud->valueZ(i) ,m_cloud->weight(i));

  m_element.appendChild(dataElement);
  return true;
}

////  reading methods 

AIDA::Dev::IDevCloud3D * iAIDA::AIDA_XMLStore::Cloud3DTranslator::createFromXML(AIDA::Dev::IDevHistogramFactory& factory)
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

  AIDA::Dev::IDevCloud3D * cloud  = 
    factory.createCloud3D( title, nMax,options );

  //no need to  get statistics since for AIDA 

  // set annotation
  AIDA::IAnnotation & anno = cloud->annotation(); 
  setAnnotation(&anno,annoData); 

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("entries3d");
  if (!dataElement) return cloud; 
  for (std::vector<DataXML::DataObject>::const_iterator entryElement = dataElement->children().begin(); entryElement != dataElement->children().end(); ++entryElement) {  
    if (entryElement->name() == "entry3d") { 
      double xval,yval,zval = 0;    
      double weight = 1; 
      int dim = 3; 
      getCloudEntryData(*entryElement,dim,xval,yval,zval,weight);
      // fill the cloud with the contents 
      cloud->fill(xval,yval,zval,weight); 
    }
  }

  return cloud; 
}

