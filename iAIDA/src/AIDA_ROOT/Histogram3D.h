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

#ifndef AIDAROOT_HISTOGRAM3D_H
#define AIDAROOT_HISTOGRAM3D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>

#include "AIDA/IHistogram3D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram3D.h"

#include "AIDA_CPP/IHistogram3D.h"

#include "TH3D.h" 

namespace AIDA_ROOT {

  /**
   * User level interface to 3D Histogram.
   *
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  class  Histogram3D  : 
    public BaseHistogram3D<TH3D,AIDA_CPP::IHistogram3D>   { 
  public:
    typedef Histogram3D self;
    typedef BaseHistogram3D<TH3D,AIDA_CPP::IHistogram3D> super;
    typedef super::REP REP;
  public:

    /// Constructors (signatures as from the IHistogramFactory::createHistogram3D methods

    /**
     * Create a IHistogram3D.
     */
    Histogram3D();

    /**
     * Create a IHistogram3D with fixed bins
     */
    Histogram3D(const std::string & title, int binsX=10, double iminX=0., double imaxX=1., int binsY=10, double iminY=0., double imaxY=1., int binsZ=10, double iminZ=0, double imaxZ=1, const std::string & options="");

    /**
     * Create a IHistogram3D with variable bins
     */
    Histogram3D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & options="");

    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, int binsZ, double iminZ, double imaxZ, const std::string & options="");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & options="");


    /** (const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="");
     *	copy constructor
     */ 
    Histogram3D(const Histogram3D & rh);


    /** 
     *	copy constructor from AIDA_CPP interface
     */ 
    Histogram3D(const AIDA_CPP::IHistogram3D & h);

    /** 
     *	copy constructor from AIDA interface
     */ 
    Histogram3D(const AIDA::IHistogram3D & h);


    /// Destructor.
    virtual ~Histogram3D();


    // --------------------------------------------------------------------------------
    // from IHistogram3D
    // --------------------------------------------------------------------------------

    /**
     * Fill the Histogram3D with a value and the
     * corresponding weight.
     * @param x      The value to be filled in.
     * @param weight The corresponding weight (by default 1).
     * @return false If the weight is <0 or >1 (?).
     *
     */
    bool  fill ( double x, double y, double z, double weight = 1. )   
      { representation().Fill ( x , y, z, weight ); return true;}
    


    /**
     *  Introspection method
     * @param className The name of the class to cast on.
     * @return The right pointer. Return 0 if failure.
     */ 
    void * cast(const std::string & className) const 
    {  
      if (className == "AIDA::IHistogram3D") 
	return const_cast<AIDA_ROOT::Histogram3D *>(this); 
      return 0;
    }



    /**
     * Fast filling method for a given bin.
     * It can be also the over/underflow bin
     */
    bool setBinContents( int binIndexX,
                               int binIndexY,
                               int binIndexZ,
                               int entries,
                               double height,
                               double error,
                               double centreX,
                               double centreY,
                               double centreZ );

  /**
   * Sets the rms of the histogram.
   * It should be called after all calls to setBinContents
   */
    bool setRms( double rmsX, double rmsY, double rmsZ );

    // overwrite reset
    bool  reset (  ); 

    /// The AIDA user-level unterface leaf class type
    const std::string& userLevelClassType() const { m_classType = "IHistogram3D"; return m_classType; }

    /// operator methods 

     /**
     * Add to this Histogram3D the contents of another IHistogram3D.
     * @param hist The Histogram3Dto be added to this IHistogram3D.
     * @if binning are incompatble an exception should be throw
     *
     */
    Histogram3D & operator += ( const AIDA::IHistogram3D & hist )   { add ( hist );  return *this; }
   
    /**
     * Create a copy of an Histogram3D  
     */
    Histogram3D & operator  = ( const Histogram3D & hist );

    /**
     * Create a copy of an IHistogram3D  
     */
    Histogram3D & operator  = ( const AIDA::IHistogram3D & hist );


  protected: 
    
    void copyFromAida( const AIDA::IHistogram3D & hist );
 

    // cache sumEntries  when setting contents since Root doesn't have bin entries
    int  m_sumEntries;
    // cache sumwx's  when setting contents since I don't have bin mean 
    double m_sumwx;
    double m_sumwy;
    double m_sumwz;
    // class type 
    mutable std::string m_classType; 

  

  }; // end class IHistogram3D 

} // end namespace AIDA

 
//Histogram3D operations 

/**
 * Adding of two Histogram3D (implement in term of += )
 * Create an IHistogram3D by adding two IHistogram3D.
 * @param name  The name of the resulting IHistogram3D.
 * @param h1    The first member of the addition.
 * @param h2    The second member of the addition.
 * @return      The sum of the two Histogram3D.
 *
 */
const AIDA_ROOT::Histogram3D operator + (const AIDA_ROOT::Histogram3D & h1, const AIDA_ROOT::Histogram3D & h2);
 

#endif /* ifndef AIDAPROXY_HISTOGRAM3D_H */
