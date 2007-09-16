// -*- C++ -*-

#ifndef AIDAROOT_HISTOGRAM2D_H
#define AIDAROOT_HISTOGRAM2D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>

#include "AIDA/IHistogram2D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram2D.h"

#include "AIDA_CPP/IHistogram2D.h"

#include "TH2D.h" 

namespace AIDA_ROOT {

  /**
   * User level interface to 2D Histogram.
   *
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  class  Histogram2D  : 
    public BaseHistogram2D<TH2D,AIDA_CPP::IHistogram2D>   { 
  public:
    typedef Histogram2D self;
    typedef BaseHistogram2D<TH2D,AIDA_CPP::IHistogram2D> super;
    typedef super::REP REP;
  public:

    /// Constructors (signatures as from the IHistogramFactory::createHistogram2D methods

    /**
     * Create a IHistogram2D.
     */
    Histogram2D();

    /**
     * Create a IHistogram2D with fixed bins
     */
    Histogram2D(const std::string & title, int binsX=10, double iminX=0., double imaxX=1., int binsY=10, double iminY=0., double imaxY=1., const std::string & options="");

    /**
     * Create a IHistogram2D with variable bins
     */
    Histogram2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="");

    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options="");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="");


    /** (const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="");
     *	copy constructor
     */ 
    Histogram2D(const Histogram2D & rh);


    /** 
     *	copy constructor from AIDA CPP 
     */ 
    Histogram2D(const AIDA_CPP::IHistogram2D & h);

    /** 
     *	copy constructor from AIDA interface
     */ 
    Histogram2D(const AIDA::IHistogram2D & h);


    /// Destructor.
    virtual ~Histogram2D();


    // --------------------------------------------------------------------------------
    // from IHistogram2D
    // --------------------------------------------------------------------------------

    /**
     * Fill the Histogram2D with a value and the
     * corresponding weight.
     * @param x      The value to be filled in.
     * @param weight The corresponding weight (by default 1).
     * @return false If the weight is <0 or >1 (?).
     *
     */
    bool  fill ( double x, double y, double weight = 1. )   
    {
      if (weight == 1.) 
	representation().Fill ( x, y ); 
      else 
	representation().Fill ( x , y, weight );
      return true;
    }
 

    /**
     *  Introspection method
     * @param className The name of the class to cast on.
     * @return The right pointer. Return 0 if failure.
     */ 
    void * cast(const std::string & className) const 
    {  
      if (className == "AIDA::IHistogram2D") 
	return const_cast<AIDA_ROOT::Histogram2D *>(this); 
      return 0;
    }

    /// from Dev interfaces

  /**
   * Fast filling method for a given bin.
   * It can be also the over/underflow bin
   */
    virtual bool setBinContents( int binIndexX,
                               int binIndexY,
                               int entries,
                               double height,
                               double error,
                               double centreX,
                               double centreY );
    /**
     * Sets the rms of the histogram.
     * It should be called after all calls to setBinContents
     */

    bool setRms( double rmsX, double rmsY );
    // overwrite reset
    bool  reset (  ); 

   /// The AIDA user-level unterface leaf class type
    const std::string& userLevelClassType() const { m_classType = "IHistogram2D"; return m_classType; }
    
    /// operator methods 

     /**
     * Add to this Histogram2D the contents of another IHistogram2D.
     * @param hist The Histogram2Dto be added to this IHistogram2D.
     * @if binning are incompatble an exception should be throw
     *
     */
    Histogram2D & operator += ( const AIDA::IHistogram2D & hist )   { add ( hist );  return *this; }
   
    /**
     * Create a copy of an Histogram2D  
     */
    Histogram2D & operator  = ( const Histogram2D & hist );

    /**
     * Create a copy of an IHistogram2D  
     */
    Histogram2D & operator  = ( const AIDA::IHistogram2D & hist );


  protected: 
    
    void copyFromAida( const AIDA::IHistogram2D & hist );

  private: 

     // cache sumEntries  when setting contents since Root doesn't have bin entries
    int  m_sumEntries;
   // cache sumwx and sumwy  when setting contents since I don't have bin mean 
    double m_sumwx;
    double m_sumwy;
    // class type 
    mutable std::string m_classType; 

 
 

  }; // end class IHistogram2D 

} // end namespace AIDA

 
//Histogram2D operations 

/**
 * Adding of two Histogram2D (implement in term of += )
 * Create an IHistogram2D by adding two IHistogram2D.
 * @param name  The name of the resulting IHistogram2D.
 * @param h1    The first member of the addition.
 * @param h2    The second member of the addition.
 * @return      The sum of the two Histogram2D.
 *
 */
const AIDA_ROOT::Histogram2D operator + (const AIDA_ROOT::Histogram2D & h1, const AIDA_ROOT::Histogram2D & h2);
 

#endif /* ifndef AIDAPROXY_HISTOGRAM2D_H */
