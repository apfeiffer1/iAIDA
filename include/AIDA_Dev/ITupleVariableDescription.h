// Emacs -*- C++ -*-
#ifndef AIDA_ITUPLEVARIABLEDESCRIPTION
#define AIDA_ITUPLEVARIABLEDESCRIPTION 1

#include <string>

namespace AIDA {
  namespace Dev {
    class ITupleVariableStatistics;

/// Abstract interface class for the description of a tuple variable

class ITupleVariableDescription {

public:
  /// Destructor
  virtual ~ITupleVariableDescription() {/*nop*/;}

  /// Retrieves/Sets the variable name
  virtual const std::string& variableName() const = 0;
  virtual bool setVariableName( const std::string& name ) = 0;

  /// Retrieves/Sets the variable type
  virtual const std::string& variableType() const = 0;
  virtual bool setVariableType( const std::string& type ) = 0;

  /// Retrieves the stastics for this variable
  virtual const ITupleVariableStatistics& statistics() const = 0;
  virtual ITupleVariableStatistics& statistics() = 0;
  virtual bool resetStatistics() = 0;

  /// Retrieves the number of sub-variables
  virtual int numberOfVariables() const = 0;

  /// Sets the description of a sub-variable indicating whether the ownership is passed
  virtual bool setVariableDescription( ITupleVariableDescription* description,
				       bool passOwnership ) = 0;

  /// Retrieves the description of a sub-variable
  virtual ITupleVariableDescription* variableDescription( int subVariableIndex ) = 0;
  virtual const ITupleVariableDescription* variableDescription( int subVariableIndex ) const = 0;

  /// Retrieves the index of a sub variable given its name
  virtual int variableIndex( const std::string& variableName ) const = 0;
  
};

  }
}

#endif /* ifndef AIDA_ITUPLEVARIABLEDESCRIPTION */
