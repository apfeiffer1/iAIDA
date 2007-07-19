inline std::vector< std::string >
pi::AIDA_HBookStore::HBookPathFormater::formPathNames( const std::string& path ) const
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
  std::vector<std::string> loutput;
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
