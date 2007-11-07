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
#ifndef IAIDA_AIDABASEHISTOGRAM
#define IAIDA_AIDABASEHISTOGRAM 1

#include "AIDA/IBaseHistogram.h"
#include "AIDA_Dev/IDevManagedObject.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/**
 * Implementation of the AIDA IBaseHistogram class
 *
 * Annotation items defined:
 *
 * "Title" (sticky), "Name" (sticky), "Entries"
 *
 */

class AIDA_BaseHistogram : virtual public AIDA::IBaseHistogram,
			   virtual public AIDA::Dev::IDevManagedObject
{
protected:
  /// Constructor
  AIDA_BaseHistogram( const std::string& title,
		      const std::string& classType,
		      int dimension );

public:
  /// Destructor
  virtual ~AIDA_BaseHistogram();

  /// Methods inherited from IBaseHistogram
  std::string title() const;
  bool setTitle( const std::string& title );
  AIDA::IAnnotation& annotation();
  const AIDA::IAnnotation& annotation() const;
  int dimension() const;

  /**
   * These are the methods that need to be defined in overloaded classes
   *
  virtual bool reset (  ) = 0;
  virtual int entries (  ) const = 0;
  */

  /// Methods inherited from IDevManagedObject
  bool isUpToDate() const;
  void setUpToDate( bool isUpToDate );
  bool setName( const std::string& newName );
  const std::string& userLevelClassType() const;
  
  /// Methods inherited from IManagedObject
  std::string name() const;

protected:
  /// Asks for the annotation without updating it
  AIDA::IAnnotation& annotationNoUpdate();
  const AIDA::IAnnotation& annotationNoUpdate() const;
  /// Method to be overloaded by the leaf classes.
  virtual void updateAnnotation() const;
  AIDA::IAnnotation* createAnnotation();

  /// check if a number is a NaN
  bool isNaN( double x); 

private:
  AIDA::IAnnotation*           m_annotation;
  std::string                  m_classType;
  int                          m_dimension;
  bool                         m_upToDate;
};

  }
}


/// Inline methods
#include "AIDA_BaseHistogram.inl"

#endif /* ifndef IAIDA_AIDABASEHISTOGRAM */
