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

// Header file for class Histo1DConverter
// 
// Created by: moneta  27/2/2004
// 
 

/*    
 *    class for transforming from AIDA to a Root function (TF1) 
 * 
 */

#ifndef IAIDA_AIDA_ROOTCONVERTER_FUNCTIONCONVERTER
#define IAIDA_AIDA_ROOTCONVERTER_FUNCTIONCONVERTER 1


#include <string>
#include <vector>
#include "boost/serialization/singleton.hpp"

class TF1; 

namespace AIDA { 
   class IFunction;
   namespace Dev { 
     class IDevFunction;
   }
 }

namespace iAIDA { 
   namespace AIDA_RootConverter { 


class FunctionConverter  {

public: 
  // constructor 
  FunctionConverter() {} 

  virtual ~FunctionConverter() {}

private:
// usually copying is non trivial, so we make this unaccessible
  FunctionConverter(const FunctionConverter &); 
  FunctionConverter & operator = (const FunctionConverter &); 

public: 

  /// transform to Root 
  static TF1 *  toRoot( AIDA::IFunction & func, const std::string & name, double xmin, double xmax);

  private : 
    

}; 


// functional adapters from AIDA to function pointer understood by ROOT

 class BaseFuncAdapter { 

 public: 

   BaseFuncAdapter() : m_func(0) {}

   BaseFuncAdapter(AIDA::IFunction * f) : m_func(0) { setFunction(f); } 
   
   double operator() (double * x, double *p); 

   void setFunction(AIDA::IFunction *f);

 private: 

   AIDA::IFunction * m_func; 
   std::vector<double> m_par;
   std::vector<double> m_var;

 };

 // have a family of function adapters 
template <int I> 
 class FuncAdapter : public BaseFuncAdapter { 
   public : 
     FuncAdapter() : BaseFuncAdapter() {}
 };

 // have a family of function adapters 
/*
 class FuncAdapter2 : public FuncAdapter { 
   public : 
     FuncAdapter2() : FuncAdapter() {}
 };
*/

 // make function adapters a singleton 
#define MAX_FUNC_ADAPTER 10
typedef boost::serialization::singleton<FuncAdapter<0> > FuncAdapterHolder0;
typedef boost::serialization::singleton<FuncAdapter<1> > FuncAdapterHolder1;
typedef boost::serialization::singleton<FuncAdapter<2> > FuncAdapterHolder2;
typedef boost::serialization::singleton<FuncAdapter<3> > FuncAdapterHolder3;
typedef boost::serialization::singleton<FuncAdapter<4> > FuncAdapterHolder4;
typedef boost::serialization::singleton<FuncAdapter<5> > FuncAdapterHolder5;
typedef boost::serialization::singleton<FuncAdapter<6> > FuncAdapterHolder6;
typedef boost::serialization::singleton<FuncAdapter<7> > FuncAdapterHolder7;
typedef boost::serialization::singleton<FuncAdapter<8> > FuncAdapterHolder8;
typedef boost::serialization::singleton<FuncAdapter<9> > FuncAdapterHolder9;

 // class holding the adapters (needts to be a singleton) 

 // class FuncAdapterHolder { 

 //  public : 
     
 //    FuncAdapterHolder() {}
 //  virtual ~FuncAdapterHolder(); 



   }      // end of namespace iAIDA::AIDA_RootStore
}

#endif /* FUNCTIONCONVERTER */
