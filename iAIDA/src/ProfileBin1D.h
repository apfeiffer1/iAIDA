// Emacs -*- C++ -*-
#ifndef AIDANATIVE_PROFILEBIN1D
#define AIDANATIVE_PROFILEBIN1D 1

namespace iAIDA {
  namespace AIDA_Histogram_native {

/**
 * class holding the contents of a profile 1-dimensional histogram bin
 * Assume data points have `w as their weight and `k as their character value.
 * Assume:
 *  . l = SUM[ w ]
 *  . h = SUM[ w*k ]
 *  . e = SUM[ w*k*k ]
 *  . s = sqrt(fabs( e/l - (h/l)^2 )), the spread
 *
 * The value of this bin then is : h/l
 *
 * The error of the bin is
 *  . 0             if l==0
 *  . sqrt(h/(l*l)) if s==0
 *  . s/sqrt(l)     otherwise
 *
 * The spread of this bin is
 *  . 0             if l==0
 *  . sqrt(h/l)     if s==0
 *  . s             otherwise
 */

class ProfileBin1D
{
public:
  /// Default constructor
  ProfileBin1D();
  /// Destructor
  ~ProfileBin1D() {/*nop*/;}
  /// Filling method
  bool fill( double weight, double x, double y );
  /// Resets the contents of the bin
  bool reset();
  /// Scales the contents of the bin by the given factor
  bool scale( double scaleFactor );
  /// Increments the contents from the data of another bin
  bool increment( const ProfileBin1D & bin );
  /// Increments the contents given entries, value, error and spread
  bool increment( int extraEntries,
		  double extraValue,
		  double extraError,
		  double extraSpread,
                  double extraCentre );
  /// Sets the contents of the bin given all the parameters.
  bool set( int newEntries,
	    double newValue,
            double newError,
            double newSpread,
            double newCentre );
  /// Returns the number of entries
  int entries() const;
  /// Returns the value of the bin
  double value() const;
  /// Returns the error
  double error() const;
  /// Returns the spread
  double spread() const;
  /// Returns the gravity point
  double centreOfGravityX() const;
  /// Returns the sum of weights
  double sumOfWeights() const;

protected:

private:
  /// Number of entries
  int m_entries;
  /// Internal running sums
  double m_sumOfWeights;
  double m_sumOfWeightTimesExtraCoordinate;
  double m_sumOfWeightTimesSquaredExtraCoordinate;
  double m_sumOfWeightTimesCoordinate;
};

  }
}

#include "ProfileBin1D.inl"

#endif /* ifndef AIDANATIVE_PROFILEBIN */
