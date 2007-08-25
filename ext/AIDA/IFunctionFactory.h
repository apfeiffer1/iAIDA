// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_IFUNCTIONFACTORY_H
#define AIDA_IFUNCTIONFACTORY_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>

namespace AIDA {

class IFunction;
class IFunctionCatalog;

/** @interface IFunctionFactory
 *
 * Function factory.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 * @stereotype abstractfactory
 */
 
class IFunctionFactory {

public: 
    /// Destructor.
    virtual ~IFunctionFactory() { /* nop */; }

    /**
     * Create function from a model registered in the catalog.
     * This is the easiest way to create simple model functions for fitting.
     * Every AIDA compliant implementation should predefine "G", "E", "Pn"
     * (n is an integer, e.e "P0","P5"). Simple operations are permitted,
     * e.g. "G+P2".
     * @param path  The path of the IFunction. The path can either be a relative or full path.
     *              ("/folder1/folder2/functionName" and "../folder/functionName" are valid paths).
     *              All the directories in the path must exist. The characther `/` cannot be used
     *              in names; it is only used to delimit directories within paths.
     * @param model The model of the function to be created.
     * @return      The newly created function.
     *
     */
    virtual IFunction * createFunctionByName(const std::string & path, const std::string & model) = 0;

    /**
     * Create function from script. Script conventions:
     *         createFunctionFromScript("f1", 2, "a*x[0]*x[0] + b*x[1]", "a,b","this is my function", "x[0]*x[0],x[1]")
     * @param path        The path of the IFunction. The path can either be a relative or full path.
     *                    ("/folder1/folder2/functionName" and "../folder/functionName" are valid paths).
     *                    All the directories in the path must exist. The characther `/` cannot be used
     *                    in names; it is only used to delimit directories within paths.
     * @param dim         The dimension of the function, i.e. the number of variables.
     * @param valexpr     The expression of the scripted function.
     * @param parameters  A comma separeted list of what has to be considered as a parameter in the valexpr expression.
     * @param description The description of the function.
     * @param gradexpr    A comma separated list of the expressions for the derivatives of the function with respect to the parameters.
     * @return            The newly created function
     *                    provided expressions is illigal.
     *
     */
    virtual IFunction * createFunctionFromScript(const std::string & name, int dim, const std::string & valexpr, const std::string & parameters, const std::string & description, const std::string & gradexpr = "") = 0;

    /**
     * Create a clone of an existing function.
     * @param path The path of the IFunction. The path can either be a relative or full path.
     *             ("/folder1/folder2/functionName" and "../folder/functionName" are valid paths).
     *             All the directories in the path must exist. The characther `/` cannot be used
     *             in names; it is only used to delimit directories within paths.
     * @param f    The IFunction to be cloned.
     * @return     The clone of the provided IFunction.
     *
     */
    virtual IFunction * cloneFunction(const std::string & path, IFunction & f) = 0;

    /**
     * get access to the function catalog
     */
    virtual IFunctionCatalog * catalog() = 0;

    /** @link association 
     * @directed*/
    /*# IFunctionCatalog lnkIFunctionCatalog; */
}; // class
} // namespace AIDA
#endif /* ifndef AIDA_IFUNCTIONFACTORY_H */