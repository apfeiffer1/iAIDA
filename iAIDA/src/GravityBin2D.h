// Emacs -*- C++ -*-
#ifndef AIDANATIVE_GRAVITYBIN2D
#define AIDANATIVE_GRAVITYBIN2D 1

#include "GravityBin1D.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Class defining a gravity bin for two-dimensional histograms

class GravityBin2D : public GravityBin1D
{
public:
  /// Constructor
  GravityBin2D();
  /// Destructor
  ~GravityBin2D() { /*nop*/; }
  /** Main fill method.
    * The weight of the entry
    * and the x, y coordinates
    */
  bool fill( double weight, double coordX, double coordY );
  /// Reset method
  bool reset();
  /// Scale method
  bool scale( double factor );
  /// Increment methods
  bool increment( const GravityBin2D& otherBin );
  bool increment( int extraEntries,
		  double extraHeight,
                  double extraError,
                  double extraCentreX,
                  double extraCentreY );
  /// Set method
  bool set( int entriesNew,
	    double heightNew,
	    double errorNew,
            double centreXNew,
            double centreYNew );
  /// Returns the gravity point along Y
  double centreOfGravityY() const;

private:
  double  m_sumOfWeightTimesCoord;
};

  }
}

#include "GravityBin2D.inl"

#endif /* ifndef AIDANATIVE_GRAVITYBIN2D */
