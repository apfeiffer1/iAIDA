# ifndef _XMLStream_h__included__
# define _XMLStream_h__included__

///////////////////////////////////////////////////
// DataXML: Utility for Storing Simple Object Data
//
// Component: utilities
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
///////////////////////////////////////////////////

// this header contains mainpage for doxygen documentation 
// see bottom of the file

# include "DataObject.h"

namespace DataXML 
{

/// holder of implementation-specific parser data
class XMLParserImplementation;

/**
 * Input stream for XML data objects.
 *
 * Buffering and error handling.
 * You can specify the stream for every read() separately or 
 * set a default stream. Because default stream is stored externally, 
 * you should make sure stream is not destroyed while it is set default.
 */
class InputXMLStream
{
 public:

  /// use cin as a default istream
  InputXMLStream();

  /// set default istream,
  /// default stream is stored externally and must not cease to exist
  /// before it is released by setDefaultStream()
  InputXMLStream(std::istream&);

  /// clean up buffers
  ~InputXMLStream();
  
  /// read-in a data object from default stream
  /// success() tells if the operation was successful
  const DataObject& read();
  
  /// read-in a data object from specified stream,
  /// success() tells if the operation was successful
  const DataObject& read(std::istream&);

  /// true if last read successful
  bool success() const;

  /// error explanation if success()==false
  std::string errorName() const;
  
  /// portion of source text around which error was detected
  std::string errorSourcePoint() const;

  /// get default stream
  std::istream& getDefaultStream() const;

  /// change default stream, 
  /// release previous default stream (not used anymore)
  void setDefaultStream(std::istream& = std::cin); 
  
 private:

  void _init(std::istream&);

  /// pointer to implementation
  XMLParserImplementation *m_impl;
  
  /// buffered streaming (XML parsers deal with memory not streams)
  char *m_SRC_BUF;

  /// size of the buffer
  int m_BUF_SIZE;

  /// success flag
  bool m_success;

  /// default stream
  std::istream *m_def_stream;
};

/**
 * Output stream for XML data objects.
 *
 * You can specify the stream for every write() separately or 
 * set a default stream. Because default stream is stored externally, 
 * you should make sure stream is not destroyed while it is set default.
 *
 */
class OutputXMLStream
{
 public:

  /// use cout as a default ostream
  OutputXMLStream();

  /// set default ostream,
  /// default stream is stored externally and must not cease to exist
  /// before it is released by setDefaultStream()
  OutputXMLStream(std::ostream& os);

  /// release default stream
  ~OutputXMLStream();

  /// render data object into specified ostream
  /// false if error, check ostream state 
  bool write(const DataObject&, std::ostream&) const;

  /// render data object into default ostream,
  /// false if error, check default ostream state
  bool write(const DataObject&) const;

  /// get default stream
  std::ostream& getDefaultStream() const;

  /// change default stream, 
  /// release previous default stream (not used anymore)
  void setDefaultStream(std::ostream& = std::cout); 

 private:

  /// default stream
  std::ostream *m_def_stream;
};

} // namespace

/**
 * \mainpage
 * \author Jakub T. Moscicki
 * \version beta
 *
 * DataXML: XML streaming capability for C++ data objects.
 *
 * \ref DataXML::DataObject "DataObject" defines the structure of your data. 
 * \ref DataXML::OutputXMLStream "OutputXMLStream" stores the
 * content of DataObject into ostream. 
 * \ref DataXML::InputXMLStream "InputXMLStream" reads it back from 
 * istream. All names defined in this package are in 
 * \ref DataXML "DataXML namespace".
 *
 * <b> Examples: </b>
 *
 * \ref dxml_write_example
 * defines a data object structure and writes it to cout.
 *
 * The result is the following XML data definition:
 * \include sample.xml
 *
 * \ref dxml_read_example
 * reads the XML data definition from cin, accesses the data and interprets it.
 *
 * \ref dxml_copy_example
 * copies the data objects from one stream to another (cin->cout)
 *
 */

# endif
