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
#ifndef IAIDA_AIDABOUNDEDPROFILE2D
#define IAIDA_AIDABOUNDEDPROFILE2D 1

#include "AIDA_Profile2D.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Profile 2D histogram with boundaries on the y axis

class AIDA_BoundedProfile2D : public AIDA_Profile2D
{

public:
  /// Constructor for fixed-sized bin profile histogram
  AIDA_BoundedProfile2D( const std::string& title,
			 int numberOfBinsX,
			 double lowEdgeX,
			 double highEdgeX,
			 int numberOfBinsY,
			 double lowEdgeY,
			 double highEdgeY,
			 double lowBound,
			 double highBound );
  /// Constructor for variable-sized bin profile histogram
  AIDA_BoundedProfile2D( const std::string& title,
			 const std::vector< double >& edgesX,
			 const std::vector< double >& edgesY,
			 double lowBound,
			 double highBound );
  /// Copy constructor
  AIDA_BoundedProfile2D( const AIDA_BoundedProfile2D& h );
  /// Destructor
  virtual ~AIDA_BoundedProfile2D() {};

  /// Methods needed to be reimplemented
  virtual bool reset();
  virtual int extraEntries() const;
  virtual bool fill( double x, double y, double z, double weight = 1. ) ;
  virtual bool add( const AIDA::IProfile2D & h );

private:
  /// extra entries
  int                m_extraEntries;

  /// Bounds on extra coordinate
  double             m_lowBound;
  double             m_highBound;

  /// No asignment operator
  AIDA_BoundedProfile2D& operator=( const AIDA_BoundedProfile2D& );
};

  }
}

#endif /* ifndef IAIDA_AIDAPROFILE2D */
