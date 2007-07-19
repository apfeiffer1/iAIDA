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

// Header file for class StoreTranslator
// 
// Created by: moneta  at Tue Sep 17 10:39:41 2002
// 
// Last update: Tue Sep 17 10:39:41 2002
// 
#ifndef STORETRANSLATOR
#define STORETRANSLATOR 1

/*    
 *    class containing helper methods to write and read an 
 *    XML store 
 */

#include "Translator.h"
#include <string>
#include <iostream>
#include <map>

#include "DataXML/DataObject.h"
//-ap #include "DataXML/XMLSealStream.h"
#include "DataXML/XMLStream.h"


namespace AIDA { 
   class IHistogram1D; 
   class IHistogram2D; 
   class IHistogram3D; 
   class IProfile1D; 
   class IProfile2D; 
   class ICloud1D; 
   class ICloud2D; 
   class ICloud3D; 
   class IDataPointSet; 
   class IFunction;
   class ITuple;
 }


//-ap namespace seal { 
//-ap   class seal::IOInput; 
//-ap   class seal::IOOutput; 
//-ap }

namespace iAIDA { 
   namespace AIDA_XMLStore { 


class StoreTranslator : public Translator {

public: 

  //typedef std::multimap<std::string, const DataXML::DataObject *> ObjectMap;

  // translator receive streams as input but does not manage them  

  //-ap  StoreTranslator(const std::string & storeName); 
  StoreTranslator(std::ostream & stream); 
  StoreTranslator(std::istream & stream); 
  //-ap StoreTranslator(seal::IOInput & stream); 
  //-ap StoreTranslator(seal::IOOutput & stream); 
  virtual ~StoreTranslator(); 

private:
// usually copying is non trivial, so we make this unaccessible
  StoreTranslator(const StoreTranslator &); 
  StoreTranslator & operator = (const StoreTranslator &); 

public: 

  /// read the store
  bool read(DataXML::DataObject & aidaElement); 
  /// write all store to the file
  bool write();    
  /// append object 
   bool append(const AIDA::IHistogram1D & h, const std::string & name, const std::string & path);
   bool append(const AIDA::IHistogram2D & h, const std::string & name, const std::string & path);
   bool append(const AIDA::IHistogram3D & h, const std::string & name, const std::string & path);
   bool append(const AIDA::IProfile1D & p, const std::string & name, const std::string & path);
   bool append(const AIDA::IProfile2D & p, const std::string & name, const std::string & path);
   bool append(const AIDA::ICloud1D & c, const std::string & name, const std::string & path);
   bool append(const AIDA::ICloud2D & c, const std::string & name, const std::string & path);
   bool append(const AIDA::ICloud3D & c, const std::string & name, const std::string & path);
   bool append(const AIDA::IDataPointSet & dps, const std::string & name, const std::string & path);
   bool append(const AIDA::IFunction & f, const std::string & name, const std::string & path);
   bool append(AIDA::ITuple & t, const std::string & name, const std::string & path);
 
  // return root element 
  DataXML::DataObject & rootElement() { return m_element; } 

protected: 


private: 

  /// private methods 
  //-ap void writeFileHeader(seal::IOOutput & stream) const; 
  void writeFileHeader(std::ostream & stream) const; 


  /// private member data 



  /// main  store XML element (<aida> element)  
  DataXML::DataObject m_element;

#ifdef USE_SEAL_IO
  ///xml input stream 
  DataXML::InputXMLSealStream *  m_inputStream; 
  ///xml output stream 
  DataXML::OutputXMLSealStream *  m_outputStream; 
#endif

  ///xml input stream 
  DataXML::InputXMLStream *  m_inputStream; 
  ///xml output stream 
  DataXML::OutputXMLStream *  m_outputStream; 

}; 

   }      // end of namespace iAIDA::AIDA_XMLStore
}

#endif /* STORETRANSLATOR */





