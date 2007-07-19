// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_Dev_IDEVFITRESULT_H
#define AIDA_Dev_IDEVFITRESULT_H 1

#include <string>
#include <vector>

#include "AIDA/IFitParameterSettings.h"
#include "AIDA/IFitResult.h"
#include "AIDA/IFunction.h"

namespace AIDA {
namespace Dev {

/**
 * @interface IDevFitResult
 *
 * Represents the result of the fit.
 *
 * Holds internal copies of fit parameter settings (set in IFitter) and the
 * fitted function (clone of original function). The value of the parameters
 * of the fitted function are obtained from the fitting.
 *
 * @author Jakub Moscicki, Max Turri, Victor Serbo
 * @author Andreas Pfeiffer, Max Sang, Tony Johnson, Lorenzo Moneta
 *
 * @stereotype developer
 */

class IDevFitResult : virtual public AIDA::IFitResult {

public: 
    /// Destructor.
    virtual ~IDevFitResult() { /* nop */; }

    virtual void setIsValid(bool yes) = 0;

    virtual void setFitStatus(int status) = 0;

    /**
     * Set dynamically allocated function object (new).
     * IDevFitResult object takes ownership and should properly delete it.
     * As a caller of this method you should not manually
     * delete f after you set it here.
     */
    virtual void setFittedFunction(AIDA::IFunction * f) = 0;

    virtual void setQuality(double quality) = 0;

    virtual void setNdf(int ndf) = 0;

    /// Set covariance matrix. PENDING: interface for matrix to be defined.
    virtual void setCovMatrixElement(int i, int j, double val) = 0;

    virtual void setFitMethodName(std::string name) = 0;

    /// Engine used to do fitting.
    virtual void setEngineName(std::string name) = 0;

    /// PENDING: the contents and format of this string to be defined.
    /// PENDING: XML? stor name and path? ranges of DataPipe?
    virtual void setDataDescription(std::string descr) = 0;

    virtual void setConstraints(const std::vector<std::string>  & cv) = 0;

    /**
     * Set a dynamically allocated object (created with new).
     * This class takes ownership and should properly delete it.
     * As a caller of this method you should not manually
     * delete 's' object after you set it here.
     */
    virtual void setFitParameterSettings(std::string name, AIDA::IFitParameterSettings * s) = 0;
}; // class
} // namespace Dev
} // namespace AIDA
#endif /* ifndef AIDA_Dev_IDEVFITRESULT_H */
