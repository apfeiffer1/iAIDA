#include "AnnotationNumberFormater.h"

#ifdef OLDSTREAMS
# include <strstream>
# define ios_base ios
# define ostringstream ostrstream
#else
# include <sstream>
#endif

#include <iomanip>
#include <cmath>

iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::AnnotationNumberFormater():
  m_precisionDigits( iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::defaultPrecisionDigits )
{}

std::string
iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::formatInteger( int number ) const
{
  std::ostringstream os;
  os << number;
#ifndef BADENDS
  os << std::ends;
#endif
  return os.str();
}


std::string
iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::formatDouble( double number ) const
{
  const double n = std::abs( number );
  std::ostringstream os;
  if ( n < 0.1 ) {
    os << std::setiosflags(std::ios_base::scientific);
    os << std::setprecision(m_precisionDigits);
  }
  else if ( n < 10 ) {
    os << std::setiosflags(std::ios_base::fixed);
    os << std::setprecision(m_precisionDigits);
  }
  else if ( n < 100 ) {
    os << std::setiosflags(std::ios_base::fixed);
    os << std::setprecision(m_precisionDigits-1);
  }
  else {
    os << std::setiosflags(std::ios_base::scientific);
    os << std::setprecision(m_precisionDigits);
  };

  os << number;
#ifndef BADENDS
  os << std::ends;
#endif
  return os.str();
}


void
iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::setPrecisionDigits( int digits )
{
  m_precisionDigits = digits;
}


int
iAIDA::AIDA_Histogram_native::AnnotationNumberFormater::precisionDigits() const
{
  return m_precisionDigits;
}

