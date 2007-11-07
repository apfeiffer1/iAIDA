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

// Emacs -*- C++ -*-
#ifndef IAIDA_PROFILEBIN2D
#define IAIDA_PROFILEBIN2D 1

#include "ProfileBin1D.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

///Class defining a profile bin for two-dimensional histograms

class ProfileBin2D : public ProfileBin1D
{
public:
  /// Constructor
  ProfileBin2D();
  /// Destructor
  ~ProfileBin2D() { /*nop*/; }
  /** Main fill method.
    * The weight of the entry
    * and the x, y, z coordinates
    */
  bool fill( double weight, double coordX, double coordY, double coordZ );
  /// Reset method
  bool reset();
  /// Scale method
  bool scale( double factor );
  /// Increment methods
  bool increment( const ProfileBin2D& otherBin );
  bool increment( int extraEntries,
		  double extraHeight,
                  double extraError,
                  double extraSpread,
                  double extraCentreX,
                  double extraCentreY );
  /// Set method
  bool set( int entriesNew,
	    double heightNew,
	    double errorNew,
	    double spreadNew,
            double centreXNew,
            double centreYNew );
  /// Returns the gravity point along Y
  double centreOfGravityY() const;

private:
  double  m_sumOfWeightTimesCoordinate;
};

  }
}

#include "ProfileBin2D.inl"

#endif /* ifndef IAIDA_PROFILEBIN2D */
