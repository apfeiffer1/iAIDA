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

#ifndef AIDAROOT_PROFILE1D_H
#define AIDAROOT_PROFILE1D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>
#include <cmath>

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram1D.h"

#include "AIDA_CPP/IProfile1D.h"

#include "TProfile.h" 

namespace AIDA_ROOT {

  /**
   * User level interface to 1D Histogram.
   *
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  class  Profile1D  : 
    public BaseHistogram1D<TProfile,AIDA_CPP::IProfile1D>   { 
  public:
    typedef Profile1D self;
    typedef BaseHistogram1D<TProfile,AIDA_CPP::IProfile1D> super;
    typedef super::REP REP;
  public:

    /// Constructors (signatures as from the IHistogramFactory::createHistogram1D methods

    /**
     * Create a Profile1D.
     */
    Profile1D();

    /**
     * Create a Profile1D.
     * @param title     The title of the IProfile1D.
     * @param nBins     The number of bins of the x axis.
     * @param lowerEdge The lower edge of the x axis.
     * @param upperEdge The upper edge of the x axis.
     * @param options   The options for the IProfile1D. The default is "".
     * @return          The newly created IProfile1D.
     *
     */
    Profile1D(const std::string & title, int bins=10, double imin=0., double imax=1., const std::string & options="");

    /**
     * Create a IProfile1D.
     * @param title     The title of the IProfile1D.
     * @param binEdges  The array of the bin edges for the x axis.
     * @param options   The options for the IProfile1D. The default is "".
     * @return          The newly created IProfile1D.
     *
     */
    Profile1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & options="");


    virtual void initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "");

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & options = "");

    virtual void initialize(const std::string & title, 
			    int nBins, double lowerEdge, double upperEdge, 
			    double lowerValue, double upperValue, 
			    const std::string & options = "");

    virtual void initialize(const std::string & title, 
			    const std::vector<double>  & binEdges,
			    double lowerValue, double upperValue, 
			    const std::string & options = "");
 
    /** 
     *	copy constructor
     */ 
    Profile1D(const Profile1D & rh);


    /** 
     *	copy constructor from AIDA_CPP interface
     */ 
    Profile1D(const AIDA_CPP::IProfile1D & h);

    /** 
     *	copy constructor from AIDA interface
     */ 
    Profile1D(const AIDA::IProfile1D & h);


    /// Destructor.
    virtual ~Profile1D();


    // --------------------------------------------------------------------------------
    // from IProfile1D
    // --------------------------------------------------------------------------------

    /**
     * Fill the Profile1D with a value and the
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
    
    int  binEntries ( int index ) const  { 
      return int(representation().GetBinEntries ( rIndex(index) )+0.5);
     }

    // spread
    double binRms(int index) const {
      return representation().GetBinError ( rIndex(index) );
    }

    // error on the mean 
    double binError(int index) const {
      if (binEntries(index) == 0) return 0; 
      return binRms(index)/std::sqrt( double(binEntries(index)) );
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
      if (className == "AIDA::IProfile1D") 
	return const_cast<AIDA_ROOT::Profile1D *>(this); 
      return 0;
    }



    /**
     * Fast filling method for a given bin.
     * It can be also the over/underflow bin
     */
    bool setBinContents( int binIndex,
                               int entries,
                               double height,
                               double error,
                               double spread,
                               double centre );

    /// The AIDA user-level unterface leaf class type
    const std::string& userLevelClassType() const { m_classType = "IProfile1D"; return m_classType; }
  
    // overwrite reset
    //bool  reset (  ); 

    /// operator methods 

     /**
     * Add to this Profile1D the contents of another IProfile1D.
     * @param hist The Profile1Dto be added to this IProfile1D.
     * @if binning are incompatble an exception should be throw
     *
     */
    Profile1D & operator += ( const AIDA::IProfile1D & hist )   { add ( hist );  return *this; }
   
    /**
     * Create a copy of an Profile1D  
     */
    Profile1D & operator  = ( const Profile1D & hist );

    /**
     * Create a copy of an IProfile1D  
     */
    Profile1D & operator  = ( const AIDA::IProfile1D & hist );

  protected: 
    void copyFromAida (const AIDA::IProfile1D & hist );

   private: 

   // class type 
    mutable std::string m_classType; 

    // cache sumEntries (allEntries)   when setting contents since Root can't compute by himself 
    int  m_sumEntries;

  }; // end class IProfile1D 

} // end namespace AIDA

 
//Profile1D operations 

/**
 * Adding of two Profile1D (implement in term of += )
 * Create an IProfile1D by adding two IProfile1D.
 * @param name  The name of the resulting IProfile1D.
 * @param h1    The first member of the addition.
 * @param h2    The second member of the addition.
 * @return      The sum of the two Profile1D.
 *
 */
const AIDA_ROOT::Profile1D operator + (const AIDA_ROOT::Profile1D & h1, const AIDA_ROOT::Profile1D & h2);
 

#endif // AIDAROOT_PROFILE1D_H
