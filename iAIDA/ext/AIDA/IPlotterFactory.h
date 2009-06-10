// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_IPLOTTERFACTORY_H
#define AIDA_IPLOTTERFACTORY_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>

namespace AIDA {

class IAxisStyle;
class IDataStyle;
class IFillStyle;
class ILineStyle;
class IMarkerStyle;
class IPlotter;
class IPlotterStyle;
class ITextStyle;
class ITitleStyle;

/**
 * Factory for plotter and styles.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 */

class IPlotterFactory {

public: 
    /// Destructor.
    virtual ~IPlotterFactory() { /* nop */; }

   /** 
     * @param name  A string identifier. 
                    A plotter is in general managed by a GUI.
                    The "name" of the plotter could be used as 
                    the string identifier for the GUI plotter widget.
                    If "" is passed, some default plotter is used.
    */

    virtual IPlotter * create(const std::string & name = "") = 0;

   /** 
    * Create various basic styles. 
    * The returned styles are not managed by the plotter.
    * A user have to delete these objects.
    */
    virtual IMarkerStyle * createMarkerStyle() = 0;

    virtual ITextStyle * createTextStyle() = 0;

    virtual ILineStyle * createLineStyle() = 0;

    virtual IFillStyle * createFillStyle() = 0;

   /**
    * Create various "scene part" styles.
    * The returned styles are not managed by the plotter.
    */ 
    virtual IDataStyle * createDataStyle() = 0;

    virtual IAxisStyle * createAxisStyle() = 0;

    virtual ITitleStyle * createTitleStyle() = 0;

   /**
    * Create a global plotter style.
    * The returned style is not managed by the plotter.
    */ 
    virtual IPlotterStyle * createPlotterStyle() = 0;
}; // class
} // namespace AIDA
#endif /* ifndef AIDA_IPLOTTERFACTORY_H */