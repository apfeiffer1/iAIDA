# ifndef RCS_util_h__included__
# define RCS_util_h__included__

#include <string>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif


namespace Util 
{

/**
 * convert value to string using stream operator <<.
 * consequently, type T must have operator << for ostream
 */

template<class T>
std::string to_string(const T& val)
{
  std::ostringstream buf;
  buf << val;

  // LM: not needed anymore ?? 
  // It causes problem with parameter names in sumof functions

// #ifndef BADENDS
//   buf << std::ends;
// #endif

  std::string ret = buf.str();
  return ret;
}


template<>
inline std::string to_string<std::string>(const std::string& s)
{
  return s;
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


template<>
inline bool to_value<std::string>(std::string s, std::string& dest)
{
  dest = s;
  return true;
}


/**
 * copy the contents of the istream into string,
 * return false if error occured
 */
bool copy_stream_to_string(std::istream& is, std::string &s);

}

# include "FML/Util/SimpleTokenizer.h"

# endif
