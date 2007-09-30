#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_RWNTUPLE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_RWNTUPLE_H

#include "IPersistentTuple.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_HBookStore {

class RWNtuple : virtual public IPersistentTuple
{
public:
  RWNtuple( const std::string& memoryDirectory,
	    int id, unsigned int numberOfVariables );
  ~RWNtuple() {}

  bool bindVariable( int variableIndex );
  bool clearBindings();
  bool writeTupleRow( int rowNumber );
  bool readTupleRow( int rowNumber );
  bool reset();
  void* variableAddress( int variableIndex );
  const void* variableAddress( int variableIndex ) const;

  bool bindVariable( int, const std::string& ) {return false;}
  bool clearBindings( const std::string& ) {return false;}
  bool writeTupleRow( int, const std::string& ) {return false;}
  bool readTupleRow( int, const std::string& ) {return false;}
  bool reset( const std::string& ) {return false;}
  void* variableAddress( int, const std::string& ) {return 0;}
  const void* variableAddress( int, const std::string& ) const {return 0;}

private:
  std::string           m_dir;
  int                   m_id;
  std::vector< float >  m_cache;
  bool                  m_initializedReading;
};

  }
}

#endif
