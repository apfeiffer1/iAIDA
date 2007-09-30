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

// Implementation file for class StoreTranslator
// 
// Created by: moneta  at Tue Sep 17 10:39:41 2002
// 
// Last update: Tue Sep 17 10:39:41 2002
// 


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AIDA/IConstants.h"

#include "AIDA/ICloud1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/ICloud3D.h"

#include "StoreTranslator.h"
#include "Histo1DTranslator.h"
#include "Histo2DTranslator.h"
#include "Histo3DTranslator.h"
#include "Profile1DTranslator.h"
#include "Profile2DTranslator.h"
#include "Cloud1DTranslator.h"
#include "Cloud2DTranslator.h"
#include "Cloud3DTranslator.h"
#include "DataPointSetTranslator.h"
#include "FunctionTranslator.h"
#include "TupleTranslator.h"
#include "DataXML/DataObject.h"

#include "DataXML/XMLStream.h"
//-ap #include "DataXML/XMLSealStream.h"
//-ap #include "SealBase/IOInput.h"
//-ap #include "SealBase/IOOutput.h"

/// constructurs - for input and outputs
iAIDA::AIDA_XMLStore::StoreTranslator::StoreTranslator( std::istream & stream ) 
  : m_inputStream(0), m_outputStream(0)
{
  m_inputStream =  new DataXML::InputXMLStream(stream);
}
iAIDA::AIDA_XMLStore::StoreTranslator::StoreTranslator( std::ostream & stream ) 
  : m_inputStream(0), m_outputStream(0)
{
  m_outputStream =  new DataXML::OutputXMLStream(stream);
  // append AIDA header
  appendAidaHeader(m_element); 
}

#ifdef USE_SEAL_IO
iAIDA::AIDA_XMLStore::StoreTranslator::StoreTranslator( seal::IOInput & stream ) 
  : m_inputStream(0), m_outputStream(0)
{
  m_inputStream =  new DataXML::InputXMLSealStream(stream);
}


iAIDA::AIDA_XMLStore::StoreTranslator::StoreTranslator( seal::IOOutput & stream ) 
  : m_inputStream(0), m_outputStream(0)
{
  m_outputStream = new DataXML::OutputXMLSealStream(stream); 
  // append AIDA header
  appendAidaHeader(m_element); 
}
#endif

iAIDA::AIDA_XMLStore::StoreTranslator::~StoreTranslator() 
{
  if (!m_outputStream) delete  m_outputStream; 
  if (!m_inputStream) delete m_inputStream;
}

iAIDA::AIDA_XMLStore::StoreTranslator::StoreTranslator(const iAIDA::AIDA_XMLStore::StoreTranslator &) : Translator() 
{
}

iAIDA::AIDA_XMLStore::StoreTranslator & iAIDA::AIDA_XMLStore::StoreTranslator::operator = (const iAIDA::AIDA_XMLStore::StoreTranslator &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

/// flush  all content to the XML file 

bool iAIDA::AIDA_XMLStore::StoreTranslator::write() 
{ 
  
  writeFileHeader(m_outputStream->getDefaultStream()); 
  return m_outputStream->write(m_element); 
  
}

/// read from XML file 

bool iAIDA::AIDA_XMLStore::StoreTranslator::read(DataXML::DataObject & aidaElement) 
{ 

  // read AIDA header 
  m_element = m_inputStream->read();
  if (m_element.name() == "aida") { 
    // std::cout << "AIDA  - version " << m_element.getAttributeValue("version") << std::endl; 
    aidaElement = m_element; 
    return true; 
  } 
  return false; 
}

// methids to append the  objects to the XML element
// histograms 

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IHistogram1D & h, 
						     const std::string& name,
						     const std::string& path )
{ 
  iAIDA::AIDA_XMLStore::Histo1DTranslator t(&h,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IHistogram2D & h, 
						     const std::string& name,
						     const std::string& path )
{ 
  iAIDA::AIDA_XMLStore::Histo2DTranslator t(&h,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IHistogram3D & h, 
						     const std::string& name,
						     const std::string& path )
{ 
  iAIDA::AIDA_XMLStore::Histo3DTranslator t(&h,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

// clouds 

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::ICloud1D & c, 
						     const std::string& name,
						     const std::string& path )

{ 
  // check conversion first
  if (c.isConverted() ) return append(c.histogram(), name, path); 

  iAIDA::AIDA_XMLStore::Cloud1DTranslator t(&c,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

// clouds 2d

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::ICloud2D & c, 
						     const std::string& name,
						     const std::string& path )

{ 
  if (c.isConverted() ) return append(c.histogram(), name, path);

  iAIDA::AIDA_XMLStore::Cloud2DTranslator t(&c,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

// clouds 3d

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::ICloud3D & c, 
						     const std::string& name,
						     const std::string& path )

{ 
  if (c.isConverted() ) return append(c.histogram(), name, path);

  iAIDA::AIDA_XMLStore::Cloud3DTranslator t(&c,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

// profile 

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IProfile1D & h, 
						     const std::string& name,
						     const std::string& path )

{ 
  iAIDA::AIDA_XMLStore::Profile1DTranslator t(&h,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IProfile2D & h, 
						     const std::string& name,
						     const std::string& path )

{ 
  iAIDA::AIDA_XMLStore::Profile2DTranslator t(&h,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

// datapoint set 

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IDataPointSet & dps, 
						     const std::string& name,
						     const std::string& path )

{ 

  iAIDA::AIDA_XMLStore::DataPointSetTranslator t(&dps,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(const AIDA::IFunction & f, 
						     const std::string& name,
						     const std::string& path )

{ 
  iAIDA::AIDA_XMLStore::FunctionTranslator t(&f,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}

bool  iAIDA::AIDA_XMLStore::StoreTranslator::append(AIDA::ITuple & nt, 
						     const std::string& name,
						     const std::string& path )

{ 
  iAIDA::AIDA_XMLStore::TupleTranslator t(&nt,name,path); 
  t.toXML(); 
  m_element.appendChild(t.dataObject()); 
  return true; 
}



/// private methods 

#ifdef USE_SEAL_IO
void iAIDA::AIDA_XMLStore::StoreTranslator::writeFileHeader(seal::IOOutput& stream) const 
#else
  void iAIDA::AIDA_XMLStore::StoreTranslator::writeFileHeader(std::ostream& stream) const 
#endif
{ 

  stream << "<?xml"
	 << " "
	 << "version=\""  << iAIDA::AIDA_XMLStore::Translator::XML_VERSION   << "\""
	 << " "
	 << "encoding=\"" << iAIDA::AIDA_XMLStore::Translator::ENCODING_TYPE << "\""
	 << "?>"
	 << std::endl;
  stream << "<!"
	 << "DOCTYPE"
	 << " " << "aida" << " "
	 << "SYSTEM"
	 << " " << "\"http://aida.freehep.org/schemas/" << std::string(AIDA_VERSION) << "/aida.dtd\"" << " "
	 << ">"
	 << std::endl;

  
#ifdef USE_SEAL_IO

  std::string header =  std::string( "<?xml version=\"")  
         + iAIDA::AIDA_XMLStore::Translator::XML_VERSION   
         + std::string("\" encoding=\"") 
         + iAIDA::AIDA_XMLStore::Translator::ENCODING_TYPE 
         + std::string("\"?>\n");
  header +=  std::string("<!DOCTYPE aida SYSTEM ") 
         + std::string("\"http://aida.freehep.org/schemas/")  
         + std::string(AIDA_VERSION) 
         + std::string("/aida.dtd\" >\n"); 
  
  stream.write(header.c_str(), header.size() ); 

#endif

}

