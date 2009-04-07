// 
// Copyright (C) CERN, Geneva 
// 
// header file for class SimpleTokenizer 
// Created by: Andreas PFEIFFER  at Thu Apr 15 17:30:48 1999
// 
// Last update: 
//              
// 

#ifndef INCLUDED_SIMPLETOKENIZER_H
#define INCLUDED_SIMPLETOKENIZER_H

#include <string>
#include <vector>


/**
  Class to tokenize an STL string. 
  This class allows the user to tokenize (i.e. split in separate tokens)
  an STL string.
  The user can specify the set of delimiters as well as the terminator 
  character. The class splits the original string in a vector of tokens.
*/

namespace fml { 

class SimpleTokenizer  { 

public: 
  /// Constructor. Users can specify delimiters and terminator character.
  SimpleTokenizer(std::string delim=" ", std::string term="\n\0");
  /// Destructor
  virtual ~SimpleTokenizer(); 
private: 
  // inhibit use of these
  SimpleTokenizer(const SimpleTokenizer &); 
  SimpleTokenizer & operator = (const SimpleTokenizer &); 
public: 
  /// Redefine the set of delimiters
  void setDelimiters (std::string str) { delimiters  = str; }
  /// Redefine the set of terminators
  void setTerminators(std::string str) { terminators = str; }
  /// Split the string according to current delimiters/terminators
  std::vector<std::string > tokenize(std::string) const;
private:		// private attributes
  std::string delimiters;
  std::string terminators;
}; 


}
#endif /* INCLUDED_SIMPLETOKENIZER_H */ 
