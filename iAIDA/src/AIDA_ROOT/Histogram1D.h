// -*- C++ -*-

#ifndef AIDAROOT_HISTOGRAM1D_H
#define AIDAROOT_HISTOGRAM1D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>

#include "AIDA/IHistogram1D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram1D.h"

#include "AIDA_CPP/IHistogram1D.h"

#include "TH1D.h" 

namespace AIDA_ROOT {

  /**
   * User level interface to 1D Histogram.
   *
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  class  Histogram1D  : 
    public BaseHistogram1D<TH1D,AIDA_CPP::IHistogram1D>   { 
  public:
    typedef Histogram1D self;
    typedef BaseHistogram1D<TH1D,AIDA_CPP::IHistogram1D> super;
    typedef super::REP REP;
  public:

    /// Constructors (signatures as from the IHistogramFactory::createHistogram1D methods

    /**
     * Create a IHistogram1D.
     */
    Histogram1D();

    /**
     * Create a IHistogram1D.
     * @param title     The title of the IHistogram1D.
     * @param nBins     The number of bins of the x axis.
     * @param lowerEdge The lower edge of the x axis.
     * @param upperEdge The upper edge of the x axis.
     * @param options   The options for the IHistogram1D. The default is "".
     * @return          The newly created IHistogram1D.
     *
     */
    Histogram1D(const std::string & title, int bins=10, double imin=0., double imax=1., const std::string & options="");

    /**
     * Create a IHistogram1D.
     * @param title     The title of the IHistogram1D.
     * @param binEdges  The array of the bin edges for the x axis.
     * @param options   The options for the IHistogram1D. The default is "".
     * @return          The newly created IHistogram1D.
     *
     */
    Histogram1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & options="");


    virtual void initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & options = "");


    /** 
     *	copy constructor
     */ 
    Histogram1D(const Histogram1D & rh);


    /** 
     *	copy constructor from dev interface
     */ 
    Histogram1D(const AIDA_CPP::IHistogram1D & h);

    /** 
     *	copy constructor from AIDA interface
     */ 
    Histogram1D(const AIDA::IHistogram1D & h);


    /// Destructor.
    virtual ~Histogram1D();


    // --------------------------------------------------------------------------------
    // from IHistogram1D
    // --------------------------------------------------------------------------------

    /**
     * Fill the Histogram1D with a value and the
     * corresponding weight.
     * @param x      The value to be filled in.
     * @param weight The corresponding weight (by default 1).
     * @return false If the weight is <0 or >1 (?).
     *
     */
    bool  fill ( double x, double weight = 1. )   
    { 
      if (weight == 1.) 
	representation().Fill ( x );       
      else 
	representation().Fill ( x , weight );

      return true;
    }


    /**
     *  Introspection method
     * @param className The name of the class to cast on.
     * @return The right pointer. Return 0 if failure.
     */ 
    void * cast(const std::string & className) const 
    {  
      if (className == "AIDA::IHistogram1D") 
	return const_cast<AIDA_ROOT::Histogram1D *>(this); 
      return 0;
    }


    /// from Dev interfaces

      /**
       * Fast filling method for a given bin.
       * It can be also the over/underflow bin
       */
    bool setBinContents( int binIndex,
                               int entries,
                               double height,
                               double error,
                               double centre );
    /**
     * Sets the rms of the histogram.
     * It should be called after all calls to setBinContents
     */
    bool setRms( double rms );


    // for setting statistics since no bean mean and entries are available) 
    bool setStatistics( int allEntries, double eqBinEntries, double mean, double rms  );

    /// The AIDA user-level unterface leaf class type
    const std::string& userLevelClassType() const { m_classType = "IHistogram1D"; return m_classType; }
  
    // overwrite reset
    bool  reset (  ); 
  
    /// operator methods 

     /**
     * Add to this Histogram1D the contents of another IHistogram1D.
     * @param hist The Histogram1Dto be added to this IHistogram1D.
     * @if binning are incompatble an exception should be throw
     *
     */
    Histogram1D & operator += ( const AIDA::IHistogram1D & hist )   { add ( hist );  return *this; }
   
    /**
     * Create a copy of an Histogram1D  
     */
    Histogram1D & operator  = ( const Histogram1D & hist );

    /**
     * Create a copy of an IHistogram1D  
     */
    Histogram1D & operator  = ( const AIDA::IHistogram1D & hist );


  protected: 
    
    void copyFromAida( const AIDA::IHistogram1D & hist );
 
  private: 

    // cache sumEntries  when setting contents since Root doesn't have bin entries
    int  m_sumEntries;
     // cache sumwx  when setting contents since I don't have bin mean 
    double m_sumwx;
   // class type 
    mutable std::string m_classType; 
    

 

  }; // end class IHistogram1D 

} // end namespace AIDA

 
//Histogram1D operations 

/**
 * Adding of two Histogram1D (implement in term of += )
 * Create an IHistogram1D by adding two IHistogram1D.
 * @param name  The name of the resulting IHistogram1D.
 * @param h1    The first member of the addition.
 * @param h2    The second member of the addition.
 * @return      The sum of the two Histogram1D.
 *
 */
const AIDA_ROOT::Histogram1D operator + (const AIDA_ROOT::Histogram1D & h1, const AIDA_ROOT::Histogram1D & h2);
 

#endif /* ifndef AIDAPROXY_HISTOGRAM1D_H */
