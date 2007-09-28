// -*- C++ -*-

#ifndef AIDAROOT_PROFILE2D_H
#define AIDAROOT_PROFILE2D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>
#include <cmath>

#include "AIDA/IProfile2D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram2D.h"

#include "AIDA_CPP/IProfile2D.h"

#include "TProfile2D.h" 

namespace AIDA_ROOT {

  /**
   * User level interface to 2D Histogram.
   *
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  class  Profile2D  : 
    public BaseHistogram2D<TProfile2D,AIDA_CPP::IProfile2D>   { 
  public:
    typedef Profile2D self;
    typedef BaseHistogram2D<TProfile2D,AIDA_CPP::IProfile2D> super;
    typedef super::REP REP;
  public:

    /// Constructors (signatures as from the IHistogramFactory::createHistogram2D methods

    /**
     * Create a Profile2D.
     */
    Profile2D();


    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options="");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="");

    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY,  double lowerValue, double upperValue, const std::string & options="");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY,  double lowerValue, double upperValue, const std::string & options="");

    /** 
     *	copy constructor
     */ 
    Profile2D(const Profile2D & rh);


    /** 
     *	copy constructor from AIDA_CPP interface
     */ 
    Profile2D(const AIDA_CPP::IProfile2D & h);

    /** 
     *	copy constructor from AIDA interface
     */ 
    Profile2D(const AIDA::IProfile2D & h);


    /// Destructor.
    virtual ~Profile2D();


    // --------------------------------------------------------------------------------
    // from IProfile2D
    // --------------------------------------------------------------------------------

    /**
     * Fill the Profile2D with a value and the
     * corresponding weight.
     * @param x      The value to be filled in.
     * @param weight The corresponding weight (by default 1).
     * @return false If the weight is <0 or >1 (?).
     *
     */
    bool  fill ( double x, double y, double z, double weight = 1. )   { representation().Fill ( x , y, z, weight ); return true;}
    
    int  binEntries ( int indexX, int indexY ) const  { 
      int rBin = representation().GetBin ( rIndexX(indexX) , rIndexY(indexY ) ); 
      return int( representation().GetBinEntries ( rBin ) + 0.5);
     }

    // spread 
    double binRms(int indexX, int indexY) const {
      return representation().GetBinError ( rIndexX(indexX), rIndexY(indexY) );
    }
    // error on the mean 
    double binError(int indexX, int indexY) const {
      if (binEntries(indexX, indexY) == 0) return 0; 
      return binRms(indexX, indexY)/std::sqrt( double(binEntries(indexX, indexY)) );
    }

    // overwrite reset
    bool  reset (  ); 


    /**
     *  Introspection method
     * @param className The name of the class to cast on.
     * @return The right pointer. Return 0 if failure.
     */ 
    void * cast(const std::string & className) const 
    {  
      if (className == "AIDA::IProfile2D") 
	return const_cast<AIDA_ROOT::Profile2D *>(this); 
      return 0;
    }


      /**
       * Fast filling method for a given bin.
       * It can be also the over/underflow bin
       */
    bool setBinContents( int binIndexX,
                               int binIndexY,
                               int entries,
                               double height,
                               double error,
                               double spread,
                               double centreX,
                               double centreY );

    /// The AIDA user-level unterface leaf class type
    const std::string& userLevelClassType() const { m_classType = "IProfile2D"; return m_classType; }

    /// operator methods 

     /**
     * Add to this Profile2D the contents of another IProfile2D.
     * @param hist The Profile2Dto be added to this IProfile2D.
     * @if binning are incompatble an exception should be throw
     *
     */
    Profile2D & operator += ( const AIDA::IProfile2D & hist )   { add ( hist );  return *this; }
   
    /**
     * Create a copy of an Profile2D  
     */
    Profile2D & operator  = ( const Profile2D & hist );

    /**
     * Create a copy of an IProfile2D  
     */
    Profile2D & operator  = ( const AIDA::IProfile2D & hist );

  protected: 
    void copyFromAida (const AIDA::IProfile2D & hist );
 
    private: 

   // class type 
    mutable std::string m_classType; 

    // cache sumEntries (allEntries)   when setting contents since Root can't compute by himself 
    int  m_sumEntries;


  }; // end class IProfile2D 

} // end namespace AIDA

 
//Profile2D operations 

/**
 * Adding of two Profile2D (implement in term of += )
 * Create an IProfile2D by adding two IProfile2D.
 * @param name  The name of the resulting IProfile2D.
 * @param h2    The first member of the addition.
 * @param h2    The second member of the addition.
 * @return      The sum of the two Profile2D.
 *
 */
const AIDA_ROOT::Profile2D operator + (const AIDA_ROOT::Profile2D & h1, const AIDA_ROOT::Profile2D & h2);
 

#endif // AIDAROOT_PROFILE2D_H
