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

// Header file for class AIDA_FunctionCatalog
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Sun Oct  6 17:42:23 2002
// 
// Last update: Sun Oct  6 17:42:23 2002
// 
#ifndef IAIDA_AIDA_FUNCTION_AIDA_FUNCTIONCATALOG
#define IAIDA_AIDA_FUNCTION_AIDA_FUNCTIONCATALOG 1


#include "AIDA_Dev/IDevFunctionCatalog.h"
#include "FML/FunctionCatalogue.h"

namespace fml { 
     class FunctionCatalogue; 
 }

namespace iAIDA  { 
  namespace AIDA_Function { 


class AIDA_FunctionCatalog : virtual public AIDA::Dev::IDevFunctionCatalog {

 public: 
  
  AIDA_FunctionCatalog(std::string repository_dir = "", std::string dbfile = ""); 
  virtual ~AIDA_FunctionCatalog(); 

private:
// usually copying is non trivial, so we make this unaccessible
  AIDA_FunctionCatalog(const AIDA_FunctionCatalog &); 
  AIDA_FunctionCatalog & operator = (const AIDA_FunctionCatalog &); 

public: 

  /// from AIDA public interface

  bool add(std::string nameId, AIDA::IFunction * f);
  bool add(std::string nameId, const std::string & codelet);

  /// Get a list of all available functions
  std::vector<std::string>   list();

  /// Remove function from the catalog.
  void remove(const std::string & nameId);

  /// Write contents of the catalog to disk.
  bool storeAll(const std::string & nameOnDisk);
  
  /// load contents of the catalog from disk
  bool loadAll(const std::string & nameOnDisk);


  /// from developer interface
 
  AIDA::IFunction * create(std::string name); 


protected: 


private: 

  fml::FunctionCatalogue *  m_cat;

  // cache list of functions
  std::vector<std::string> m_funList; 
}; 

  }     // end of namespace iAIDA::AIDA_Function
}
#endif /* AIDA_FUNCTIONCATALOG */
