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

#ifndef AIDA_CPP_EXCEPTIONS_H
#define AIDA_CPP_EXCEPTIONS_H

#include<exception>
#include<string>

namespace AIDA_CPP {
  typedef std::exception ExceptionI;

  /** base generic exception for AIDA_CPP
   */
  class BaseException: public ExceptionI  {
  public:
    BaseException() throw();
    BaseException(const std::string & mess) throw();
    virtual ~BaseException() throw();
    virtual const char* what() const throw() { return message.c_str();}
    
  private:
    std::string message;
  };
  
  
}

#endif // AIDA_CPP_EXCEPTIONS_H
