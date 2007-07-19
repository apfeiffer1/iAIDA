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

// Implementation file for class TupleTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "TupleTranslator.h"
#include "AIDA/ITuple.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/ITupleHeader.h"
#include <algorithm>
#include <cctype>


#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::TupleTranslator::TupleTranslator(AIDA::ITuple * tuple, const std::string & name, const std::string & path ) : 
  m_tuple(tuple), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::TupleTranslator::TupleTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::TupleTranslator::~TupleTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::TupleTranslator::TupleTranslator(const iAIDA::AIDA_XMLStore::TupleTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::TupleTranslator & iAIDA::AIDA_XMLStore::TupleTranslator::operator = (const iAIDA::AIDA_XMLStore::TupleTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::TupleTranslator::toXML() 
{ 
  // write name and title
  appendObjectHeader(m_element,"tuple",m_name,m_tuple->title(),m_path); 
  // get tuple options using DevTuple 
  AIDA::Dev::IDevTuple * dev_tuple = dynamic_cast< AIDA::Dev::IDevTuple * > 
    (const_cast<AIDA::ITuple * > (m_tuple) ); 
  if ( !dev_tuple ) return false; 
  m_element.setAttribute("options", dev_tuple->header().options() ); 
  
  appendAnnotation(m_element,m_tuple->annotation()); 

  if (!setHeader() ) return false; 
  if (!setData () ) return false;

  return true;

}

// set columns header

bool iAIDA::AIDA_XMLStore::TupleTranslator::setHeader()
{ 
  DataXML::DataObject colsElement;
  colsElement.setName("columns"); 
  for (int i = 0; i < m_tuple->columns(); ++i) { 
    DataXML::DataObject cElement;
    cElement.setName("column"); 
    // check if is a subtupla
    std::string colType = m_tuple->columnType(i); 
    if (colType.find("uple") == std::string::npos) { 
      cElement.setAttribute("name",m_tuple->columnName(i)); 
      cElement.setAttribute("type",m_tuple->columnType(i)); 
    }
    else {  
      // case of a tuple in a tuple 
      AIDA::ITuple* subTuple = m_tuple->getTuple(i); 
      std::string nameString = "";
	// add options, if they are in " ( )" - skip now 

      // pi convention :  ITuple {...  } name
      nameString.append("ITuple "); 

      // JAS convention : name =  { ...}      
      //nameString.append( m_tuple->columnName(i) ); 
      //nameString.append(" = ");

      nameString.append(getSubTupleDescription(subTuple) ); 

      nameString.append(" " + m_tuple->columnName(i)); 

      cElement.setAttribute("name",nameString); 
      cElement.setAttribute("type",m_tuple->columnType(i)); 

    }

    colsElement.appendChild(cElement);
  }

  m_element.appendChild(colsElement);
  return true; 
}

// write tuple data (rows) 

bool iAIDA::AIDA_XMLStore::TupleTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject rowsElement;
  rowsElement.setName("rows");
  
  appendRow(rowsElement, m_tuple); 

  m_element.appendChild(rowsElement);
  return true;
}

// get description of a subtuple 

 std::string iAIDA::AIDA_XMLStore::TupleTranslator::getSubTupleDescription(AIDA::ITuple* subTuple) { 
   std::string description = "{ "; 
   if (subTuple == 0) { 
     description += "}"; 
     return description; 
   }
   // remember previous type to avoid repetition
   std::string prev_type = ""; 
   for (int j = 0; j < subTuple->columns(); ++j) {
     // write type and names fpr subtuples
     if (subTuple->columnType(j).find("uple") == std::string::npos) { 
       if (subTuple->columnType(j) != prev_type ) {  
         if ( j > 0) description.append("; "); 
	 description.append(subTuple->columnType(j)); 
	 description.append(" "); 
       } 
       else // if same type 
         if ( j > 0) description.append(", "); 

       description.append(subTuple->columnName(j) );
       prev_type = subTuple->columnType(j);
     }
     else { 
       if ( j > 0) description.append("; "); 
       description.append("ITuple ");
       // JAS convention : name =  { ...}
       //description.append(subTuple->columnName(j) ); 
       //description.append(" = ");	     

       /// do recursive here
       description.append(getSubTupleDescription(subTuple->getTuple(j)) ); 
       // use our convention: name at the end
       description.append(" " + subTuple->columnName(j)); 
       
     }
   }
   
   description += " }"; 
   return description; 
 }

// append row of a tuple

void iAIDA::AIDA_XMLStore::TupleTranslator::appendRow(DataXML::DataObject & parElement, AIDA::ITuple * tuple)
{ 
  // loop on the tuple rows
  tuple->start(); 
  while (tuple->next() ) { 
    DataXML::DataObject dataElement;
    dataElement.setName("row");    
    for (int j = 0; j < tuple->columns(); ++ j) { 
      // get element according to type
      if (tuple->columnType(j) == "double" )              
	appendTupleEntry<double>(dataElement, tuple->getDouble(j) ); 
      else if (tuple->columnType(j) == "float" )
	appendTupleEntry<float>(dataElement, tuple->getFloat(j) ); 
      else if (tuple->columnType(j) == "int" )
	appendTupleEntry(dataElement, tuple->getInt(j) ); 
      else if (tuple->columnType(j) == "short" )
	appendTupleEntry(dataElement, tuple->getShort(j) ); 
      else if (tuple->columnType(j) == "long" )
	appendTupleEntry(dataElement, tuple->getLong(j) ); 
      else if (tuple->columnType(j) == "char" )
	appendTupleEntry(dataElement, tuple->getChar(j) ); 
      else if (tuple->columnType(j) == "bool" || tuple->columnType(j) == "boolean")
	appendTupleEntry(dataElement, tuple->getBoolean(j) ); 
      //  for strings can be "string" or "std::string" 
      else if (tuple->columnType(j).find("string") != std::string::npos )
	appendTupleEntry(dataElement, tuple->getString(j) );       
      // case of Tuple in a Tuple (check if type contains "uple" )
      else if (tuple->columnType(j).find("uple") != std::string::npos ) { 
	DataXML::DataObject subTupleElement;
	subTupleElement.setName("entryITuple"); 
        AIDA::ITuple * subTuple = tuple->getTuple(j); 
	// append row
	appendRow(subTupleElement,subTuple); 
	dataElement.appendChild(subTupleElement); 
      }
    }
    // append row element to parent      
    parElement.appendChild(dataElement); 
  }
}

// append tuple entry (template on all possible types )

template <class T> 
bool iAIDA::AIDA_XMLStore::TupleTranslator::appendTupleEntry(DataXML::DataObject & dataElement,  T value) 
{ 

  DataXML::DataObject entryElement;
  entryElement.setName("entry");
  entryElement.setAttribute("value",  toString(value) );

  dataElement.appendChild(entryElement);  

  return true;
}
/*******************************************************************/
////  reading methods 
/*******************************************************************/

AIDA::Dev::IDevTuple * iAIDA::AIDA_XMLStore::TupleTranslator::createFromXML(AIDA::Dev::IDevTupleFactory& factory, AIDA::Dev::IBackingStore * store)
{ 
  // read header 
  std::string title,options = emptyString;
  getObjectHeader(m_element,m_name,title,m_path,options); 


  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // read columns information 
  std::vector<std::string> colNames; 
  std::vector<std::string> colTypes; 
  getColumns(colNames,colTypes); 

  // Now I can create the tuple (similar code as in AIDA_TupleFactory) 

  AIDA::Dev::IDevTuple * tuple  = factory.create( store, title, options );

  if (!tuple) { 
    std::cerr << " AIDA_XMLStore::TupleTranslator - Cannot create Tuple " << std::endl; 
    return 0; 
  }

  std::string fullPath = m_path + "/" + m_name; 
  if (m_path == "/") fullPath = m_path + m_name;  // avoid two //

  // build tuple description string 
  std::string description = ""; 
  for (unsigned int i = 0; i < colNames.size(); ++i) { 
    if ( i > 0) description += "; "; 
    description += colTypes[i] + "  " + colNames[i]; 
  }

  if (! factory.buildTupleHeader( description, fullPath, tuple->header() ) ) { 
    std::cerr << " AIDA_XMLStore::TupleTranslator - Cannot build Tuple Header" << std::endl; 
    delete tuple;
    return 0; 
  }
  if ( ! store->writeTupleHeader( tuple->header() ) ) {
    std::cerr << " AIDA_XMLStore::TupleTranslator - Cannot write Tuple Header" << std::endl; 
    delete tuple;
    return 0; 
  }


  //no need to  get statistics since for AIDA 

  // set annotation
  AIDA::IAnnotation & anno = tuple->annotation(); 
  setAnnotation(&anno,annoData); 


  //debug
  //for (int j = 0; j < tuple->columns(); ++j) 
  //  cout << " Column " << j << "  -- " << tuple->columnName(j) << "   " << tuple->columnType(j) << endl; 


  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("rows");
  if (!dataElement) return tuple; 

  // fill tuple
  fillTuple(*dataElement,tuple); 
    
  return tuple; 
}


void iAIDA::AIDA_XMLStore::TupleTranslator::getColumns(std::vector<std::string> & colNames, std::vector<std::string> & colTypes ) 
{ 
  const DataXML::DataObject * colsElement = m_element.getChild("columns");
  if (colsElement) { 
    // loop on children 
    for (std::vector<DataXML::DataObject>::const_iterator itr = colsElement->children().begin(); itr != colsElement->children().end(); ++itr) {
      if (itr->name() == "column") { 
	std::string type = itr->getAttributeValue("type");
	// for compatibility with file written from Java 
	if (type == "boolean") type = "bool";   
	if (type == "byte") type = "short"; 
	if (type.find("String") != std::string::npos || 
	    type.find("string") != std::string::npos ) type = "string";
	
	  colTypes.push_back(type);
	  colNames.push_back(itr->getAttributeValue("name") );
	  // TO DO : transform here from JAS connvention to pi
	  // ignore for the moment support for JAS files  
	//if (type.find("uple") != std::string::npos) 
      }
    }
  }

}

void iAIDA::AIDA_XMLStore::TupleTranslator::fillTuple(const DataXML::DataObject & dataElement, AIDA::ITuple * tuple)
{
// loop on rows
  for (std::vector<DataXML::DataObject>::const_iterator rowElement = dataElement.children().begin(); rowElement != dataElement.children().end(); ++rowElement) {  
    // need to check on the name to eliminate white spaces
    if (rowElement->name() == "row") { 
      // loop on entries (columns)
      int iCol = 0;      // column index
      for (std::vector<DataXML::DataObject>::const_iterator entryElement = rowElement->children().begin(); entryElement != rowElement->children().end(); ++entryElement) {  
	if (iCol < tuple->columns() ) { 
	  // need to check on the name to eliminate white spaces
	  if (entryElement->name() == "entry") {
	    std::string colType = tuple->columnType(iCol);
	    std::transform ( colType.begin(), colType.end(), colType.begin(), tolower );
	    if (colType == "float") {
	      float val = 0; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType == "double") { 
	      double val = 0; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType == "int") {
	      int val = 0; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType == "long") {
	      long val = 0; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType == "short") {
	      short val = 0; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType == "char") {
	      char val; 
	      toValue(entryElement->getAttributeValue("value"),val); 
	      tuple->fill(iCol, val); 
	    }
	    if (colType.find("string") != std::string::npos) {
	      std::string s = entryElement->getAttributeValue("value"); 
	      tuple->fill(iCol, s); 
	    }
	  }
	  else if (entryElement->name() == "entryITuple") {
	    AIDA::ITuple * subTuple = tuple->getTuple(iCol);
	    if (subTuple == 0) { 
	      std::cout << " Cannot retrieve subtuple for column " << iCol 
			<< " with name " << tuple->columnName(iCol) << " and type " 
			<< tuple->columnType(iCol) << std::endl;
	      return; 
	    }
	    fillTuple(*entryElement,subTuple); 
	  }
	  iCol++;
	}
      }
      tuple->addRow(); 
    }
  }
}
