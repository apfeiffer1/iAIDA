#include "HBookOptionParser.h"

#ifdef OLDSTREAMS
# include <strstream>
# define istringstream istrstream
#else
# include <sstream>
#endif

pi::AIDA_HBookStore::HBookOptionParser::HBookOptionParser( const std::string& options ) :
  m_recordLength( pi::AIDA_HBookStore::HBookOptionParser::DEFAULT_RECORD_LENGTH ),
  m_areErrorsStored( true ),
  m_bufferSizeForRWN( pi::AIDA_HBookStore::HBookOptionParser::DEFAULT_BUFFER_SIZE_FOR_RWN )
{
  std::string::size_type idx;
  const std::string lrec_keyword = "lrec";
  idx = options.find( lrec_keyword );
  if ( idx != std::string::npos ) {
    std::string substring1 = options.substr( idx + lrec_keyword.length() );
    std::string::size_type idx1 = substring1.find( "=" );
    if ( idx1 != std::string::npos ) {
      std::istringstream is( substring1.substr( idx1 + 1 ).c_str() );
      is >> std::ws;
      if ( ! is.eof() ) {
	is >> m_recordLength;
      }
    }
  }

  const std::string storeErrors_keyword = "--noErrors";
  if ( options.find( storeErrors_keyword ) != std::string::npos ) m_areErrorsStored = false;

  const std::string bufRWN_keyword = "nwbuff";
  idx = options.find( bufRWN_keyword );
  if ( idx != std::string::npos ) {
    std::string substring1 = options.substr( idx + bufRWN_keyword.length() );
    std::string::size_type idx1 = substring1.find( "=" );
    if ( idx1 != std::string::npos ) {
      std::istringstream is( substring1.substr( idx1 + 1 ).c_str() );
      is >> std::ws;
      if ( ! is.eof() ) {
	is >> m_bufferSizeForRWN;
      }
    }
  }

}


int
pi::AIDA_HBookStore::HBookOptionParser::recordLength() const
{
  return m_recordLength;
}


bool
pi::AIDA_HBookStore::HBookOptionParser::areErrorsStored() const
{
  return m_areErrorsStored;
}


int
pi::AIDA_HBookStore::HBookOptionParser::bufferSizeForRWN() const
{
  return m_bufferSizeForRWN;
}
