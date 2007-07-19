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

// Header file for class TupleTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

/*    
 *    class containing methods to transfrom a Tuple to and from XML 
 *    using DataXML library.
 *    When translating to XML the class it is constructed using 
 *    AIDA Tuple plus name and path and creates a DataXML::DataObject. 
 *    The DataObject is filled with all histogram information when the 
 *    toXML() methos is called, and the data object is returned using the
 *    dataObject() method. 
 *    Transforming from XML, the class is constructed using a 
 *    DataXML::DataObject in the constructor and  method createFromXML
 *    takes an AIDA::Dev::IDevTupleFactory and a pointer to 
 *    AIDA::Dev::IBackingStore to create the corresponding Tuple
 *    The helper methods used to transfrom to/from XML are in the base class 
 *    Translator since are used also by the other AIDA objects. 
 * 
 */

#ifndef TUPLETRANSLATOR
#define TUPLETRANSLATOR 1

#include "Translator.h"
#include <string>

#include "DataXML/DataObject.h"

namespace AIDA { 
   class ITuple; 
   namespace Dev { 
     class IDevTuple;
     class IDevTupleFactory;
     class IBackingStore;
   }
 }

namespace iAIDA { 
   namespace AIDA_XMLStore { 


class TupleTranslator : public Translator {

public: 
  // constructor for writing
  TupleTranslator(AIDA::ITuple * tuple, const std::string & name, const std::string & path ); 
  // constructor for reading 
  TupleTranslator(const DataXML::DataObject * element); 

  virtual ~TupleTranslator(); 

private:
// usually copying is non trivial, so we make this unaccessible
  TupleTranslator(const TupleTranslator &); 
  TupleTranslator & operator = (const TupleTranslator &); 

public: 


  DataXML::DataObject & dataObject() { return m_element; } 
  const AIDA::ITuple * histo() { return m_tuple; } 
  const std::string & name() { return m_name; } 
  const std::string & path() { return m_path; } 

  /// transform to XML filling the  data Object 
  bool toXML();
  /// create Cloud from XML 
  AIDA::Dev::IDevTuple * createFromXML(AIDA::Dev::IDevTupleFactory& factory,AIDA::Dev::IBackingStore * store ); 

protected: 





private: 

  // private methods
  bool setData(); 
  bool setHeader(); 
  void appendRow(DataXML::DataObject & parElement, AIDA::ITuple * tuple); 

  template <class T> 
  bool appendTupleEntry(DataXML::DataObject & dataElement, T value); 

  std::string getSubTupleDescription(AIDA::ITuple * tuple); 

  void getColumns(std::vector<std::string> & colNames, std::vector<std::string> & colTypes ); 

  void fillTuple(const DataXML::DataObject & dataEement, AIDA::ITuple * tuple);


  /// private member data 

  /// histogram XML element 
  DataXML::DataObject m_element;
  /// representative AIDA TUPLE (cannot be const) 
  AIDA::ITuple * m_tuple;  
  /// object name 
  std::string m_name; 
  /// object path 
  std::string m_path;  
}; 



   }      // end of namespace iAIDA::AIDA_XMLStore
}

#endif /* TUPLETRANSLATOR */
