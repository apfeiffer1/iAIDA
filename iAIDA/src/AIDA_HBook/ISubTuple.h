#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_ISUBTUPLE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_ISUBTUPLE_H

namespace iAIDA {
  namespace AIDA_HBookStore {

class ISubTuple
{
public:
  virtual ~ISubTuple() {}
  virtual bool setFirstElement( unsigned int i ) = 0;
  virtual bool writeTupleRow( int rowNumber ) = 0;
  virtual bool readTupleRow( int rowNumber ) = 0;
  virtual bool reset() = 0;
  virtual void* variableAddress( int variableIndex ) = 0;
  virtual const void* variableAddress( int variableIndex ) const = 0;
};

  }
}

#endif
