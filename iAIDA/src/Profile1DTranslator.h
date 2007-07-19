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

// Header file for class Histo1DTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

/*    
 *    class containing methods to transfrom a 1D Histogram to and from XML 
 *    using DataXML library.
 *    When translating to XML the class it is constructed using 
 *    AIDA Histogram1D plus name and path and creates a DataXML::DataObject. 
 *    The DataObject is filled with all histogram information when the 
 *    toXML() methos is called, and the data object is returned using the
 *    dataObject() method. 
 *    Transforming from XML, the class is constructed using a 
 *    DataXML::DataObject in the constructor and  method createFromXML
 *    takes an AIDA::Dev::IDevHistogramFactory to create the corresponding 
 *    AIDA Histogram
 *    The helper methods used to transfrom to/from XML are in the base class 
 *    Translator since are used also by the other AIDA objects. 
 * 
 */

#ifndef PROFILE1DTRANSLATOR
#define PROFILE1DTRANSLATOR 1

#include "Translator.h"
#include <string>

#include "DataXML/DataObject.h"

namespace AIDA { 
   class IProfile1D; 
   namespace Dev { 
     class IDevProfile1D;
     class IDevHistogramFactory;
   }
 }

namespace iAIDA { 
   namespace AIDA_XMLStore { 


class Profile1DTranslator : public Translator {

public: 
  // constructor for writing to XML
  Profile1DTranslator(const AIDA::IProfile1D * histo, const std::string & name, const std::string & path ); 
  // constructor for reading from XML
  Profile1DTranslator(const DataXML::DataObject * element); 

  virtual ~Profile1DTranslator(); 

private:
// usually copying is non trivial, so we make this unaccessible
  Profile1DTranslator(const Profile1DTranslator &); 
  Profile1DTranslator & operator = (const Profile1DTranslator &); 

public: 

  /// transform to XML filling the  data Object 
  bool toXML();
  /// create Histogram from XML 
  AIDA::Dev::IDevProfile1D * createFromXML(AIDA::Dev::IDevHistogramFactory& factory); 

  /// getters for class members 
  DataXML::DataObject & dataObject() { return m_element; } 
  const AIDA::IProfile1D * histo() { return m_histo; } 
  const std::string & name() { return m_name; } 
  const std::string & path() { return m_path; } 

protected: 

private: 

  // private methods
  bool setAxes(); 
  bool setStatistics();
  bool setData(); 

  /// private member data 

  /// histogram XML element 
  DataXML::DataObject m_element;
  /// representative AIDA histogram
  const AIDA::IProfile1D * m_histo;  
  /// object name 
  std::string m_name; 
  /// object path 
  std::string m_path;  
}; 



   }      // end of namespace iAIDA::AIDA_XMLStore
}

#endif /* PROFILE1DTRANSLATOR */
