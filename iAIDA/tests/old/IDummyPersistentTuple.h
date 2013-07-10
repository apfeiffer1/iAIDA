#ifndef IDUMMYPERSISTENTTUPLE_H
#define IDUMMYPERSISTENTTUPLE_H

class DummyTupleData;

class IDummyPersistentTuple
{
public:
  virtual ~IDummyPersistentTuple() {}
  virtual bool setTupleData( DummyTupleData* data ) = 0;
  virtual DummyTupleData* tupleData() const = 0;
  virtual bool bindVariable( int variableIndex ) = 0;
  virtual bool clearBindings() = 0;
  virtual bool writeTupleRow( int rowNumber ) = 0;
  virtual bool readTupleRow( int rowNumber ) = 0;
  virtual void* variableAddress( int variableIndex ) = 0;
  virtual const void* variableAddress( int variableIndex ) const = 0;
};

#endif
