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
