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
  * Le ser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Implementation file for class Translator
// 
// Created by: moneta  at Fri Sep 13 10:43:55 2002
// 
// Last update: Fri Sep 13 10:43:55 2002
// 

#include "Translator.h"

#include "AIDA/IAxis.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IConstants.h"
#include "DataXML/DataObject.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

#include <algorithm>

namespace iAIDA {

  namespace AIDA_XMLStore { 

//////////////////////////////////
//                              //
// static members of Translator //
//                              //
//////////////////////////////////

    const int     Translator::SCI_PRECISION          = 8;
    const std::string Translator::XML_VERSION        = "1.0";
    const std::string Translator::ENCODING_TYPE      = "ISO-8859-1";
    const std::string Translator::PACKAGE            = "pi";
    const std::string Translator::PACKAGE_VERSION    = "1";   // set by hand 

  }
}


iAIDA::AIDA_XMLStore::Translator::Translator() 
{
}

iAIDA::AIDA_XMLStore::Translator::~Translator() 
{
}

iAIDA::AIDA_XMLStore::Translator::Translator(const iAIDA::AIDA_XMLStore::Translator &) 
{
}

iAIDA::AIDA_XMLStore::Translator & iAIDA::AIDA_XMLStore::Translator::operator = (const iAIDA::AIDA_XMLStore::Translator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

/// write top header information (aida element and implementation element) 

bool iAIDA::AIDA_XMLStore::Translator::appendAidaHeader(DataXML::DataObject & aidaElement) 
{ 
  aidaElement.setName("aida"); 
  aidaElement.setAttribute("version",AIDA_VERSION);
  DataXML::DataObject implementationElement; 
  implementationElement.setName("implementation");
  implementationElement.setAttribute("package",iAIDA::AIDA_XMLStore::Translator::PACKAGE);
  implementationElement.setAttribute("version",iAIDA::AIDA_XMLStore::Translator::PACKAGE_VERSION);
  aidaElement.appendChild(implementationElement);   
  return true; 
}

/// write Header information of a general Object (Histo,Cloud, etc..) (type name path) 

bool iAIDA::AIDA_XMLStore::Translator::appendObjectHeader(DataXML::DataObject & objElement, const std::string & objType, const std::string & name, const std::string & title, const std::string & path, const std::string & options)
{ 
  // type identifies the object (e.g. histogram1d..)
  objElement.setName(objType);
  objElement.setAttribute("name", name);
  // optional arguments 
  if (!title.empty())
    objElement.setAttribute("title", title);
  if (!path.empty())
    objElement.setAttribute("path", path);
  if (!options.empty())
    objElement.setAttribute("options", options);
  return true;  
}

/// create and append annotation element 

bool iAIDA::AIDA_XMLStore::Translator::appendAnnotation(DataXML::DataObject & objElement, const AIDA::IAnnotation& anno) 
{ 
  // add annotation child
  DataXML::DataObject annoElement;
  annoElement.setName("annotation");
  for (int i = 0; i < anno.size() ; ++i) {  
    DataXML::DataObject itemElement;
    itemElement.setName("item");     
    itemElement.setAttribute("key", anno.key(i));
    itemElement.setAttribute("value", anno.value(i));
    // retieve stckness from dev level 
    // to do.....
    annoElement.appendChild(itemElement); 
  }
  objElement.appendChild(annoElement);
  return true;
}

/// create and append axis Element

bool iAIDA::AIDA_XMLStore::Translator::appendAxis(DataXML::DataObject & objElement, const std::string& axisName, const AIDA::IAxis& axis) 
{ 

    // add axis child 
  DataXML::DataObject axisElement;
  axisElement.setName("axis");
  axisElement.setAttribute("direction", axisName);
  axisElement.setAttribute("min",          toString(axis.lowerEdge()));
  axisElement.setAttribute("max",          toString(axis.upperEdge()));
  axisElement.setAttribute("numberOfBins", toString(axis.bins()));

  // for variable Bins axis
  if (!axis.isFixedBinning() ) {
    DataXML::DataObject borderElement;
    borderElement.setName("binBorder");
    const int nEdges = axis.bins() - 1;
    for (int i = 0; i < nEdges; ++i) {
      borderElement.setAttribute("value", toString(axis.binUpperEdge(i)));
      axisElement.appendChild(borderElement);
    }
  }
  objElement.appendChild(axisElement);
  return true;
}

/// append a statistic child to the statistics element (there is one per axis)

bool iAIDA::AIDA_XMLStore::Translator::appendStatistics(DataXML::DataObject & statisticsElement, const std::string& axisName, 
			double mean, double rms, double skew)
{

  DataXML::DataObject statElement;
  statElement.setName("statistic");
  statElement.setAttribute("direction", axisName);
  statElement.setAttribute("mean",toString(mean));
  statElement.setAttribute("rms", toString(rms));
  if (skew != 0) statElement.setAttribute("skewness", toString(skew));
  statisticsElement.appendChild(statElement);
  return true;
  
}

/// append  single bin element data to the histogram data element (1D)

bool iAIDA::AIDA_XMLStore::Translator::appendHisto1DBinData(DataXML::DataObject & dataElement, int xbin, int entries, double height, double error, double wMean, double rms, double wRms, double error2)

{
  // skip bins with sero entries
  if (entries == 0) return false; 
  DataXML::DataObject binElement;
  binElement.setName(std::string("bin1d"));
  binElement.setAttribute("binNum",binNumToString(xbin));
  binElement.setAttribute("height",  toString(height) );
  binElement.setAttribute("error",   toString(error)  );
  binElement.setAttribute("entries", toString(entries));
  if (wMean != 0) 
    binElement.setAttribute("weightedMean", toString(wMean));
  if (rms != 0) 
    binElement.setAttribute("rms", toString(rms));
  if (wRms != 0) 
    binElement.setAttribute("weightedRms", toString(wRms));
  if (error2 != 0) 
    binElement.setAttribute("error2", toString(error2));

  dataElement.appendChild(binElement);  
  return true;
}


/// append 2d bin 

bool iAIDA::AIDA_XMLStore::Translator::appendHisto2DBinData(DataXML::DataObject & dataElement, int xbin, int ybin, int entries, double height, double error, double wMeanX, double wMeanY, double rms)

{

  // skip bins with sero entries
  if (entries == 0) return false; 
  DataXML::DataObject binElement;
  binElement.setName(std::string("bin2d") );
  binElement.setAttribute("binNumX",binNumToString(xbin));
  binElement.setAttribute("binNumY",binNumToString(ybin));
  binElement.setAttribute("height",  toString(height) );
  binElement.setAttribute("error",   toString(error)  );
  binElement.setAttribute("entries", toString(entries));
  if (wMeanX != 0 || wMeanY != 0 ) {
      binElement.setAttribute("weightedMeanX", toString(wMeanX));
      binElement.setAttribute("weightedMeanY", toString(wMeanY));
  }
  if (rms != 0) 
    binElement.setAttribute("rms", toString(rms));

  dataElement.appendChild(binElement);  
  return true;
}

/// append 3d bin 

bool iAIDA::AIDA_XMLStore::Translator::appendHisto3DBinData(DataXML::DataObject & dataElement, int xbin, int ybin, int zbin, int entries, double height, double error, double wMeanX, double wMeanY, double wMeanZ, double rms)

{

  // skip bins with sero entries
  if (entries == 0) return false; 
  DataXML::DataObject binElement;
  binElement.setName(std::string("bin3d") );
  binElement.setAttribute("binNumX",binNumToString(xbin));
  binElement.setAttribute("binNumY",binNumToString(ybin));
  binElement.setAttribute("binNumZ",binNumToString(zbin));
  binElement.setAttribute("height",  toString(height) );
  binElement.setAttribute("error",   toString(error)  );
  binElement.setAttribute("entries", toString(entries));
  if (wMeanX != 0 || wMeanY != 0  || wMeanZ != 0 ) {
      binElement.setAttribute("weightedMeanX", toString(wMeanX));
      binElement.setAttribute("weightedMeanY", toString(wMeanY));
      binElement.setAttribute("weightedMeanZ", toString(wMeanZ));
  }
  if (rms != 0) 
    binElement.setAttribute("rms", toString(rms));

  dataElement.appendChild(binElement);  
  return true;
}
  
bool iAIDA::AIDA_XMLStore::Translator::appendCloudEntry(DataXML::DataObject & dataElement, int dim, double xval, double yval, double zval, double weight ) 
{ 

  DataXML::DataObject entryElement;
  entryElement.setName(std::string("entry")+toString(dim)+std::string("d"));

  if (dim >= 1) entryElement.setAttribute("valueX", toString(xval));
  if (dim >= 2) entryElement.setAttribute("valueY", toString(yval));
  if (dim >= 3) entryElement.setAttribute("valueZ", toString(zval));
  if (weight != 1.0) 
    entryElement.setAttribute("weight",  toString(weight) );

  dataElement.appendChild(entryElement);  
  return true;
}

bool iAIDA::AIDA_XMLStore::Translator::appendDataPointMeasurement(DataXML::DataObject & dataElement,  double value, double eminus, double eplus) 
{ 

  DataXML::DataObject measurementElement;
  measurementElement.setName("measurement");
  measurementElement.setAttribute("value",  toString(value) );
  if (eplus != 0)
    measurementElement.setAttribute("errorPlus",  toString(eplus) );
  if (eminus != 0)
    measurementElement.setAttribute("errorMinus",  toString(eminus) );

  dataElement.appendChild(measurementElement);  

  return true;
}

/// reading helper methods 

void iAIDA::AIDA_XMLStore::Translator::getObjectHeader(const DataXML::DataObject & obj, std::string & name, std::string & title, std::string & path, std::string & options) 
{ 
  name = obj.getAttributeValue("name"); 
  title = obj.getAttributeValue("title");
  // try for label in case title does not exist 
  if (title.empty()) 
    title = obj.getAttributeValue("label");
    
  path = obj.getAttributeValue("path");
  options = obj.getAttributeValue("options");
}

/// get Annotation
void iAIDA::AIDA_XMLStore::Translator::getAnnotation(const DataXML::DataObject & obj,AnnotationData & annoData) { 
  const DataXML::DataObject * anno_obj = obj.getChild("annotation"); 
  if (!anno_obj) return;   
  for (ElementIterator i = anno_obj->children().begin(); i != anno_obj->children().end(); ++i) {
    if (i->name() == "item") { 
      bool stickness = (i->getAttributeValue("sticky") == "true"); 
      annoData.push_back(std::make_pair(i->getAttributeValue("key"),std::make_pair(i->getAttributeValue("value"),stickness) ) );
    }
  }
}
// set Annotation on histograms

    void iAIDA::AIDA_XMLStore::Translator::setAnnotation(AIDA::IAnnotation * anno, const AnnotationData  & annoData) { 
      int nItem = annoData.size(); 
      for (int i = 0; i < nItem; ++i ) {
	bool ret = anno->addItem(annoData[i].first,(annoData[i].second).first,(annoData[i].second).second); 
	if (!ret) { // item exists just set them 
	  anno->setValue(annoData[i].first, (annoData[i].second).first ); 
	  anno->setSticky(annoData[i].first,(annoData[i].second).second );
	}
      }
    }

/// get Axis contents for Histograms

void iAIDA::AIDA_XMLStore::Translator::getAxis(const DataXML::DataObject & obj, const std::string & axisName, int& nBins, double& low, double& high, std::vector<double> & edges) 
{

  // extract the parameters of the histogram axis, including bin edges if necessary
  for (ElementIterator i = obj.children().begin(); i != obj.children().end(); ++i) {

    if (i->name() == "axis" && i->getAttributeValue("direction") == axisName) {
      // get common parameters
      toValue(i->getAttributeValue("min"),          low);
      toValue(i->getAttributeValue("max"),          high);
      toValue(i->getAttributeValue("numberOfBins"), nBins);

      // and then get variable bin edges (if available)

      edges.clear(); 
      for (ElementIterator jBin = i->children().begin(); jBin != i->children().end(); ++jBin) { 
	if (jBin->name() == "binBorder") {
	  double binEdge = 0.0;
	  if  (toValue(jBin->getAttributeValue("value"), binEdge)) 
	    edges.push_back(binEdge); 
	}
      }
      // add min and max of edges 
      if (!edges.empty()) { 
	edges.push_back(low); 
	edges.push_back(high);
      }

      // check if edges vector has the right size otherwise used equal bins (empty vector) 
      if ( nBins != static_cast<int>(edges.size())-1)
	edges.clear(); 
      // if OK sort vector in correct order of the edges 
      else 
	std::sort(edges.begin(), edges.end() ); 

    }
  }
}

/// get statistic content of histogrsam

void iAIDA::AIDA_XMLStore::Translator::getStatistics(const DataXML::DataObject & statisticsElement, const std::string & axisName, double & mean, double & rms, double & skew) 
{ 
  for (ElementIterator i = statisticsElement.children().begin();
       i != statisticsElement.children().end(); ++i) {
    if (i->name() == "statistic" && i->getAttributeValue("direction") == axisName) {
      toValue(i->getAttributeValue("mean"),     mean);
      toValue(i->getAttributeValue("rms"),      rms);
      toValue(i->getAttributeValue("skewness"), skew);
      return;
    }
  }
  //  missingElementError("statistic ("+axisName+" axis)");
}

/// get Histogram single bin data 

void iAIDA::AIDA_XMLStore::Translator::getHisto1DBinData(const DataXML::DataObject &  binElement, int & xbin, int & entries, double & height, double & error, double & wMean, double & wRms, double & error2, double & rms)
{ 

  xbin = stringToBinNum(binElement.getAttributeValue("binNum"));

  toValue(binElement.getAttributeValue("entries"),entries);
  toValue(binElement.getAttributeValue("height"),height);
  toValue(binElement.getAttributeValue("error"),error);
  toValue(binElement.getAttributeValue("weightedMean"),wMean);
  toValue(binElement.getAttributeValue("weightedRms"),wRms);
  toValue(binElement.getAttributeValue("error2"),error2);
  toValue(binElement.getAttributeValue("rms"),rms);

}


void iAIDA::AIDA_XMLStore::Translator::getHisto2DBinData(const DataXML::DataObject &  binElement, int & xbin, int & ybin, int & entries, double & height, double & error, double & wMeanX, double & wMeanY, double & wRmsX, double & wRmsY, double & error2, double & rms)
{ 

  xbin = stringToBinNum(binElement.getAttributeValue("binNumX"));
  ybin = stringToBinNum(binElement.getAttributeValue("binNumY"));

  toValue(binElement.getAttributeValue("entries"),entries);
  toValue(binElement.getAttributeValue("height"),height);
  toValue(binElement.getAttributeValue("error"),error);
  toValue(binElement.getAttributeValue("weightedMeanX"),wMeanX);
  toValue(binElement.getAttributeValue("weightedMeanY"),wMeanY);
  toValue(binElement.getAttributeValue("weightedRmsX"),wRmsX);
  toValue(binElement.getAttributeValue("weightedRmsY"),wRmsY);
  toValue(binElement.getAttributeValue("error2"),error2);
  toValue(binElement.getAttributeValue("rms"),rms);

}


void iAIDA::AIDA_XMLStore::Translator::getHisto3DBinData(const DataXML::DataObject &  binElement, int & xbin, int & ybin, int & zbin, int & entries, double & height, double & error, double & wMeanX, double & wMeanY, double & wMeanZ, double & wRmsX, double & wRmsY, double & wRmsZ, double & error2, double & rms)
{ 

  xbin = stringToBinNum(binElement.getAttributeValue("binNumX"));
  ybin = stringToBinNum(binElement.getAttributeValue("binNumY"));
  zbin = stringToBinNum(binElement.getAttributeValue("binNumZ"));

  toValue(binElement.getAttributeValue("entries"),entries);
  toValue(binElement.getAttributeValue("height"),height);
  toValue(binElement.getAttributeValue("error"),error);
  toValue(binElement.getAttributeValue("weightedMeanX"),wMeanX);
  toValue(binElement.getAttributeValue("weightedMeanY"),wMeanY);
  toValue(binElement.getAttributeValue("weightedMeanZ"),wMeanZ);
  toValue(binElement.getAttributeValue("weightedRmsX"),wRmsX);
  toValue(binElement.getAttributeValue("weightedRmsY"),wRmsY);
  toValue(binElement.getAttributeValue("weightedRmsZ"),wRmsZ);
  toValue(binElement.getAttributeValue("error2"),error2);
  toValue(binElement.getAttributeValue("rms"),rms);

}

/// get Cloud entry data 

void iAIDA::AIDA_XMLStore::Translator::getCloudEntryData(const DataXML::DataObject &  entryElement, const int dim, double & xval, double & yval, double & zval, double & weight )
{ 
  if (dim >= 1)   toValue(entryElement.getAttributeValue("valueX"),xval);
  if (dim >= 2)   toValue(entryElement.getAttributeValue("valueY"),yval);
  if (dim >= 3)   toValue(entryElement.getAttributeValue("valueZ"),zval);

  toValue(entryElement.getAttributeValue("weight"),weight);
}

/// get DataPoint measurement 

void iAIDA::AIDA_XMLStore::Translator::getDataPointMeasurement(const DataXML::DataObject & dataElement,  double & value, double & eminus, double & eplus) 
{ 
  toValue(dataElement.getAttributeValue("value"),value);
  toValue(dataElement.getAttributeValue("errorPlus"),eplus);
  toValue(dataElement.getAttributeValue("errorMinus"),eminus);
}


/// Helper methods to tranform numbers tranformation to strings 

std::string iAIDA::AIDA_XMLStore::Translator::toString(int number) const
{
  // specialisation for integers
  std::ostringstream buf;
  buf << number;
#ifndef BADENDS
  buf << std::ends;
#endif
  std::string ret = buf.str();
  return ret;
}


/// private helper methods 

std::string iAIDA::AIDA_XMLStore::Translator::binNumToString(int ibin) 
{
  std::string id;
  if (ibin == AIDA::IAxis::UNDERFLOW_BIN)  id = "UNDERFLOW";
  else if (ibin == AIDA::IAxis::OVERFLOW_BIN)  id = "OVERFLOW";
  else                      id = toString(ibin);
  return id; 
}

int iAIDA::AIDA_XMLStore::Translator::stringToBinNum(const std::string & sbin) 
{
  int ibin = -9999;
  if (sbin == "UNDERFLOW") 
    ibin =   static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN);
  else if (sbin == "OVERFLOW") 
    ibin =  static_cast<int> ( AIDA::IAxis::OVERFLOW_BIN);
  else
    toValue(sbin,ibin); 

  return ibin; 
}



