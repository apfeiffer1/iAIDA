#ifndef FML_FUNCTIONNAMEMATCHER_H
#define FML_FUNCTIONNAMEMATCHER_H

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
# include <map>
# include <vector>

namespace fml
{
// namespace FML
// {
  /**
   * Detect and match function name patterns.
   *
   * A pattern may be a string like: "freetext@name@$i2$".
   * @name@ is a string parameter called "name" which must be provided to 
   * the matcher
   * $i2$ is an integer variable called "i2" which is matched
   * freetext is a free text which is matched verbatim
   */
  class FunctionNameMatcher
  {
  public:

    FunctionNameMatcher();
    FunctionNameMatcher(std::string pattern);

    void setPattern(std::string pattern);
    bool setParameter(std::string parName, std::string parVal);
    std::string parameter(std::string) const;

    /// return false if name does not match
    bool match(std::string str);

    int intVariable(std::string varname);

  private:

    bool _split_and_subst();

    std::string m_pattern;

    mutable std::map<std::string, std::string> m_par;
    mutable std::map<std::string, int> m_intvar;

    mutable std::vector<std::string> m_split_buf;
  };
// }
}
#endif
