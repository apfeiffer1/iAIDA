// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_Dev_IDEVMODELFUNCTION_H
#define AIDA_Dev_IDEVMODELFUNCTION_H 1

#include "AIDA/IModelFunction.h"
#include "AIDA_Dev/IDevFunction.h"

namespace AIDA {
namespace Dev {

/** 
 * @interface IDevModelFunction
 *
 * Developers interface for AIDA model function.
 *
 * @author Jakub Moscicki, Max Turri, Victor Serbo
 * @author Andreas Pfeiffer, Max Sang, Tony Johnson, Lorenzo Moneta
 *
 * @stereotype developer
 */    
class IDevModelFunction : virtual public AIDA::IModelFunction, virtual public IDevFunction {

public: 
    /// Destructor.
    virtual ~IDevModelFunction() { /* nop */; }

  /// Fails if you try to set (NOT provides AND is_normalized).
    virtual bool setNormalization(bool provides, bool is_normalized) = 0;

    virtual void setProvidesParameterGradient(bool yes) = 0;
}; // class
} // namespace Dev
} // namespace AIDA
#endif /* ifndef AIDA_Dev_IDEVMODELFUNCTION_H */
