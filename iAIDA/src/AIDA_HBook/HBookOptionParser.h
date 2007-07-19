#ifndef PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKPARSER_H
#define PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOKPARSER_H

#include <string>

namespace pi {
  namespace AIDA_HBookStore {

class HBookOptionParser
{
private:
  static const int DEFAULT_RECORD_LENGTH = 1024;
  static const int DEFAULT_BUFFER_SIZE_FOR_RWN = 1024;

public:
  /// Constructor initializing the class with the option string
  HBookOptionParser( const std::string& options );
  /// Destructor
  ~HBookOptionParser(){}

  /// returns the file record length
  int recordLength() const;

  /// Checks if the errors are stored together with the contents of the histogram
  bool areErrorsStored() const;

  /// Returns the buffer size for the creation of RWN tuples
  int bufferSizeForRWN() const;

private:
  int m_recordLength;
  bool m_areErrorsStored;
  int m_bufferSizeForRWN;
};

  }
}

#endif
