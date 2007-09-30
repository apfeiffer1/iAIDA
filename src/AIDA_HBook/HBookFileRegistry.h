#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOK_HBOOKFILEREGISTRY_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOK_HBOOKFILEREGISTRY_H

#include <map>
#include <string>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookFileRegistry
{
public:
  static HBookFileRegistry& theRegistry();

protected:
  HBookFileRegistry() {};
  HBookFileRegistry( const HBookFileRegistry& );
  HBookFileRegistry& operator=( const HBookFileRegistry& );

public:
  ~HBookFileRegistry() {};

  int nextAvailableLUN( const std::string& fileName );
  void releaseLUN( int lun );

private:
  std::map< int, std::string > m_allocatedLUNs;
  std::map< std::string, int > m_fileNames;
};

  }
}
#endif
