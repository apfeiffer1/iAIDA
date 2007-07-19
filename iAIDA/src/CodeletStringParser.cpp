# include "FML/CodeletStringParser.h"

namespace fml
{
  // namespace FML
  // {

    std::vector<std::string> tokenize(std::string src, std::string delim, std::string term)
    {
      if(term != "")
	{
	  src = src.substr(0, src.find_first_of(term));
	}

      std::vector<std::string> v;

      std::string::size_type begIdx, endIdx;

      begIdx = src.find_first_not_of(delim);
      
      while(begIdx != std::string::npos)
	{
	  endIdx = src.find_first_of(delim, begIdx);

	  if(endIdx == std::string::npos)
	    endIdx = src.length();

	  v.push_back(src.substr(begIdx, endIdx-begIdx));

	  begIdx = src.find_first_not_of(delim, endIdx);
	}

      return v;
    }

    bool CodeletStringParser::read(std::string s)
    {
      // parse at most the first line
      std::vector<std::string> v(tokenize(s,":","\n"));

      if(v.size() < 2) return false;

      if(v[0] != "codelet") return false;


      std::string auto_typenameId; 
      std::string auto_location; 
      std::string auto_locationPath; 
      std::string auto_implementationType;
      std::string auto_implementationBody;

      auto_typenameId = v[1];

      if(auto_typenameId=="") return false;

      if(v.size() == 2)
	{
	  clear();
	  typenameId = auto_typenameId;
	  return true;
	}

      auto_location = v[2];

      if(auto_location == "catalogue")
	if(v.size() != 3) 
	  return false;
	else
	  {
	  clear();
	  typenameId = auto_typenameId;
	  location = auto_location;
	  return true;
	  }
      
      if(auto_location == "file")
	if(v.size() != 4)
	  return false;
	else
	  {
	    clear();
	    typenameId = auto_typenameId;
	    location = auto_location;
	    locationPath = v[3];
	    
	    std::string::size_type extpos = locationPath.find_last_of('.');
	    
	    if(extpos != std::string::npos)
	      implementationType = locationPath.substr(extpos+1);
	    
	    return true;
	  }
      
      if(auto_location == "verbatim")
	if(v.size() != 4)
	  return false;
      else
	{
	  clear();

	  typenameId = auto_typenameId;
	  location = auto_location;
	  implementationType = v[3];

	  std::string::size_type begIdx = s.find_first_of('\n');

	  if(begIdx != std::string::npos)
	    implementationBody = s.substr(begIdx);

	  return true;
	}

      return false;
    }
    
    void CodeletStringParser::clear()
    {
      typenameId = 
	location = 
	locationPath =  
	implementationType = 
	implementationBody = "";
      
    }

    std::string CodeletStringParser::write()
    {
      std::string buf;
      buf += "codelet:" + typenameId;

      if(location != "")
	buf += ":" + location;
      
      if(location == "file")
	buf += ":"+locationPath;
      
      if(location == "verbatim")
	buf += ":"+implementationType + "\n" + implementationBody;
      
      return buf;
    }
    
  // }
}
