#include "HBookFileHandler.h"
#include "HBook.h"
#include "HBookFileRegistry.h"

#include <stdexcept>
#include <fstream>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

iAIDA::AIDA_HBookStore::HBookFileHandler::HBookFileHandler( const std::string& fileName,
							     int recordLength,
							     bool readOnly,
							     bool createNew ):
  m_lun( iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().nextAvailableLUN( fileName ) )
{
  if ( ! m_lun ) throw std::runtime_error( "File \"" + fileName + "\" is already opened !!!" );

  // Check for bad input
  if ( readOnly && createNew ) {
    iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().releaseLUN( m_lun );
    throw std::runtime_error( "Cannot open NEW file " + fileName + " for READONLY access !!!" );
  }

  // Define the zebra name
  std::ostringstream osZebra;
  osZebra << "LUN" << m_lun;
#ifdef OLDSTREAMS
  osZebra << std::ends;
#endif
  m_zebraName = osZebra.str();

  // Determine the open mode
  iAIDA::AIDA_HBookStore::HBook::FileMode mode = iAIDA::AIDA_HBookStore::HBook::UPDATE;
  if ( createNew || ( ! existsFile( fileName ) ) ) {
    mode = iAIDA::AIDA_HBookStore::HBook::NEW;
  }
  else  if ( readOnly ) {
    if ( ! existsFile( fileName ) ) {
      iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().releaseLUN( m_lun );
      throw std::runtime_error( "File " + fileName + " does not exist" );
    }
    mode = iAIDA::AIDA_HBookStore::HBook::READ_ONLY;
  }

  // Open the file
  bool OK = iAIDA::AIDA_HBookStore::HBook::openDirectAccessFile( m_lun,
								  m_zebraName,
								  fileName,
								  mode,
								  recordLength );
  if ( ! OK ) { // try to unlock the file
    if ( ! iAIDA::AIDA_HBookStore::HBook::removeLock( m_lun, fileName, recordLength ) ) {
      iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().releaseLUN( m_lun );
      throw std::runtime_error( "Could not open file " + fileName );
    }
    if ( ! iAIDA::AIDA_HBookStore::HBook::openDirectAccessFile( m_lun,
								 m_zebraName,
								 fileName,
								 mode,
								 recordLength ) ) {
      iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().releaseLUN( m_lun );
      throw std::runtime_error( "Could not open file " + fileName );
    }
  }
}


iAIDA::AIDA_HBookStore::HBookFileHandler::~HBookFileHandler()
{
  iAIDA::AIDA_HBookStore::HBook::releaseZebraDirectory( m_zebraName );
  iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry().releaseLUN( m_lun );
}

const std::string&
iAIDA::AIDA_HBookStore::HBookFileHandler::zebraName() const
{
  return m_zebraName;
}


bool
iAIDA::AIDA_HBookStore::HBookFileHandler::existsFile( const std::string& fileName ) const
{
  std::ifstream f( fileName.c_str() );
  if ( ! f ) return false;
  else return true;
}
