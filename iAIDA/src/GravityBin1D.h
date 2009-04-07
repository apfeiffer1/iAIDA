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
#ifndef IAIDA_GRAVITYBIN1D
#define IAIDA_GRAVITYBIN1D 1

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Class defining a gravity bin for one-dimensional histograms

class GravityBin1D
{
public:
  /// Constructor
  GravityBin1D();
  /// Destructor
  ~GravityBin1D() { /*nop*/; }
  /** Main fill method.
    * The weight of the entry
    * and the x coordinate
    */
  bool fill( double weight, double coord );
  /// Reset method
  bool reset();
  /// Scale method
  bool scale( double factor );
  /// Increment methods
  bool increment( const GravityBin1D& otherBin );
  bool increment( int extraEntries,
		  double extraHeight,
                  double extraError,
                  double extraCentre );
  /// Set method
  bool set( int entriesNew,
	    double heightNew,
	    double errorNew,
            double centreNew );
  /// Returns the number of entries
  int entries() const;
  /// Returns the bin height
  double height() const;
  /// Returns the bin error
  double error() const;
  /// Returns the bin error squared
  double errorSquared() const;
  /// Returns the gravity point
  double centreOfGravityX() const;

private:
  int     m_entries;
  double  m_sumOfWeights;
  double  m_sumOfWeightTimesCoord;
  double  m_sumOfSquaredWeights;
};

  }
}

#include "GravityBin1D.inl"

#endif /* ifndef IAIDA_GRAVITYBIN1D */
