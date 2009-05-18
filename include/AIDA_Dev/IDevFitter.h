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

// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_Dev_IDEVFITTER_H
#define AIDA_Dev_IDEVFITTER_H 1

#include "AIDA/IFitter.h"

namespace AIDA {
namespace Dev {

/** 
 * @interface IDevFitter
 *
 * Developer interface for AIDA::IFitter
 *
 * All the setters defined at user level.
 *
 * @author Jakub Moscicki, Max Turri, Victor Serbo
 * @author Andreas Pfeiffer, Max Sang, Tony Johnson, Lorenzo Moneta
 *
 * @stereotype developer
 */

class IDevFitter : virtual public AIDA::IFitter {

public: 
    /// Destructor.
    virtual ~IDevFitter() { /* nop */; }

  /**
   * Constants specifying the type of fit.
   *
   */
    enum { BINNED_FIT = 0, UNBINNED_FIT = 1 };
}; // class
} // namespace Dev
} // namespace AIDA
#endif /* ifndef AIDA_Dev_IDEVFITTER_H */