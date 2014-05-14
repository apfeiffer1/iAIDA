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

#ifndef AIDAROOT_BASEHISTOGRAM1D_H
#define AIDAROOT_BASEHISTOGRAM1D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>

#include "AIDA/IHistogram1D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram.h"

#include "AIDA_CPP/IHistogram1D.h"
#include "AIDA_CPP/DummyAnnotation.h"

#include "TH1D.h" 

namespace AIDA {
  class IAxis; // from AIDA                                                                                                                                          
}

namespace AIDA_ROOT {

  /**
   * common interface to 1D Histogram and Profile
   *
   * @param R the Root histogram
   * @param B the AIDA_CPP base (either  IHistogram1D or IProfile1D)
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  template<typename R, typename B>
  class  BaseHistogram1D  :  
    public virtual B, 
    public virtual BaseHistogram<R,typename B::Base>  {  
  public:
    typedef BaseHistogram1D<R,B> self;
    typedef B super;
    typedef typename super::Base AIDABase;
    typedef typename super::super AIDAsuper;
    typedef BaseHistogram<R,AIDABase> Base;
    typedef typename Base::REP REP;
  public:



    /// Destructor.
    virtual ~BaseHistogram1D(){}

  public: 

    int rIndex(int index) const { return axis_.rIndex(index);}

    // --------------------------------------------------------------------------------
    // from IHistogram1D & IProfile1D
    // --------------------------------------------------------------------------------

 
    /**
     * The weighted mean of a bin. 
     * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
     * @return      The mean of the corresponding bin.
     *
     */
    double  binMean ( int index ) const  { return this->representation().GetBinCenter ( rIndex(index) ); }

    /**
     * Number of entries in the corresponding bin (ie the number of times fill was called for this bin).
     * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
     * @return      The number of entries in the corresponding bin. 
     *
     */
    int  binEntries ( int index ) const  { 
      if (binHeight(index)<=0) return 0;
      double xx =  binHeight(index)/binError(index);
      return int(xx*xx+0.5);
    }

    /**
     * Total height of the corresponding bin (ie the sum of the weights in this bin).
     * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
     * @return      The height of the corresponding bin.
     *
     */
    double  binHeight ( int index ) const  { return this->representation().GetBinContent ( rIndex(index) ); }

    /**
     * The error of a given bin.
     * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
     * @return      The error on the corresponding bin.
     *
     */
    double  binError ( int index ) const  { return this->representation().GetBinError ( rIndex(index) ); }

    /**
     * The mean of the whole IHistogram1D.
     * @return The mean of the IHistogram1D.
     *
     */
    double  mean (  ) const  { return this->representation().GetMean ( ); }

    /**
     * The RMS of the whole IHistogram1D.
     * @return The RMS if the IHistogram1D.
     *
     */
    double  rms (  ) const  { return this->representation().GetRMS(); }

    /**
     * Get the x axis of the IHistogram1D.
     * @return The x coordinate IAxis.
     *
     */
    const AIDA_ROOT::Axis & axis (  ) const  { return axis_; }

    /**
     * Get the bin number corresponding to a given coordinate along the x axis.
     * This is a convenience method, equivalent to <tt>axis().coordToIndex(coord)</tt>.
     * @see IAxis#coordToIndex(double)
     * @param coord The coordinalte along the x axis.
     * @return      The corresponding bin number.
     *
     */
    int  coordToIndex ( double coord ) const { return axis().coordToIndex(coord);}

    /**
     * Add to this Histogram1D the contents of another IHistogram1D.
     * @param hist The Histogram1D to be added to this IHistogram1D.
     * @return false If the IHistogram1Ds binnings are incompatible.
     *
     */
    bool  add ( const AIDAsuper & hist ) {
      const self * it = dynamic_cast<const self*>(& hist);
      if (it) return Base::add(*it);
      else return false;
    }

    // overwrite extraentries 
    int extraEntries() const { 
      return binEntries(AIDA::IAxis::UNDERFLOW_BIN) + binEntries(AIDA::IAxis::OVERFLOW_BIN); 
    }


    // --------------------------------------------------------------------------------
    // from IHistogram
    // --------------------------------------------------------------------------------


    /**
     * Get the Histogram's dimension.
     * @return The Histogram's dimension.
     *
     */ 
    int  dimension (  ) const  { return 1; }


    // from Dev interfaces

    /// object name 
    std::string name() const { return this->annotation().value(nameKey); }

    /// Checks if the object is up-to-date with respect to its stored image
    bool isUpToDate() const { return m_isUpToDate; } 

    /// The up-to-date flag
    void setUpToDate( bool isUpToDate ) { m_isUpToDate = isUpToDate; }
    
    /// Sets the name of the object
    bool setName( const std::string& newName ) { 
      this->representation().SetName(newName.c_str());
      this->annotation().setValue( nameKey, newName ); 
      return true;
    } 

    
 
  protected:
    AIDA_ROOT::Axis & axis (  ) { return axis_; }
 

  private:
   AIDA_ROOT::Axis axis_;

    bool m_isUpToDate; 
    

  }; // end class IHistogram1D 

} // end namespace AIDA



#endif /* ifndef AIDAPROXY_HISTOGRAM1D_H */
