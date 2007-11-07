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
#ifndef TUPLE_VARIABLE_COPY_H
#define TUPLE_VARIABLE_COPY_H

#include "AIDA/ITuple.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {

// A small class for automatic lookup of a method
class TupleVariableCopyBase {
public:
  virtual ~TupleVariableCopyBase(){}
  virtual void copyTupleVariable( const AIDA::ITuple& tupleFrom,
				  int columnFrom,
				  AIDA::ITuple& tupleTo,
				  int columnTo ) const = 0;
};

class TupleVarialbeCopyDouble : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyDouble(){}
  ~TupleVarialbeCopyDouble(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getDouble( columnFrom ) );
  }

};


class TupleVarialbeCopyFloat : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyFloat(){}
  ~TupleVarialbeCopyFloat(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getFloat( columnFrom ) );
  }

};


class TupleVarialbeCopyLong : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyLong(){}
  ~TupleVarialbeCopyLong(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getLong( columnFrom ) );
  }

};


class TupleVarialbeCopyInt : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyInt(){}
  ~TupleVarialbeCopyInt(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getInt( columnFrom ) );
  }

};


class TupleVarialbeCopyShort : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyShort(){}
  ~TupleVarialbeCopyShort(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getShort( columnFrom ) );
  }

};


class TupleVarialbeCopyChar : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyChar(){}
  ~TupleVarialbeCopyChar(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getChar( columnFrom ) );
  }

};


class TupleVarialbeCopyString : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyString(){}
  ~TupleVarialbeCopyString(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getString( columnFrom ) );
  }

};


class TupleVarialbeCopyBoolean : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyBoolean(){}
  ~TupleVarialbeCopyBoolean(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const
  {
    tupleTo.fill( columnTo, tupleFrom.getBoolean( columnFrom ) );
  }

};


class TupleVarialbeCopyTuple : virtual public TupleVariableCopyBase {
public:
  TupleVarialbeCopyTuple(){}
  ~TupleVarialbeCopyTuple(){}
  void copyTupleVariable( const AIDA::ITuple& tupleFrom,
			  int columnFrom,
			  AIDA::ITuple& tupleTo,
			  int columnTo ) const;
};


  }
}
#endif
