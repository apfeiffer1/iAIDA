# ifndef FMLLIB_Contour_h__included__
# define FMLLIB_Contour_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include <vector>

class IPoint2D;

namespace fml
{

// namespace FML {

/**
 * Contour. 
 * Like GEminiContour it supports by value semantics (assignement, copying, etc).
 * PENDING: not yet implemented
 */

class Contour
{
 public:

  Contour() {}

  const IPoint2D& point(int i) const;
  const IPoint2D& origin(int i) const;

  int nPoints() const;

  /// Problem with using IPoint2D in STL container (reference semantics)
  std::vector<double> xPointVector();
  std::vector<double> yPointVector();
};

// }

}

# endif
