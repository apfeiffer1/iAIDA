// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_ITITLESTYLE_H
#define AIDA_ITITLESTYLE_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include "AIDA/IBaseStyle.h"

namespace AIDA {

class ITextStyle;

/**
 * Style for the "title scene part" of a plotting region or
 * for the global title of a plotting page.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 */

class ITitleStyle : virtual public IBaseStyle {

public: 
    /// Destructor.
    virtual ~ITitleStyle() { /* nop */; }

    virtual ITextStyle & textStyle() = 0;

    virtual bool setTextStyle(const ITextStyle & textStyle) = 0;
}; // class
} // namespace AIDA
#endif /* ifndef AIDA_ITITLESTYLE_H */
