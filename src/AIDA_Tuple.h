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
#ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLE
#define IAIDA_AIDA_TUPLE_AIDA_TUPLE 1

#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_TupleHeader.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {

/// Implementation of the AIDA::ITuple interface

class AIDA_Tuple : virtual public AIDA::Dev::IDevTuple
{
public:
  /// Constructor
  AIDA_Tuple();
  /// Destructor
  ~AIDA_Tuple();

  // Methods inherited from IManagedObject
  std::string  name() const;

  // Methods inherited from IDevManagedObject
  bool isUpToDate() const;
  void setUpToDate( bool isUpToDate );
  bool setName( const std::string& newName );
  const std::string& userLevelClassType() const;

  // Methods inherited from IDevTuple
  bool connectToStore( AIDA::Dev::IBackingStore* store );
  AIDA::Dev::IBackingStore* store();
  const AIDA::Dev::IBackingStore* store() const;
  AIDA::Dev::ITupleHeader& header();
  const AIDA::Dev::ITupleHeader& header() const;
  void* variableAddress( const std::string& variableName );
  const void* variableAddress( const std::string& variableName ) const;
  void setVariableAddress(int column, void * p);

  // Methods inherited from AIDA::ITuple
  std::string  title() const;
  bool setTitle(const std::string & title);
  AIDA::IAnnotation & annotation();
  const AIDA::IAnnotation & annotation() const;
  bool fill(int column, double value);
  bool fill(int column, float value);
  bool fill(int column, int value);
  bool fill(int column, short value);
  bool fill(int column, long value);
  bool fill(int column, char value);
  bool fill(int column, bool value);
  bool fill(int column, const std::string & value);
  bool fill(int column, const AIDA::ITupleEntry & value);
  bool fill(const std::vector<double>& values);
  bool fill(const std::vector<float>& values);
  bool addRow();
  void resetRow();
  void reset();
  int rows() const;
  void start();
  bool skip(int rows);
  bool next();
  bool setRow(int rowIndex);
  int findColumn(const std::string & name) const;
  double getDouble(int column) const;
  float getFloat(int column) const;
  int getInt(int column) const;
  short getShort(int column) const;
  long getLong(int column) const;
  char getChar(int column) const;
  bool getBoolean(int column) const;
  std::string  getString(int column) const;
  const AIDA::ITupleEntry * getObject(int column) const;
  const AIDA::ITuple * getTuple(int column) const;
  AIDA::ITuple* getTuple(int column);
  int columns() const;
  std::string  columnName(int column) const;
  std::vector<std::string>  columnNames() const;
  std::string  columnType(int column) const;
  std::vector<std::string>  columnTypes() const;
  double columnMin(int column) const;
  double columnMax(int column) const;
  double columnMean(int column) const;
  double columnRms(int column) const;
  bool project(AIDA::IHistogram1D & histogram, AIDA::IEvaluator & evaluatorX);
  bool project(AIDA::IHistogram1D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IFilter & filter);
  bool project(AIDA::IHistogram1D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & weight);
  bool project(AIDA::IHistogram1D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::IHistogram2D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY);
  bool project(AIDA::IHistogram2D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter);
  bool project(AIDA::IHistogram2D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & weight);
  bool project(AIDA::IHistogram2D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::IHistogram3D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ);
  bool project(AIDA::IHistogram3D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter);
  bool project(AIDA::IHistogram3D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IEvaluator & weight);
  bool project(AIDA::IHistogram3D & histogram, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud1D & cloud, AIDA::IEvaluator & evaluatorX);
  bool project(AIDA::ICloud1D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IFilter & filter);
  bool project(AIDA::ICloud1D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud1D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud2D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY);
  bool project(AIDA::ICloud2D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter);
  bool project(AIDA::ICloud2D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud2D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud3D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ);
  bool project(AIDA::ICloud3D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter);
  bool project(AIDA::ICloud3D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IEvaluator & weight);
  bool project(AIDA::ICloud3D & cloud, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::IProfile1D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY);
  bool project(AIDA::IProfile1D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter);
  bool project(AIDA::IProfile1D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & weight);
  bool project(AIDA::IProfile1D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IFilter & filter, AIDA::IEvaluator & weight);
  bool project(AIDA::IProfile2D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ);
  bool project(AIDA::IProfile2D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter);
  bool project(AIDA::IProfile2D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IEvaluator & weight);
  bool project(AIDA::IProfile2D & profile, AIDA::IEvaluator & evaluatorX, AIDA::IEvaluator & evaluatorY, AIDA::IEvaluator & evaluatorZ, AIDA::IFilter & filter, AIDA::IEvaluator & weight);

  void * cast(const std::string & className) const; 

private:
  AIDA_TupleHeader              m_header;
  bool                          m_upToDate;
  AIDA::Dev::IBackingStore*     m_store;

  // private methods
  template< class T > bool fillChecked( int column, T value );
  template< class T > const T* getVariable( int column ) const;
  template< class T > T* getVariable( int column );
};

  }
}

#include "AIDA_Tuple.templ"

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_TUPLE */
