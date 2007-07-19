// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDA_TUPLE_AIDA_CHAINEDTUPLE
#define AIDANATIVE_AIDA_TUPLE_AIDA_CHAINEDTUPLE 1

#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_TupleHeader.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class AIDA_ChainedTuple : virtual public AIDA::Dev::IDevTuple
{
public:
  AIDA_ChainedTuple( const std::vector<IDevTuple*>& tuples );
  ~AIDA_ChainedTuple();

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
  AIDA::ITuple * getTuple(int column);
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

public:
  std::vector<AIDA::Dev::IDevTuple*>  m_tuples;
  AIDA_TupleHeader                    m_header;
  unsigned int                        m_currentTuple;
};

  }
}

#endif /* ifndef AIDANATIVE_AIDA_TUPLE_AIDA_CHAINEDTUPLE */
