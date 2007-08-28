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

// Implementation file for class FunctionTranslator
// 
// Created by: moneta  at Thu Sep 12 10:41:06 2002
// 
// Last update: Thu Sep 12 10:41:06 2002
// 

#include "FunctionTranslator.h"
#include "AIDA/IFunction.h"
#include "AIDA/IModelFunction.h"
#include "AIDA_Dev/IDevFunction.h"
#include "AIDA_Dev/IDevFunctionFactory.h"

#include "DataXML/DataObject.h"

static const std::string emptyString = "";

/// constructors for writing (histo -> xml)  
iAIDA::AIDA_XMLStore::FunctionTranslator::FunctionTranslator(const AIDA::IFunction * f, const std::string & name, const std::string & path ) : 
  m_function(f), m_name(name), m_path(path)
{
}
/// constructors for reading (xml -> histo) 
iAIDA::AIDA_XMLStore::FunctionTranslator::FunctionTranslator(const DataXML::DataObject * xmlObject) : 
  m_element(*xmlObject)
{
}


iAIDA::AIDA_XMLStore::FunctionTranslator::~FunctionTranslator() 
{  // no op
}

iAIDA::AIDA_XMLStore::FunctionTranslator::FunctionTranslator(const iAIDA::AIDA_XMLStore::FunctionTranslator &) : Translator()  
{
}

iAIDA::AIDA_XMLStore::FunctionTranslator & iAIDA::AIDA_XMLStore::FunctionTranslator::operator = (const iAIDA::AIDA_XMLStore::FunctionTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

bool iAIDA::AIDA_XMLStore::FunctionTranslator::toXML() 
{ 
  // write name (no need for title is in annotation)
  appendObjectHeader(m_element,"function",m_name,emptyString,m_path); 
  // extra stuff for functions (is normalized)
  std::string norm = "false"; 
  AIDA::IModelFunction* mf = 
    dynamic_cast< AIDA::IModelFunction* >( const_cast<AIDA::IFunction * > (m_function ));
  if (mf) { 
    if (mf->isNormalized()) 
	   norm = "true"; 
  }
  m_element.setAttribute("isNormalized", norm );  
  
  appendAnnotation(m_element,m_function->annotation()); 

  if (!setData () ) return false;
  return true;

}


bool iAIDA::AIDA_XMLStore::FunctionTranslator::setData()
{ 
  // write data element and append to the dataobject

  DataXML::DataObject codeletElement;
  codeletElement.setName("codelet");
  codeletElement.setAttribute("name",m_function->codeletString());
  m_element.appendChild(codeletElement);

  DataXML::DataObject argsElement;
  argsElement.setName("arguments");
  // put here the ranges
  m_element.appendChild(argsElement);
  
  DataXML::DataObject parsElement;
  parsElement.setName("parameters");
  // put here the params
  std::vector<std::string> parNames = m_function->parameterNames(); 
  std::vector<double> params = m_function->parameters(); 
  for (int i = 0; i < m_function->numberOfParameters() ; ++ i) { 
    DataXML::DataObject pElement;
    pElement.setName("parameter");
    pElement.setAttribute("name",parNames[i]); 
    pElement.setAttribute("value",toString(params[i]) ); 
    parsElement.appendChild(pElement);
  }
  m_element.appendChild(parsElement); 

  return true;
}

////  reading methods 

AIDA::Dev::IDevFunction * iAIDA::AIDA_XMLStore::FunctionTranslator::createFromXML(AIDA::Dev::IDevFunctionFactory& factory)
{ 
  // read header 
  std::string title,options = emptyString;
  getObjectHeader(m_element,m_name,title,m_path,options); 
  bool norm = false; 
  if (m_element.getAttributeValue("isNormalized") == "true") 
    norm = true;

  // if title is empty try with name
  if (title.empty()) title = m_name; 

  const DataXML::DataObject * codElement = m_element.getChild("codelet");
  std::string codelet = ""; 
  if (codElement) 
    codelet = codElement->getAttributeValue("name");

  // read annotation
  AnnotationData annoData; 
  getAnnotation(m_element,annoData); 

  // Now I can create the function 

  std::string model = codelet; 
  // then try with title
  if (model.empty()) model = title; 


  AIDA::Dev::IDevFunction * f  = 
    factory.createFunctionByName( model);

  if (!f) return 0; 

  //no need to  get statistics since for AIDA 

  // set annotation
  AIDA::IAnnotation & anno = f->annotation(); 
  setAnnotation(&anno,annoData); 

  // forget about the range.. (to be done..)

  // get the data 
 
  const DataXML::DataObject * dataElement = m_element.getChild("parameters");
  if (!dataElement) return f;
  std::vector<std::string> parNames; 
  std::vector<double> params; 
  for (std::vector<DataXML::DataObject>::const_iterator entryElement = dataElement->children().begin(); entryElement != dataElement->children().end(); ++entryElement) {  
    if (entryElement->name() == "parameter") { 
      double val;   
      std::string name = entryElement->getAttributeValue("name"); 
      toValue(entryElement->getAttributeValue("value"), val);
      parNames.push_back(name); 
      params.push_back(val); 
    }
  }
  // check .. 
  if (f->numberOfParameters() != static_cast<int>(params.size() )) { 
    std::cerr << " FunctionTranslator: Error wrong number of parameters " << std::endl;
    return 0; 

  }

  f->setParameters(params); 
  f->setParameterNames(parNames); 

  return f; 
}
