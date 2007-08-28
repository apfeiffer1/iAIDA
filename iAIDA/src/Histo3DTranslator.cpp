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

// Implementation file for class Histo3DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "Histo3DTranslator.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA/IAxis.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::Histo3DTranslator::Histo3DTranslator(const AIDA::IHistogram3D * histo, const std::string & name, const std::string & path ) : 
  m_histo(histo), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::Histo3DTranslator::Histo3DTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::Histo3DTranslator::~Histo3DTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::Histo3DTranslator::Histo3DTranslator(const iAIDA::AIDA_XMLStore::Histo3DTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::Histo3DTranslator & iAIDA::AIDA_XMLStore::Histo3DTranslator::operator = (const iAIDA::AIDA_XMLStore::Histo3DTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::Histo3DTranslator::toXML() 
{ 

  // write name and title
  appendObjectHeader(m_element,"histogram3d",m_name,m_histo->title(),m_path); 

  appendAnnotation(m_element,m_histo->annotation()); 
  if (!setAxes() ) return false;
  if (!setStatistics() ) return false;
  if (!setData () ) return false;



  return true;

}

bool iAIDA::AIDA_XMLStore::Histo3DTranslator::setAxes()
{ 
  appendAxis (m_element, "x", m_histo->xAxis());
  appendAxis (m_element, "y", m_histo->yAxis());
  appendAxis (m_element, "z", m_histo->zAxis());
  return true;
}


/// create statistics element containing all stat information 
///   and append the single statistic elements

bool iAIDA::AIDA_XMLStore::Histo3DTranslator::setStatistics()
{ 
  DataXML::DataObject allStatisticsElement;
  allStatisticsElement.setName("statistics");
  allStatisticsElement.setAttribute("entries", toString(m_histo->entries()));

  // append the statistics info
  appendStatistics(allStatisticsElement, "x", m_histo->meanX(), m_histo->rmsX());
  appendStatistics(allStatisticsElement, "y", m_histo->meanY(), m_histo->rmsY());
  appendStatistics(allStatisticsElement, "z", m_histo->meanZ(), m_histo->rmsZ());
  m_element.appendChild(allStatisticsElement);
  return true;
}


bool iAIDA::AIDA_XMLStore::Histo3DTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject dataElement;
  dataElement.setName("data3d");
  const int nbx = (m_histo->xAxis()).bins();
  const int nby = (m_histo->yAxis()).bins();
  const int nbz = (m_histo->zAxis()).bins();
  for (int i = -2; i < nbx; ++i) {
    for (int j = -2; j < nby; ++j ) { 
      for (int k = -2; k < nbz; ++k ) { 
	int    entries = m_histo->binEntries(i,j,k);
	double height  = m_histo->binHeight (i,j,k);
	double error   = m_histo->binError  (i,j,k);
	double binMeanX = m_histo->binMeanX   (i,j,k);
	double binMeanY = m_histo->binMeanY   (i,j,k);
	double binMeanZ = m_histo->binMeanZ   (i,j,k);
	appendHisto3DBinData(dataElement, i, j, k, entries, height, error, binMeanX, binMeanY, binMeanZ);
      }
    }
  }
  m_element.appendChild(dataElement);
  return true;
}

////  reading methods 

AIDA::Dev::IDevHistogram3D * iAIDA::AIDA_XMLStore::Histo3DTranslator::createFromXML(AIDA::Dev::IDevHistogramFactory& factory)
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
  int numberOfBinsZ = 0;
  double lowerEdgeZ = 0;
  double upperEdgeZ = 0;
  std::vector<double> edgesZ; 
  getAxis(m_element,"z",numberOfBinsZ,lowerEdgeZ,upperEdgeZ,edgesZ); 


  // Now I can create the histogram 

  AIDA::Dev::IDevHistogram3D * h = 0; 
  if (edgesX.empty() )  
    // equal bins histograms
    h = factory.createHistogram3D( title, numberOfBinsX, lowerEdgeX, upperEdgeX, numberOfBinsY, lowerEdgeY, upperEdgeY, numberOfBinsZ, lowerEdgeZ, upperEdgeZ );
  else   
    // variable bins histograms 
    h = factory.createHistogram3D( title, edgesX, edgesY, edgesZ, emptyString );

  //read statistics (rms is needed since no binRMs is stored) 
  double rmsX,rmsY,rmsZ = 0; 
  const DataXML::DataObject * statElement = m_element.getChild("statistics");
  if (statElement) { 
    double mean,skew = 0; // not used 
    getStatistics(*statElement,"x",mean,rmsX,skew); 
    getStatistics(*statElement,"y",mean,rmsY,skew); 
    getStatistics(*statElement,"z",mean,rmsZ,skew); 
  }

  // set annotation
  AIDA::IAnnotation & anno = h->annotation(); 
  setAnnotation(&anno,annoData); 

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("data3d");
  if (dataElement) { 
    for (std::vector<DataXML::DataObject>::const_iterator binElement = dataElement->children().begin(); binElement != dataElement->children().end(); ++binElement) {  
      if (binElement->name() == "bin3d") { 
	int xbin,ybin, zbin = -999;    // just a number which is not used 
	int entries = 0;
	// get info (not all is used..) 
	double height, error, wMeanX, wMeanY, wMeanZ, wRmsX, wRmsY, wRmsZ, error2, rms = 0; 
	getHisto3DBinData(*binElement,xbin,ybin,zbin,entries,height,error,wMeanX,wMeanY,wMeanZ,wRmsX,wRmsY,wRmsZ, error2, rms); 
	// fill the histogram with the bin contents 
	// (assume that binMean(centre) is defined) 
	h->setBinContents(xbin,ybin,zbin,entries, height, error, wMeanX, wMeanY,wMeanZ); 
      }
    }
  }
  h->setRms(rmsX,rmsY,rmsZ);

  return h; 
}
