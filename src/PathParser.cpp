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

#include "PathParser.h"
#include <vector>

iAIDA::aida_tree::PathParser::PathParser()
{}


iAIDA::aida_tree::PathParser::~PathParser()
{}


std::list< std::string >
iAIDA::aida_tree::PathParser::formNames( const std::string& path ) const
{
  std::vector<std::string> output;
  std::string currentString;
  for ( unsigned int iChar = 0; iChar < path.size(); ++iChar ) {
    const char& c = path[iChar];
    if ( c != '/' ) {
      currentString += c;
    }
    else {
      if ( currentString.size() > 0 ) {
        output.push_back( currentString );
        currentString = "";
      }
    }
  }
  if ( currentString.size() > 0 ) output.push_back( currentString );

  // Remove . or .. if they are in the middle.
  std::list<std::string> loutput;
  if ( output.empty() ) return loutput;
  loutput.push_back( output[0] );
  for( unsigned int i = 1; i < output.size(); ++i ) {
    const std::string& name = output[i];
    if ( name == "." ) continue;
    if ( name == ".." ) {
      if ( loutput.empty() ) return loutput;
      else loutput.pop_back();
    }
    else loutput.push_back( name );
  }

  return loutput;
}


std::string
iAIDA::aida_tree::PathParser::formPath( const std::list< std::string >& names ) const
{
  std::string output = "";
  for ( std::list< std::string >::const_iterator iName = names.begin(); iName != names.end(); ++iName ) {
    if ( output.size() > 0 ) output += "/";
    output += *iName;
  }
  return output;
}

std::string
iAIDA::aida_tree::PathParser::parent( const std::string& path ) const
{
  std::list< std::string > names = formNames( path );
  if ( names.size() > 0 ) names.pop_back();
  std::string par = formPath( names );
  if ( path.size() > 0 && path[0] == '/' ) par = "/" + par;
  return par;
}
