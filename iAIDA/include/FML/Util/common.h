# ifndef FMLLIB_common_h__included__
# define FMLLIB_common_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

// This is auxiliary header file for common declarations.

# include <vector>

# include <string>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

# include "FML/Util/debug.h"

namespace fml
{

namespace FML
{

/// Empty vector buffer.
extern std::vector<double> empty_vector;

/**
 * convert value to string using stream operator <<.
 * consequently, type T must have operator << for ostream
 */

template<class T>
std::string to_string(T val)
{
  std::ostringstream buf;
  buf << val;
#ifndef BADENDS
  buf << std::ends;
#endif
  std::string ret = buf.str();
  return ret;
}

/**
 * convert string to value using stream operator >>.
 * consequently, type T must have operator >> for istream
 */

template<class T>
bool to_value(std::string s, T& val)
{
  std::istringstream buf(s.c_str());
  buf >> std::ws;
  return buf >> val;
}

}
}

#endif
