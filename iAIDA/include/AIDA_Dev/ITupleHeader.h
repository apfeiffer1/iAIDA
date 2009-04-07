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

// Emacs -*- C++ -*-
#ifndef AIDA_ITUPLEHEADER
#define AIDA_ITUPLEHEADER 1

#include <string>

namespace AIDA {
  class IAnnotation;

  namespace Dev {
    class ITupleVariableDescription;

/// Abstract interface class for the header of a tuple

class ITupleHeader {

public:
  /// Destructor
  virtual ~ITupleHeader() {/*nop*/;}

  /// Returns the annotation object
  virtual AIDA::IAnnotation& annotation() = 0;
  virtual const AIDA::IAnnotation& annotation() const = 0;

  /// Sets the description of a variable passing the ownership if needed. (Yes for top-level tuple, no for all sub-tuples).
  virtual bool setVariableDescription( ITupleVariableDescription* description,
				       bool passOwnership ) = 0;

  /// Retrieves the description of a variable
  virtual const ITupleVariableDescription* variableDescription( int index ) const = 0;
  virtual ITupleVariableDescription* variableDescription( int index ) = 0;

  /// Returns the column index given the variable name.
  virtual int variableIndex( const std::string& variableName ) const = 0;

  /// Retrieves the number of variables
  virtual int numberOfVariables() const = 0;

  /// Sets the path of the tuple object in the store
  virtual bool setPathInStore( const std::string& path ) = 0;

  /// Retrieves the path of the tuple in the store
  virtual const std::string& pathInStore() const = 0;

  /// Sets/Retrieves the number of rows in the tuple
  virtual bool setNumberOfRows( int rows ) = 0;
  virtual int numberOfRows() const = 0;

  /// Sets/Retrieves the current row number
  virtual bool setCurrentRowNumber( int rowNumber ) = 0;
  virtual int currentRowNumber() const = 0;

  /// Sets/Retrieves the options string
  virtual bool setOptions( const std::string& options ) = 0;
  virtual const std::string& options() const = 0;
};

  }
}

#endif /* ifndef AIDA_ITUPLEHEADER */
