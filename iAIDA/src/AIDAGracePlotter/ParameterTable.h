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

/**
 *
 * \file Header file for class ParameterTable
 *
 * (c) Copyright CERN, Geneva
 *
 */

// 
// Created by user sang on Wed Aug  7 17:41:14 2002
// 

#ifndef AIDANATIVE_QPLOTTER_NEW_VIEWER_PARAMETERTABLE_H
#define AIDANATIVE_QPLOTTER_NEW_VIEWER_PARAMETERTABLE_H




#include <string>
#include <vector>
#include <map>
#include <utility>




namespace AidaNative {




/**
   ParameterTable is intended to be used as a lookup table (map) of available 
   parameter names and their possible "option values". It is basically a map
   of [string] [vector<string>, string] where the key is the parameter name, 
   and the value is a) the vector of strings is the list of possible values 
   of the parameter, and b) the default value. It is intended mainly as 
   a helper class for the XXXStyle classes. If the option list of a particular
   parameter is empty or contains a single empty element ("") then this signifies
   that the parameter may hold arbitrary values.
 */

class ParameterTable
{
public:
  ParameterTable();
  ParameterTable(const ParameterTable&);
  virtual ~ParameterTable();
  ParameterTable& operator=(const ParameterTable&);

public:
  /// Clear internal map
  void reset();
  /// \brief Add a parameter with its possible values ("" if arbitrary)
  /// and its default value (if appropriate).
  bool addParameter(const std::string& parameterName, 
		    const std::vector<std::string>& possibleOptions,
		    const std::string& defaultValue);
  /// \brief Copy the assigned default value to theValue. 
  /// Returns false if parameter is not known.
  bool getDefaultValue(const std::string& parameterName, 
		       std::string& theDefault) const;
  /// \brief Manually set the default value of a given parameter
  /// Returns false if parameter is not known.
  bool setDefaultValue(const std::string& parameterName,
		       const std::string& newDefault);
  /// Get list of all available parameters
  std::vector<std::string> availableParameters() const;
  /// \brief Get list of all available values of a given parameter.
  /// Returns an empty vector if arbitrary values acceptable.
  bool availableOptions(const std::string& param,
			std::vector<std::string>& theOptions) const;

private:
  typedef std::vector<std::string> StringList;
  typedef std::pair<StringList, std::string> ListAndDefault;
  typedef std::map<std::string, ListAndDefault> SMap;
  typedef SMap::iterator SMapIt;
  typedef SMap::const_iterator SMapCIt;

private:
  SMap theMap;
  
};



} // end of namespace AidaNative


#endif // AIDANATIVE_QPLOTTER_NEW_VIEWER_PARAMETERTABLE_H

