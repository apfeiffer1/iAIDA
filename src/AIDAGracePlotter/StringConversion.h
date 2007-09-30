// -*- C++ -*-

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  This library is free software; you can redistribute it and/or      //
//  modify it under the terms of the GNU Lesser General Public         //
//  License as published by the Free Software Foundation; either       //
//  version 2.1 of the License, or (at your option) any later version. //
//                                                                     //
//  This library is distributed in the hope that it will be useful,    //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of     //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   //
//  Lesser General Public License for more details.                    //
//                                                                     //
//  You should have received a copy of the GNU Lesser General Public   //
//  License along with this library (see file COPYING); if not, write  //
//  to the Free Software Foundation, Inc., 59 Temple Place, Suite      //
//  330, Boston, MA 02111-1307 USA, or contact the author.             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifndef IAIDA_GRACE_STRING_CONVERTER_H
#define IAIDA_GRACE_STRING_CONVERTER_H

/*
 * \file
 *
 * conversions between std::string and any streamable type
 * using operators << and >>
 *
 * \author Jakub T. Moscicki, CERN 2001
 */



#include <string>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif


namespace AidaNative {



/**
 * Convert value to string using stream operator <<.
 * Type T must have operator << for ostream.
 */

template<class T>
std::string to_string(const T& val)
{
  std::ostringstream buf;
  buf << val;
//-ap #ifndef BADENDS
//-ap   buf << std::ends;
//-ap #endif
  std::string ret = buf.str();
  return ret;
}




/**
 * Convert string to value using stream operator >>.
 * Type T must have operator >> for istream.
 */

template<class T>
bool to_value(std::string s, T& val)
{
  std::istringstream buf( s.c_str() );
  return buf >> val;
}


} // end of namespace AidaNative


# endif // IAIDA_GRACE_STRING_CONVERTER_H
