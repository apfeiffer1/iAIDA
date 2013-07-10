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
#ifndef IAIDA_ANNOTATIONNUMBERFORMATER
#define IAIDA_ANNOTATIONNUMBERFORMATER 1

#include <string>

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Utility class to create well formated strings from numbers

class AnnotationNumberFormater
{
public:
  // Default number of precision digits
  static const int defaultPrecisionDigits = 4;

  AnnotationNumberFormater();
  ~AnnotationNumberFormater() {};
  std::string formatInteger( int number ) const;
  std::string formatDouble( double number ) const;

  void setPrecisionDigits( int digits );
  int precisionDigits() const;

private:
  int m_precisionDigits;

};

  }
}

/// a file scope singleton
static const iAIDA::AIDA_Histogram_native::AnnotationNumberFormater iAIDA_annotationNumberFormater;


#endif /* ifndef IAIDA_ANNOTATIONNUMBERFORMATER */
