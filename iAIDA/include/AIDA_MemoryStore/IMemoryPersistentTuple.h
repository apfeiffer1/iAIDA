#ifndef IMEMORYPERSISTENTTUPLE_H
#define IMEMORYPERSISTENTTUPLE_H

namespace iAIDA {
  namespace AIDA_MemoryStore {

    class MemoryTupleData;

class IMemoryPersistentTuple
{
public:
  virtual ~IMemoryPersistentTuple() {}
  virtual bool setTupleData( MemoryTupleData* data ) = 0;
  virtual MemoryTupleData* tupleData() const = 0;
  virtual bool bindVariable( int variableIndex ) = 0;
  virtual bool clearBindings() = 0;
  virtual bool writeTupleRow( int rowNumber ) = 0;
  virtual bool readTupleRow( int rowNumber ) = 0;
  virtual void* variableAddress( int variableIndex ) = 0;
  virtual const void* variableAddress( int variableIndex ) const = 0;
};

  }
}
#endif
