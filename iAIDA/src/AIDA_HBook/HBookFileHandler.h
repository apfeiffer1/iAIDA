#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKFILEHANDLER_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKFILEHANDLER_H

#include <string>

namespace iAIDA {
  namespace AIDA_HBookStore {

class HBookFileHandler
{
public:
  /// Constructor : opens the file
  HBookFileHandler( const std::string& fileName,
		    int recordLength,
		    bool readOnly,
		    bool createNew );

  /// Destructor : closes the file
  ~HBookFileHandler();

  /// Returns the ZEBRA name associated to the file
  const std::string& zebraName() const;

private:
  int         m_lun;
  std::string m_zebraName;

private:
  bool existsFile( const std::string& fileName ) const;
};

  }
}

#endif
