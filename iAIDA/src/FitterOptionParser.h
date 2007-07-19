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

// Header file for class FitterOptionParser
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct 11 12:10:34 2002
// 
// Last update: Fri Oct 11 12:10:34 2002
// 
#ifndef AIDANATIVE_AIDA_FITTER_FITTEROPTIONPARSER
#define AIDANATIVE_AIDA_FITTER_FITTEROPTIONPARSER 1

#include <string>

namespace iAIDA  { 
  namespace AIDA_Fitter { 


class FitterOptionParser {

public: 
  FitterOptionParser(const std::string  & option); 
  virtual ~FitterOptionParser(); 

private:
// usually copying is non trivial, so we make this unaccessible
  FitterOptionParser(const FitterOptionParser &); 
  FitterOptionParser & operator = (const FitterOptionParser &); 

public: 

  // return the print level
  int printLevel() const; 

  // use Minos
  bool useMinos() const;

  //errorUP
  double errorUP() const; 

protected: 

  template <class T> 
  void getValueOption(const std::string & keyword, T & value);
 
  bool getBoolOption(const std::string & keyword); 
  

private: 

  bool m_use_minos; 
  int m_print_level; 
  double m_error_up;
  std::string m_options; 

}; 

  }     // end of namespace iAIDA::AIDA_Fitter
}
#endif /* FITTEROPTIONPARSER */
