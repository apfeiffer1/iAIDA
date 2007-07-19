// 
// Copyright (C) CERN, Geneva 
// 
// implementation file for class SimpleTokenizer 
// 
// Created by: Andreas PFEIFFER
// 
// Last update: 
// 

#ifndef INCLUDED_SIMPLETOKENIZER_H
#include "FML/Util/SimpleTokenizer.h"
#endif

namespace fml { 

SimpleTokenizer::~SimpleTokenizer() 
{
}
SimpleTokenizer::SimpleTokenizer(std::string delim, std::string term) : delimiters(delim), terminators(term) 
{
}

SimpleTokenizer::SimpleTokenizer(const SimpleTokenizer &) 
{
}

SimpleTokenizer & SimpleTokenizer::operator = (const SimpleTokenizer &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

// clean up old object, prepare new, copy over ...
   return *this;
}

std::vector<std::string> SimpleTokenizer::tokenize(std::string input) const 
{
  std::vector<std::string> tokens;

  int endOfSearch = input.size(); // pre-set
  if (terminators.size() != 0) {	  // iterate to find last pos to check
    std::string::size_type pos = input.find_first_of(terminators);
    if (pos != std::string::npos) endOfSearch = pos;
  }
  
  std::string newInput = input.substr(0, endOfSearch);

  std::string::size_type startPos = 0;
  std::string::size_type newPos   = 0;
  while (startPos != std::string::npos) {
    newPos = newInput.find_first_of(delimiters, startPos);
    if (newPos != std::string::npos) {
      tokens.push_back( newInput.substr( startPos, newPos-startPos ) );
      newPos++;
    } else {			// scanned up to terminator
	tokens.push_back( newInput.substr( startPos, endOfSearch-startPos ) );
    }
    startPos = newPos;
  }

  // I'm sacrificing efficiency for clarity here ... but since this class is 
  // basically only used in the context of interactive work, speed should
  // not be a critical issue.

  std::vector<std::string> newTokens;
  std::vector<std::string>::iterator i;
  i = tokens.begin();
  while(i != tokens.end()) { // clear out empty (i.e. only-space) tokens
    if (! ( (i->find_first_not_of(" ") == std::string::npos) || ((*i).size() == 0) ) ) {
      newTokens.push_back(*i);	// add to new vector
    }
    i++;
  }

#ifdef DEBUG_PRINT
  i = newTokens.begin();
  std::cerr << "found " << newTokens.size() << " newTokens " << endl;
  while(i != newTokens.end()) {
    std::cerr << "token = '" << *i << "' size:" << (*i).size() << endl;
    i++;
  }
#endif
  return newTokens;
}


}  // end namespace fml
