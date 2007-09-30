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

// Implementation file for class FitterOptionParser
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct 11 12:10:34 2002
// 
// Last update: Fri Oct 11 12:10:34 2002
// 

#include "FitterOptionParser.h"


#ifdef OLDSTREAMS
# include <strstream>
# define istringstream istrstream
#else
# include <sstream>
#endif


namespace iAIDA  { 
  namespace AIDA_Fitter { 


FitterOptionParser::FitterOptionParser(const std::string & opt) 
{
  // defaults 
  m_error_up = 1.0; 
  m_print_level = -1; 
  m_use_minos = false; 

  m_options = opt; 
  // unpack options 
  getValueOption("printLevel",m_print_level); 
  getValueOption("errorUP",m_error_up); 
  m_use_minos = getBoolOption("useMinos"); 
  
}

FitterOptionParser::~FitterOptionParser() 
{
}

FitterOptionParser::FitterOptionParser(const FitterOptionParser &) 
{
}

FitterOptionParser & FitterOptionParser::operator = (const FitterOptionParser &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

int  FitterOptionParser::printLevel() const { return m_print_level; } 

bool  FitterOptionParser::useMinos() const { return m_use_minos; } 

double  FitterOptionParser::errorUP() const { return m_error_up; } 


template <class T> 
void  FitterOptionParser::getValueOption(const std::string & keyword, T & value) 
{ 
  std::string::size_type idx;
  idx = m_options.find( keyword );
  if ( idx != std::string::npos ) {
    std::string substring1 = m_options.substr( idx + keyword.length() );
    std::string::size_type idx1 = substring1.find( "=" );
    if ( idx1 != std::string::npos ) {
      std::istringstream is( substring1.substr( idx1 + 1 ).c_str() );
      is >> std::ws;
      if ( ! is.eof() ) {
	is >> value;
      }
    }
  }
}

bool  FitterOptionParser::getBoolOption(const std::string & keyword) 
{ 
  bool value = false; 
  if ( m_options.find( keyword ) != std::string::npos ) value = true;
  return value; 
}

  }     // end of namespace iAIDA::AIDA_Fitter
} 
