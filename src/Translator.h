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

// Header file for class Translator
// 
// Created by: moneta  at Fri Sep 13 10:43:55 2002
// 
// Last update: Fri Sep 13 10:43:55 2002
// 

/*   Helper class containing methods using by the various types to 
 *    transform into and from XML
 *   It uses of the DataXML package
 */
 
#ifndef TRANSLATOR
#define TRANSLATOR 1

#include <string>
#include <vector>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif



namespace DataXML { 
   class DataObject; 
 }

namespace AIDA { 
   class IAxis; 
   class IAnnotation;
}


namespace iAIDA { 
   namespace AIDA_XMLStore { 

class Translator {

public: 
  Translator(); 
  virtual ~Translator();

private:
// usually copying is non trivial, so we make this unaccessible
  Translator(const Translator &); 
  Translator & operator = (const Translator &); 

public: 

  typedef std::vector<DataXML::DataObject>::const_iterator ElementIterator;

protected: 

  // helper methods to be used by the specific translators 

  bool appendAidaHeader(DataXML::DataObject & aidaElement); 

  bool appendObjectHeader(DataXML::DataObject & objElement, 
			  const std::string & elementName, 
			  const std::string & name, 
			  const std::string & title = "", 
			  const std::string & path = "", 
			  const std::string & options = "");

  bool appendAnnotation(DataXML::DataObject & objElement,
			const AIDA::IAnnotation& anno); 

  bool appendAxis(DataXML::DataObject & objElement, 
		  const std::string& axisName, 
		  const AIDA::IAxis& axis);

  bool appendStatistics(DataXML::DataObject & statisticsElement, 
			const std::string& axisName, 
			double mean, double rms, double skew = 0);

  bool appendHisto1DBinData(DataXML::DataObject & dataElement,  
			    int xbin, int entries, double height, double error, 
			    double wMean = 0, double rms = 0, 
			    double wRms = 0,  double error2= 0 );

  bool appendHisto2DBinData(DataXML::DataObject & dataElement,  
			    int xbin, int ybin , int entries, 
			    double height, double error, 
			    double wMeanX = 0, double wMeanY=0, double rms = 0);

  bool appendHisto3DBinData(DataXML::DataObject & dataElement,  
			    int xbin, int ybin , int zbin, int entries, 
			    double height, double error, 
			    double wMeanX = 0, double wMeanY=0, double wMeanZ=0,  double rms = 0 );

  bool appendCloudEntry(DataXML::DataObject & dataElement,  int dimension, 
			double xval, double yval, double zval, double weight); 

  bool appendDataPointMeasurement(DataXML::DataObject & dataElement, 
				  double value, double eminus, double eplus); 

  /// reading methods

  void getObjectHeader(const DataXML::DataObject & objElement, std::string & name, 
		  std::string & title, std::string & path, std::string & options); 

  typedef std::vector<std::pair<std::string, std::pair<std::string, bool> > > AnnotationData;
 
  ///  get Annotation from XML and store in temp data 

  void getAnnotation(const DataXML::DataObject & obj,AnnotationData & annoData);
  /// write the Annotation
  void setAnnotation(AIDA::IAnnotation * anno, const AnnotationData & annoData); 

  void getAxis(const DataXML::DataObject & objElement, const std::string& axisName, 
	  int& nBins, double& low, double& high, std::vector<double> & edges);

  void getStatistics(const DataXML::DataObject & statisticsElement, 
		const std::string& axisName, 
		double & mean, double & rms, double & skew);

  
  void getHisto1DBinData(const DataXML::DataObject &  binElement, int & xbin, int & entries, double & height, double & error, double & wMean, double & wRms, double & error2, double & rms);

  void getHisto2DBinData(const DataXML::DataObject &  binElement, int & xbin, int & ybin, int & entries, double & height, double & error, double & wMeanX, double & wMeanY, double & wRmsX, double & wRmsY, double & error2, double & rms);

  void getHisto3DBinData(const DataXML::DataObject &  binElement, int & xbin, int & ybin, int & zbin, int & entries, double & height, double & error, double & wMeanX, double & wMeanY, double & wMeanZ, double & wRmsX, double & wRmsY, double & wRmsZ, double & error2, double & rms);

  void getCloudEntryData(const DataXML::DataObject & entryElement, 
			 const int dim, double & xval, double & yval, 
			 double & zval, double & weight );

  void getDataPointMeasurement(const DataXML::DataObject & dataElement, 
			       double & value, double & eminus, double & eplus); 
  // other helper methods
  template<class FP>
  std::string toString(FP v) const;
  std::string toString(int i) const;

  template<class T>
  bool toValue(std::string s, T& val);

  // static members available to all subclasses 

  static const int SCI_PRECISION;
  static const std::string XML_VERSION; 
  static const std::string ENCODING_TYPE; 
  static const std::string PACKAGE; 
  static const std::string PACKAGE_VERSION; 


private: 

  // private helper methods 

  std::string  binNumToString(int bn);
  int stringToBinNum(const std::string & bin); 
  

}; 

   }      // end of namespace iAIDA::AIDA_XMLStore
}


/// Helper methods to tranform numbers tranformation to strings 
/// template method need to be in header file 

template<class FP>
std::string iAIDA::AIDA_XMLStore::Translator::toString(FP number) const
{
  // generic method for floating point types
  std::ostringstream buf;
  buf.setf(std::ios::scientific);
  buf.precision(iAIDA::AIDA_XMLStore::Translator::SCI_PRECISION);
  buf << number;
#ifndef BADENDS
  buf << std::ends;
#endif
  std::string ret = buf.str();
  return ret;
}

/**
 * convert std::string to value using stream operator >>.
 * consequently, type T must have operator >> for istream
 */

template<class T>
bool iAIDA::AIDA_XMLStore::Translator::toValue(std::string s, T& val)
{
  std::istringstream buf(s.c_str());
  buf >> std::ws;
  if ( buf.eof() || (!buf.good() ) ) return false;
  buf >> val;
  return true;
}



#endif /* TRANSLATOR */
