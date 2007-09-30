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

// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE
#define IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE 1

#include <string>
#include <map>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class IFilterExpressionBase {
public:
  virtual ~IFilterExpressionBase() {};
  virtual bool bind( const std::map<std::string, void*>& args ) = 0;
  virtual bool accept() const = 0;
};

class IFilterExpressionBaseFactory {
public:
  virtual ~IFilterExpressionBaseFactory() {};
  virtual IFilterExpressionBase* create() = 0;
};

template<class T>
class IFilterExpressionFactory : virtual public IFilterExpressionBaseFactory {
public:
  IFilterExpressionFactory() {};
  ~IFilterExpressionFactory() {};
  IFilterExpressionBase* create() { return new T; }  
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_FILTEREXPRESSIONBASE */
