//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

# ifndef _XMLSealStream_h__included__
# define _XMLSealStream_h__included__

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

// stream for XML data objects based on streams defined in SealIO

# include "DataObject.h"

namespace iAIDA { 
  class IOInput; 
  class IOOutput; 
}

namespace DataXML 
{

/// holder of implementation-specific parser data
class XMLParserImplementation;

/**
 * Input stream for XML data objects based on the family of streams defined in   *   SealIOTools and SealZip 
 *
 * Buffering and error handling.
 * You can specify the stream for every read() separately or 
 * set a default stream. Because default stream is stored externally, 
 * you should make sure stream is not destroyed while it is set default.
 */

class InputXMLSealStream
{
 public:

  /// set default istream,
  /// default stream is stored externally and must not cease to exist
  /// before it is released by setDefaultStream()
  InputXMLSealStream( seal::IOInput& s);

  /// clean up buffers
  ~InputXMLSealStream();
  
  /// read-in a data object from default stream
  /// success() tells if the operation was successful
  const DataObject& read();
  
  /// read-in a data object from specified stream,
  /// success() tells if the operation was successful
  const DataObject& read( seal::IOInput& s);

  /// true if last read successful
  bool success() const;

  /// error explanation if success()==false
  std::string errorName() const;
  
  /// portion of source text around which error was detected
  std::string errorSourcePoint() const;

  /// get default stream
  seal::IOInput& getDefaultStream() const;

  /// change default stream, 
  /// release previous default stream (not used anymore)
  void setDefaultStream( seal::IOInput& s); 
  
 private:

  void _init(seal::IOInput& s);

  /// pointer to implementation
  XMLParserImplementation *m_impl;
  
  /// buffered streaming (XML parsers deal with memory not streams)
  char *m_SRC_BUF;

  /// size of the buffer
  int m_BUF_SIZE;

  /// success flag
  bool m_success;

  /// default stream
  seal::IOInput *m_def_stream;
};

/**
 * Output stream for XML data objects.
 *
 * You can specify the stream for every write() separately or 
 * set a default stream. Because default stream is stored externally, 
 * you should make sure stream is not destroyed while it is set default.
 *
 */
class OutputXMLSealStream
{
 public:

  /// use cout as a default ostream
  //OutputXMLSealStream();

  /// set default ostream,
  /// default stream is stored externally and must not cease to exist
  /// before it is released by setDefaultStream()
  OutputXMLSealStream( seal::IOOutput & os);

  /// release default stream
  ~OutputXMLSealStream();

  /// render data object into specified ostream
  /// false if error, check ostream state 
  bool write(const DataObject&, seal::IOOutput &) const;

  /// render data object into default ostream,
  /// false if error, check default ostream state
  bool write(const DataObject&) const;

  /// get default stream
  seal::IOOutput & getDefaultStream() const;

  /// change default stream, 
  /// release previous default stream (not used anymore)
  void setDefaultStream( seal::IOOutput & ); 

 private:

  /// default stream
  seal::IOOutput *m_def_stream;
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
 * \ref DataXML::OutputXMLSealStream "OutputXMLSealStream" stores the
 * content of DataObject into ostream. 
 * \ref DataXML::InputXMLSealStream "InputXMLSealStream" reads it back from 
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
