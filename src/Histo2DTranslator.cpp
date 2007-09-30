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

// Implementation file for class Histo2DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "Histo2DTranslator.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA/IAxis.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::Histo2DTranslator::Histo2DTranslator(const AIDA::IHistogram2D * histo, const std::string & name, const std::string & path ) : 
  m_histo(histo), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::Histo2DTranslator::Histo2DTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::Histo2DTranslator::~Histo2DTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::Histo2DTranslator::Histo2DTranslator(const iAIDA::AIDA_XMLStore::Histo2DTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::Histo2DTranslator & iAIDA::AIDA_XMLStore::Histo2DTranslator::operator = (const iAIDA::AIDA_XMLStore::Histo2DTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::Histo2DTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"histogram2d",m_name,m_histo->title(),m_path); 

  appendAnnotation(m_element,m_histo->annotation()); 
  if (!setAxes() ) return false;
  if (!setStatistics() ) return false;
  if (!setData () ) return false;
  return true;

}

bool iAIDA::AIDA_XMLStore::Histo2DTranslator::setAxes()
{ 
  appendAxis (m_element, "x", m_histo->xAxis());
  appendAxis (m_element, "y", m_histo->yAxis());
  return true;
}


/// create statistics element containing all stat information 
///   and append the single statistic elements

bool iAIDA::AIDA_XMLStore::Histo2DTranslator::setStatistics()
{ 
  DataXML::DataObject allStatisticsElement;
  allStatisticsElement.setName("statistics");
  allStatisticsElement.setAttribute("entries", toString(m_histo->entries()));

  // append the statistics info
  appendStatistics(allStatisticsElement, "x", m_histo->meanX(), m_histo->rmsX());
  appendStatistics(allStatisticsElement, "y", m_histo->meanY(), m_histo->rmsY());
  m_element.appendChild(allStatisticsElement);
  return true;
}


bool iAIDA::AIDA_XMLStore::Histo2DTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject dataElement;
  dataElement.setName("data2d");
  const int nbx = (m_histo->xAxis()).bins();
  const int nby = (m_histo->yAxis()).bins();
  for (int i = -2; i < nbx; ++i) {
    for (int j = -2; j < nby; ++j ) { 
      int    entries = m_histo->binEntries(i,j);
      double height  = m_histo->binHeight (i,j);
      double error   = m_histo->binError  (i,j);
      double binMeanX = m_histo->binMeanX   (i,j);
      double binMeanY = m_histo->binMeanY   (i,j);
      appendHisto2DBinData(dataElement, i, j, entries, height, error, binMeanX, binMeanY);
    }
  }
  m_element.appendChild(dataElement);
  return true;
}

////  reading methods 

AIDA::Dev::IDevHistogram2D * iAIDA::AIDA_XMLStore::Histo2DTranslator::createFromXML(AIDA::Dev::IDevHistogramFactory& factory)
{ 
  // read header 
  std::string title, options;
  getObjectHeader(m_element,m_name,title,m_path,options); 

  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // read axis
  int numberOfBinsX = 0;
  double lowerEdgeX = 0;
  double upperEdgeX = 0;
  std::vector<double> edgesX; 
  getAxis(m_element,"x",numberOfBinsX,lowerEdgeX,upperEdgeX,edgesX); 
  int numberOfBinsY = 0;
  double lowerEdgeY = 0;
  double upperEdgeY = 0;
  std::vector<double> edgesY; 
  getAxis(m_element,"y",numberOfBinsY,lowerEdgeY,upperEdgeY,edgesY); 


  // Now I can create the histogram 

  AIDA::Dev::IDevHistogram2D * h = 0; 
  if (edgesX.empty() )  
    // equal bins histograms
    h = factory.createHistogram2D( title, numberOfBinsX, lowerEdgeX, upperEdgeX, numberOfBinsY, lowerEdgeY, upperEdgeY );
  else   
    // variable bins histograms 
    h = factory.createHistogram2D( title, edgesX, edgesY, emptyString );

  //read statistics (rms is needed since no binRMs is stored) 
  double rmsX,rmsY = 0; 
  const DataXML::DataObject * statElement = m_element.getChild("statistics");
  if (statElement) { 
    double mean,skew = 0; // not used 
    getStatistics(*statElement,"x",mean,rmsX,skew); 
    getStatistics(*statElement,"y",mean,rmsY,skew); 
  }

  // set annotation
  AIDA::IAnnotation & anno = h->annotation(); 
  setAnnotation(&anno,annoData); 

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("data2d");
  if (dataElement) { 
    for (std::vector<DataXML::DataObject>::const_iterator binElement = dataElement->children().begin(); binElement != dataElement->children().end(); ++binElement) {  
       if (binElement->name() == "bin2d") { 
	 int xbin,ybin = -999;    // just a number which is not used 
	 int entries = 0;
	 // get info (not all is used..) 
	 double height, error, wMeanX, wMeanY, wRmsX, wRmsY, error2, rms = 0; 
	 getHisto2DBinData(*binElement,xbin,ybin,entries,height,error,wMeanX,wMeanY,wRmsX,wRmsY,error2, rms); 
	 // fill the histogram with the bin contents 
	 // (assume that binMean(centre) is defined) 
	 h->setBinContents(xbin,ybin,entries, height, error, wMeanX, wMeanY); 
       }
    }
  }
  h->setRms(rmsX,rmsY);

  return h; 
}
