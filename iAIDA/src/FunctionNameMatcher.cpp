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

# include "FML/FunctionNameMatcher.h"


# include <assert.h>


#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif


namespace fml
{

// namespace FML
// {

  FunctionNameMatcher::FunctionNameMatcher(std::string pattern) : m_pattern(pattern)
  {
  }

  FunctionNameMatcher::FunctionNameMatcher() {}

  void FunctionNameMatcher::setPattern(std::string pattern)
  {
    m_pattern = pattern;
  }
  

  bool FunctionNameMatcher::_split_and_subst()
  {
    m_split_buf.clear();

    std::string pat = m_pattern;

    while(1)
      {
	// find opening bracket
	std::string::size_type idx = pat.find_first_of("@$");

	// if not found, put the rest as verbatim and quit
	if(idx == std::string::npos)
	  {
	    m_split_buf.push_back(pat);
	    break;
	  }

	// put verbatim component if non-empty and move pat
	if(idx!=0)
	  {
	    m_split_buf.push_back(pat.substr(0,idx));
	    pat = pat.substr(idx);
	  }

	// opening bracket at character [0] now
	// find closing bracket
	idx = pat.find_first_of(pat[0],1);
	
	// unbalanced bracket or empty name inside (like $$ or @@)
	if(idx == std::string::npos || idx <= 1) return false;

	if(pat[0] == '@') // parameter so substitute it
	  m_split_buf.push_back(parameter(pat.substr(1,idx-1)));
	else
	  m_split_buf.push_back(pat); // leave variable as is

	pat = pat.substr(idx+1);	
      }
    
    
    return true;
  }

  bool FunctionNameMatcher::setParameter(std::string parName, std::string parVal)
  {
    m_par[parName] = parVal;

    return true;
  }

  std::string FunctionNameMatcher::parameter(std::string parName) const
  {
    return m_par[parName];
  }

  bool FunctionNameMatcher::match(std::string str)
  {
    if(!_split_and_subst()) return false;

    // str is shrinking with every iteration of the loop by the sucesfully
    // parsed component
    for(unsigned int i=0; i<m_split_buf.size(); ++i)
      {
	std::string cs = m_split_buf[i];

	if(cs.empty()) continue;

	if(cs[0] == '$')
	  {
	    // match the integer variable and set it

	    assert(cs.size()>2);

	    std::string vname = cs.substr(1,cs.size()-2);

	    int vval = 0;

	    std::istringstream buf(str.c_str());
	    bool ok = (buf >> vval);
	    if(!ok) return false;	    

	    // assign the rest of the string to str
	    char *new_str = new char[str.size()+2];

	    buf.get(new_str, str.size()+1, 0);
	    // PORTABILITY: readsome() not implemented in gcc2.95.2
	    //	    new_str[buf.readsome(new_str, str.size()+1)] = std::ends;
	    str = new_str;
	    delete [] new_str;
	    
	    m_intvar[vname] = vval;	    
	  }
	else
	  {
	    // match strings verbatim

	    if(cs == str.substr(0, cs.size()))
	      str = str.substr(cs.size());
	    else
	      return false; // mismatch
	  }
	
      }

    return str.empty(); // if not empty then remaining chars not matching    
  }
  
  int FunctionNameMatcher::intVariable(std::string varname)
  {
    return m_intvar[varname];
  }
 
// }
}
