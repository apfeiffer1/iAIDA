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

#ifndef AIDA_Dev_IDEVFUNCTIONFACTORY_H
#define AIDA_Dev_IDEVFUNCTIONFACTORY_H 1

#include <string>
#include <vector>


namespace AIDA {
  class IFunction; 

  namespace Dev {

    class IDevFunction; 
    class IDevFunctionCatalog; 

/** 
 * @interface IDevFunctionFactory
 *
 * Developers interface for AIDA function.
 *
 * @author Jakub Moscicki, Max Turri, Victor Serbo
 * @author Andreas Pfeiffer, Max Sang, Tony Johnson, Lorenzo Moneta
 *
 * @stereotype developer
 */    

class IDevFunctionFactory 
  {
      public: 
  /// Destructor.
  virtual ~IDevFunctionFactory() { /* nop */; }

    /**
     * Create function from a name registered in the catalog.
     * This is the easiest way to create simple model functions for fitting.
     * Every AIDA compliant implementation should predefine "G", "E", "Pn"
     * (n is an integer, e.e "P0","P5"). Simple operations are permitted,
     * e.g. "G+P2".
     */
    virtual IDevFunction * createFunctionByName(std::string model) = 0;

    /**
     * Create function from script. Script conventions:
     * example:  createFunctionFromScript("f1", 2, "a*x[0]*x[0] + b*x[1]", "a,b", 
                                          "this is my function", "2*a*x[0],b")
     */
    virtual IDevFunction * createFunctionFromScript(int dim, std::string valexpr, std::string parameters, std::string description, std::string gradexpr = "") = 0;

    virtual IDevFunction * cloneFunction(IFunction * f) = 0;

    /**
     * get access to the function catalog
     */
    virtual IDevFunctionCatalog * catalog() = 0;


}; // class
} // namespace Dev
} // namespace AIDA
#endif 
