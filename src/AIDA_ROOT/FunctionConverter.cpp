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

// Implementation file for class Histo1DTranslator
// 
// Created by: moneta  17/06/2003
// 
// 

#include "AIDA_ROOT/FunctionConverter.h"

#include "AIDA/IFunction.h"

#include "TF1.h"

#include <iostream>
// define function pointers
typedef double (* FP) (double * x, double *p);

static const std::string emptyString = "";

static int n_func_instance = 0; 

// need to define a global object (ugly !!!) and works only for one function
//

double function_adapter0(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<0> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder0::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter1(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<1> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder1::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter2(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<2> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder2::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter3(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<3> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder3::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter4(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<4> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder4::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter5(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<5> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder5::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter6(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<6> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder6::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter7(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<7> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder7::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter8(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<8> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder8::instance();
  return theFunctionAdapter(x,p); 
}
double function_adapter9(double *x, double *p) {
  iAIDA::AIDA_RootConverter::FuncAdapter<9> & theFunctionAdapter =  iAIDA::AIDA_RootConverter::FuncAdapterHolder9::instance();
  return theFunctionAdapter(x,p); 
}
  

iAIDA::AIDA_RootConverter::FunctionConverter::FunctionConverter(const iAIDA::AIDA_RootConverter::FunctionConverter &)  
{
}

iAIDA::AIDA_RootConverter::FunctionConverter & iAIDA::AIDA_RootConverter::FunctionConverter::operator = (const iAIDA::AIDA_RootConverter::FunctionConverter &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}



// implementation of copy to Root 



TF1 *  iAIDA::AIDA_RootConverter::FunctionConverter::toRoot(AIDA::IFunction & func, const std::string & name, double xmin, double xmax ) 
{ 
  TF1 * tf = 0;

  FP pFunc = 0; 
  iAIDA::AIDA_RootConverter::BaseFuncAdapter * theFunctionAdapter = 0; 
  // need to create a pointer to the AIDA function (use a global object)

  if (n_func_instance >= MAX_FUNC_ADAPTER) { 
    n_func_instance = 0;
    std::cout << "Warning - AIDA_RootConverter::FunctionConverter : max count exceeded- overwriting an existing function" << std::endl; 
  }
  switch(n_func_instance) {  
  case 0 : 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder0::instance();
    pFunc = function_adapter0; 
    break; 
  case 1 : 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder1::instance();
    pFunc = function_adapter1; 
    break; 
  case 2: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder2::instance();
    pFunc = function_adapter2; 
    break; 
  case 3: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder3::instance();
    pFunc = function_adapter3; 
    break; 
  case 4: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder4::instance();
    pFunc = function_adapter4; 
    break; 
  case 5: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder5::instance();
    pFunc = function_adapter5;
    break; 
  case 6: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder6::instance();
    pFunc = function_adapter6; 
    break; 
  case 7: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder7::instance();
    pFunc = function_adapter7; 
    break; 
  case 8: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder8::instance();
    pFunc = function_adapter8; 
    break; 
  case 9: 
    theFunctionAdapter =  &iAIDA::AIDA_RootConverter::FuncAdapterHolder9::instance();
    pFunc = function_adapter9; 
    break; 
  default : 
    return 0; 
  }
  n_func_instance++; 

  if (theFunctionAdapter) 
    theFunctionAdapter->setFunction(&func); 
  else 
    return 0; 


  tf = new TF1(name.c_str(),pFunc,xmin,xmax,func.numberOfParameters());
  // need to scopy the parameters 
  tf->SetParameters(&func.parameters().front()); 

  return tf; 

}

void iAIDA::AIDA_RootConverter::BaseFuncAdapter::setFunction(AIDA::IFunction * f)  
{ 
  m_func = f;
  m_par = m_func->parameters(); 
  m_var.resize(m_func->dimension()); 

}

double iAIDA::AIDA_RootConverter::BaseFuncAdapter::operator() (double *x, double *p) { 
  // set parameters 
  if ( p != 0) { 
    for (unsigned int i = 0; i < m_par.size() ; ++i) 
      m_par[i] = p[i]; 

    m_func->setParameters(m_par); 
  }

  // set variables 
  for (unsigned int i = 0; i < m_var.size() ; ++i) 
    m_var[i] = x[i]; 


  return m_func->value(m_var); 
}

