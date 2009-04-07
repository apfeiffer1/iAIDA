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

// Implementation file for class AIDA_FunctionCatalog
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Sun Oct  6 17:42:23 2002
// 
// Last update: Sun Oct  6 17:42:23 2002
// 

#include "AIDA_Function/AIDA_FunctionCatalog.h"
#include "AIDA_Function/AIDA_FunctionAdapter.h"
#include "FML/FunctionCatalogue.h"
#include "FML/Util/debug.h"

namespace iAIDA  { 
  namespace AIDA_Function { 


AIDA_FunctionCatalog::AIDA_FunctionCatalog(std::string /* repository_dir */ , std::string /* dbfile */) 
{
  // I need a dbfile for FML catalog 
//   if (!dbfile.empty()) 
//     m_cat = new fml::FunctionCatalogue(repository_dir, dbfile); 
//   else 
    m_cat = 0; 
}

AIDA_FunctionCatalog::~AIDA_FunctionCatalog() 
{
  if (m_cat) delete m_cat; 
}


AIDA_FunctionCatalog & AIDA_FunctionCatalog::operator = (const AIDA_FunctionCatalog &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}


bool AIDA_FunctionCatalog::add(std::string /* nameId */, AIDA::IFunction * /* f */)
{
  INFO_MSG("AIDA_FunctionCatalog::add(..) , sorry not yet implemented..."); 
  return false; 
//   if(!f) return false;
//   if (!m_cat) return false; 
//   return m_cat->storeFunction(nameId, f->codeletString());
}

bool AIDA_FunctionCatalog::add(std::string /* nameId */, const std::string & /*codelet */)
{
  INFO_MSG("AIDA_FunctionCatalog::add(..) , sorry not yet implemented..."); 
  return false; 
//   if (!m_cat) return false; 
//   return m_cat->storeFunction(nameId, codelet);
}

/// Get a list of all available functions
std::vector<std::string>   AIDA_FunctionCatalog::list()
{
  m_funList = fml::FunctionCatalogue::listAvailableFunctions(); 
  return m_funList;
}
/// Remove function from the catalog.

void AIDA_FunctionCatalog::remove(const std::string & /* nameId */ ) 
{
  INFO_MSG("AIDA_FunctionCatalog::remove() , sorry not yet implemented..."); 
}   

/// Write contents of the catalog to disk.
bool AIDA_FunctionCatalog::storeAll(const std::string & /* nameOnDisk */)
{
  INFO_MSG("AIDA_FunctionCatalog::storeAll(..) , sorry not yet implemented..."); 
  return false; 
}   
 
/// load contents of the catalog from disk
bool AIDA_FunctionCatalog::loadAll(const std::string & /* nameOnDisk */)
{
  INFO_MSG("AIDA_FunctionCatalog::loadAll(..) , sorry not yet implemented..."); 
  return false; 
}   

/// from developer interface
 
AIDA::IFunction * AIDA_FunctionCatalog::create(std::string name)
{  
  std::auto_ptr<fml::IFMLFunction> fml_f(fml::FunctionCatalogue::createFunction(name));
  if(!fml_f.get()) return 0; 

  AIDA::IFunction *f = new AIDA_FunctionAdapter(fml_f);
  return f;
}


  }     // end of namespace iAIDA::AIDA_Function
} 
