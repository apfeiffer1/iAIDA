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

// Implementation file for class Profile1DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "Profile1DTranslator.h"
#include "AIDA/IProfile1D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA/IAxis.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::Profile1DTranslator::Profile1DTranslator(const AIDA::IProfile1D * histo, const std::string & name, const std::string & path ) : 
  m_histo(histo), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::Profile1DTranslator::Profile1DTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::Profile1DTranslator::~Profile1DTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::Profile1DTranslator::Profile1DTranslator(const iAIDA::AIDA_XMLStore::Profile1DTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::Profile1DTranslator & iAIDA::AIDA_XMLStore::Profile1DTranslator::operator = (const iAIDA::AIDA_XMLStore::Profile1DTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::Profile1DTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"profile1d",m_name,m_histo->title(),m_path); 

  appendAnnotation(m_element,m_histo->annotation()); 

  if (!setAxes() ) return false;
  if (!setStatistics() ) return false;
  if (!setData () ) return false;
  return true;

}

bool iAIDA::AIDA_XMLStore::Profile1DTranslator::setAxes()
{ 
  appendAxis (m_element, "x", m_histo->axis());
  return true;
}


/// create statistics element containing all stat information 
///   and append the single statistic elements

bool iAIDA::AIDA_XMLStore::Profile1DTranslator::setStatistics()
{ 
  DataXML::DataObject allStatisticsElement;
  allStatisticsElement.setName("statistics");
  allStatisticsElement.setAttribute("entries", toString(m_histo->entries()));

  // append the statistics info
  appendStatistics(allStatisticsElement, "x", m_histo->mean(), m_histo->rms());
  m_element.appendChild(allStatisticsElement);
  return true;
}


bool iAIDA::AIDA_XMLStore::Profile1DTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject dataElement;
  dataElement.setName("data1d");
  const int nbx = (m_histo->axis()).bins();
  for (int i = -2; i < nbx; ++i) {
    int    entries = m_histo->binEntries(i);
    double height  = m_histo->binHeight (i);
    double error   = m_histo->binError  (i);
    double binMean = m_histo->binMean   (i);
    double rms     = m_histo->binRms    (i);

    appendHisto1DBinData(dataElement, i, entries, height, error, binMean, rms );
  }
  m_element.appendChild(dataElement);
  return true;
}

////  reading methods 

AIDA::Dev::IDevProfile1D * iAIDA::AIDA_XMLStore::Profile1DTranslator::createFromXML(AIDA::Dev::IDevHistogramFactory& factory)
{ 
  // read header 
  std::string title, options;
  getObjectHeader(m_element,m_name,title,m_path,options); 

  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // read axis
  int numberOfBins = 0;
  double lowerEdge = 0;
  double upperEdge = 0;
  std::vector<double> edges; 
  getAxis(m_element,"x",numberOfBins,lowerEdge,upperEdge,edges); 


  // Now I can create the histogram 

  AIDA::Dev::IDevProfile1D * h = 0; 
  if (edges.empty())  
    // equal bins histograms
    h = factory.createProfile1D( title, numberOfBins, lowerEdge, upperEdge );
  else   
    // variable bins histograms 
    h = factory.createProfile1D( title, edges, emptyString );

  //no need to  get statistics since for AIDA 

  // set annotation
  AIDA::IAnnotation & anno = h->annotation(); 
  setAnnotation(&anno,annoData); 

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("data1d");
  if (!dataElement) return h;
  for (std::vector<DataXML::DataObject>::const_iterator binElement = dataElement->children().begin(); binElement != dataElement->children().end(); ++binElement) {  
    if (binElement->name() == "bin1d") { 
      int xbin = -999;    // just a number which is not used 
      int entries = 0;
      double height, error, wMean, wRms, error2, rms = 0; 
      getHisto1DBinData(*binElement,xbin,entries,height,error,wMean,wRms,error2, rms); 
      // fill the histogram with the bin contents 
      // (assume that binMean(centre) is defined) 
      h->setBinContents(xbin,entries, height, error, rms, wMean); 
    }
  }

  return h; 
}
