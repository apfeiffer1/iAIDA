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

#ifndef AIDAROOT_BASEHISTOGRAM3D_H
#define AIDAROOT_BASEHISTOGRAM3D_H 1

//  This file is part of the LCG PI library
//  Copyright (C) 2003 by the AIDA and LCG PI teams.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <vector>

#include "AIDA/IHistogram3D.h"

#include "AIDA_ROOT/Axis.h"

#include "AIDA_ROOT/BaseHistogram.h"

#include "AIDA_CPP/IHistogram3D.h"
#include "AIDA_CPP/DummyAnnotation.h"

#include "TH3D.h" 

namespace AIDA {
  class IAxis; // from AIDA                                                                                                                                          
}

namespace AIDA_ROOT {

  /**
   * common interface to 3D Histogram and Profile
   *
   * @param R the Root histogram
   * @param B the AIDA_CPP base (either  IHistogram3D or IProfile3D)
   * @author The AIDA and LCG PI teams (http://aida.freehep.org/)
   *
   */
  template<typename R, typename B>
  class  BaseHistogram3D  :  
    public virtual B, 
    public virtual BaseHistogram<R,typename B::Base>  {  
  public:
    typedef BaseHistogram3D<R,B> self;
    typedef B super;
    typedef typename super::Base AIDABase;
    typedef typename super::super AIDAsuper;
    typedef BaseHistogram<R,AIDABase> Base;
    typedef typename Base::REP REP;
  public:



    /// Destructor.
    virtual ~BaseHistogram3D(){}

  public: 

    int rIndexX(int index) const { return xAxis_.rIndex(index);}
    int rIndexY(int index) const { return yAxis_.rIndex(index);}
    int rIndexZ(int index) const { return zAxis_.rIndex(index);}

    // --------------------------------------------------------------------------------
    // from IHistogram3D & IProfile2D
    // --------------------------------------------------------------------------------

   /**
     * The weighted mean along the x axis of a given bin. 
     * NOTE: beamMean is not supported by Root, return beamCenterX
     */
    double binMeanX(int indexX, int , int ) const 
      { return (this->representation().GetXaxis())->GetBinCenter( rIndexX(indexX) ); } 

    /**
     * The weighted mean along the y axis of a given bin.
     * NOTE: beamMean is not supported by Root, return beamCentreY
     */
    double binMeanY(int, int indexY,int  ) const 
    { return (this->representation().GetYaxis())->GetBinCenter( rIndexY(indexY) ); } 

    /**
     * The weighted mean along the z axis of a given bin.
     * NOTE: beamMean is not supported by Root, return beamCentreZ
     */
    double binMeanZ(int , int ,int indexZ) const 
    { return (this->representation().GetYaxis())->GetBinCenter( rIndexY(indexZ) ); } 

    /**
     * Number of entries in the corresponding bin (ie the number of times fill was calle d for this bin).
     * NOTE: not implemented by Root, estimate assuming histogram has been filled 
     * with weights = 1    
     */
    int binEntries(int indexX, int indexY, int indexZ) const
      {
	if (binHeight(indexX, indexY, indexZ)<=0) return 0;
	double xx =  binHeight(indexX, indexY, indexZ)/binError(indexX, indexY, indexZ);
	return int(xx*xx+0.5);
    }

    /**
     * Sum of all the entries of the bins along a given x bin.
     */
    int binEntriesX(int index) const
      { 
	int n = 0; 
	for (int i = -2; i < yAxis().bins(); ++i) 
	  for (int j = -2; j < zAxis().bins(); ++j) 
	    n += binEntries(index,i,j); 
	return n; 
 
      }

    /**
     * Sum of all the entries of the bins along a given y bin.
     */
     int binEntriesY(int index) const
      { 
	int n = 0; 
	for (int i = -2; i < xAxis().bins(); ++i) 
	  for (int j = -2; j < zAxis().bins(); ++j) 
	    n += binEntries(i,index,j); 
	return n; 
      }

    /**
     * Sum of all the entries of the bins along a given z bin.
     */
     int binEntriesZ(int index) const
      { 
	int n = 0; 
	for (int i = -2; i < xAxis().bins(); ++i) 
	  for (int j = -2; j < yAxis().bins(); ++j) 
	    n += binEntries(i,j,index); 
	return n; 
      }

    /**
     * Total height of the corresponding bin (ie the sum of the weights in this bin).
     */
    double  binHeight ( int indexX, int indexY, int indexZ ) const  
      { return this->representation().GetBinContent ( rIndexX(indexX), rIndexY(indexY), rIndexZ(indexZ) ); }

    /**
     * Sum of all the heights of the bins along a given x bin.
     * This is equivalent to <tt>projectionX().binHeight(index)</tt>.
     */
    double binHeightX(int index) const 
      {    
	double s = 0; 
	for (int i = -2; i < yAxis().bins(); ++i) 
	  for (int j = -2; j < zAxis().bins(); ++j) 
	    s += binHeight(index,i,j); 
	return s; 
      }

    /**
     * Sum of all the heights of the bins along a given y bin.
     * This is equivalent to <tt>projectionY().binHeight(index)</tt>.
     */
    double binHeightY(int index) const
      { 
	double s = 0; 
	for (int i = -2; i < xAxis().bins(); ++i) 
	  for (int j = -2; j < zAxis().bins(); ++j) 
	    s += binHeight(i,index,j); 
	return s; 
      }

    /**
     * Sum of all the heights of the bins along a given z bin.
     * This is equivalent to <tt>projectionZ().binHeight(index)</tt>.
     */
    double binHeightZ(int index) const
      {    
	double s = 0; 
	for (int i = -2; i < xAxis().bins(); ++i) 
	  for (int j = -2; j < yAxis().bins(); ++j) 
	    s += binHeight(i,j,index); 
	return s; 
      }

    /**
     * The error of a given bin.
     */
    double  binError ( int indexX, int indexY, int indexZ ) const  
      { return this->representation().GetBinError ( rIndexX(indexX), rIndexY(indexY ), rIndexZ(indexZ ) ); }
 
    /**
     * The mean of the IHistogram3D along the x axis.
     */
    double  meanX (  ) const  { return this->representation().GetMean ( 1); }

    /**
     * The mean of the IHistogram3D along the y axis.
     */
    double  meanY (  ) const  { return this->representation().GetMean ( 2 ); }

    /**
     * The mean of the IHistogram3D along the z axis.
     */
    double  meanZ (  ) const  { return this->representation().GetMean ( 3 ); }

    /**
     * The RMS of the IHistogram3D along the x axis.
     */
    double  rmsX (  ) const  { return this->representation().GetRMS( 1 ); }

    /**
     * The RMS of the IHistogram3D along the y axis.
     */
    double  rmsY (  ) const  { return this->representation().GetRMS( 2 ); }

    /**
     * The RMS of the IHistogram3D along the z axis.
     */
    double  rmsZ (  ) const  { return this->representation().GetRMS( 3 ); }
 
 
    /**
     * Get the x axis of the IHistogram3D.
     */
    const AIDA_ROOT::Axis & xAxis (  ) const  { return xAxis_; }

    /**
     * Get the y axis of the IHistogram3D.
     */
    const AIDA_ROOT::Axis & yAxis (  ) const  { return yAxis_; }

    /**
     * Get the z axis of the IHistogram3D.
     */
    const AIDA_ROOT::Axis & zAxis (  ) const  { return zAxis_; }

    /**
     * Get the bin number corresponding to a given coordinate along the x axis.
     * This is a convenience method, equivalent to <tt>xAxis().coordToIndex(coord)</tt>.
    */
    int  coordToIndexX ( double coord ) const { return xAxis().coordToIndex(coord);}
    /**
     * Get the bin number corresponding to a given coordinate along the y axis.
     * This is a convenience method, equivalent to <tt>yAxis().coordToIndex(coord)</tt>.
    */
    int  coordToIndexY ( double coord ) const { return yAxis().coordToIndex(coord);}
    /**
     * Get the bin number corresponding to a given coordinate along the z axis.
     * This is a convenience method, equivalent to <tt>zAxis().coordToIndex(coord)</tt>.
    */
    int  coordToIndexZ ( double coord ) const { return zAxis().coordToIndex(coord);}

    /**
     * Add to this Histogram3D the contents of another IHistogram3D.
     */
    bool  add ( const AIDAsuper & hist ) {
      const self * it = dynamic_cast<const self*>(& hist);
      if (it) return Base::add(*it);
      else return false;
    }

    // overwrite extraentries 
    int extraEntries() const { 
	int n = 0; 
	for (int iX = -2; iX < xAxis().bins(); ++iX) 
	  for (int iY = -2; iY < yAxis().bins(); ++iY) 
	    n += binEntries(iX,iY,AIDA::IAxis::UNDERFLOW_BIN) + 
	      binEntries(iX,iY,AIDA::IAxis::OVERFLOW_BIN);
	// start from zero to avoid double counting (under/over flow in all coordinates are counted before)
 	for (int iX = 0; iX < xAxis().bins(); ++iX) 
	  for (int iZ = 0; iZ < zAxis().bins(); ++iZ) 
	    n += binEntries(iX,AIDA::IAxis::UNDERFLOW_BIN,iZ) + 
	    binEntries(iX,AIDA::IAxis::OVERFLOW_BIN,iZ);
 	for (int iY = 0; iY < yAxis().bins(); ++iY) 
	  for (int iZ = 0; iZ < zAxis().bins(); ++iZ) 
	    n += binEntries(AIDA::IAxis::UNDERFLOW_BIN,iY,iZ) + 
	      binEntries(AIDA::IAxis::OVERFLOW_BIN,iY,iZ);
	    
	return n; 
    }


    // --------------------------------------------------------------------------------
    // from IHistogram
    // --------------------------------------------------------------------------------


    /**
     * Get the Histogram's dimension.
     */ 
    int  dimension (  ) const  { return 3; }

 

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
    // non const methods 
    AIDA_ROOT::Axis & xAxis (  ) { return xAxis_; }
    AIDA_ROOT::Axis & yAxis (  ) { return yAxis_; }
    AIDA_ROOT::Axis & zAxis (  ) { return zAxis_; }
 

  private:
   AIDA_ROOT::Axis xAxis_;
   AIDA_ROOT::Axis yAxis_;
   AIDA_ROOT::Axis zAxis_;

   bool m_isUpToDate; 

  }; // end class IHistogram3D 

} // end namespace AIDA



#endif /* ifndef AIDAPROXY_HISTOGRAM3D_H */
