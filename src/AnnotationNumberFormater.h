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
