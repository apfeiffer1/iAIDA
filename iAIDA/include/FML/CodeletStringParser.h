#ifndef CODELETSTRINGPARSER_H
#define CODELETSTRINGPARSER_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <string>
# include <vector>

/**
 * \file Codelet string parser.
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2002
 */

namespace fml
{
  // namespace FML
  // {
    /**
     * Parse the codelet string.
     * Generic format of the string:
     *
     * codelet:TYPE:LOCATION
     *
     * TYPE part contains only a 'typename' string
     * which is a metatype identifier (used in plugins and catalogues)
     *
     * LOCATION may be: 'file', 'verbatim', or 'catalogue'
     *
     * Detailed description:
     *
     * codelet:typename - no location specified (using 'catalogue')
     *
     * codelet:typename:file:/filename.cpp    -  a c++ source file
     * codelet:typename:file:/filename.py     -  a python source file
     * codelet:typename:file:/filename.so     -  a shared library
     *
     * codelet:typename:catalogue        - entry in a catalogue repository
     *
     * codelet:typename:verbatim:cpp \n  - c++ source file included verbatim 
     *                                     after \n
     *
     * codelet:typename:verbatim:py  \n  - python source file included 
     *                                     verbatim aftern \n
     *
     *
     * Comments:
     *
     * file:/filename is a relative path to $AIDA_FUNCTION_DEFAULT_DIR
     * file://filename is an absolute path
     *
     * Examples:
     *
     * codelet:G:catalogue
     * codelet:userSinus:verbatim:cpp \n { return sin(x); }
     */

    class CodeletStringParser
    {
    public:
      /**
       * False if string malformed (does not corrupt data members if error).
       */
      bool read(std::string s);

      /** 
       * Write formatted string.
       * Return empty string if errors.
       */

      std::string write();

      std::string typenameId; 
      std::string location; // file, verbatim, catalogue, [empty]
      std::string locationPath; // [empty], [path]
      std::string implementationType; // cpp, py, so
      std::string implementationBody; // verbatim only: body of implementation

      /// make all strings empty
      void clear();
    };

    /// Simple string tokenizer.
    /// If term = "" then take the whole string.
    std::vector<std::string> tokenize(std::string src, std::string delim, std::string term = "");
  // }
}

#endif
