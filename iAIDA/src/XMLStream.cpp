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

# include "DataXML/XMLStream.h"
# include "XMLParserImplementation.h"

# include <assert.h>

namespace DataXML 
{

  // just to make things shorter 
  typedef std::map<std::string,std::string> attr_map_t;
  typedef std::vector<DataObject> obj_vec_t;

  typedef const std::map<std::string,std::string>& const_ref_attr_map_t;
  typedef const std::vector<DataObject>& const_ref_obj_vec_t;

    
// ==========================================================================================
// Input
// ==========================================================================================

InputXMLStream::InputXMLStream()
{
  _init(std::cin);
}

InputXMLStream::InputXMLStream(std::istream& is)
{
  _init(is);
}

void InputXMLStream::_init(std::istream &is)
{
  m_success = true;

  m_BUF_SIZE = 300;
  m_SRC_BUF = new char[m_BUF_SIZE];

  m_impl = new XMLParserImplementation();

  m_def_stream = &is;

  assert(m_SRC_BUF); // PENDING: no memory
  assert(m_impl);    // PENDING: no memory

  assert(m_def_stream); // fatal error ?
}

InputXMLStream::~InputXMLStream()
{
  delete m_impl;
  delete [] m_SRC_BUF;
}

bool InputXMLStream::success() const
{
  return m_success;
}

std::string InputXMLStream::errorName() const
{
  return XML_ErrorString(XML_GetErrorCode(m_impl->parser()));
}

std::string InputXMLStream::errorSourcePoint() const
{
  return ""; // PENDING: to be implemented
}

const DataObject& InputXMLStream::read()
{
  return read(getDefaultStream());
}

const DataObject& InputXMLStream::read(std::istream& is)
{
  m_impl->reset(); // reset parser and tree

  int pars_OK = 1;

  // PENDING: if garbage at the begining, then the root tag is lost
  // to make it better, 
  do
    {
      // well, we never read beyond terminating >
      // this implementation is not very efficient but very simple.
      // faster implementation would require to seek for terminating
      // tag once the root element name is known (carefully with "/>")

      is.get(m_SRC_BUF,m_BUF_SIZE, '>');

      int buf_len = is.gcount(); // max buf_len == BUF_SIZE-1

      is.get(m_SRC_BUF[buf_len]);
      pars_OK = XML_Parse(m_impl->parser(), m_SRC_BUF, buf_len+1, 0);

      /* DEBUG:
            m_BUF_SRC[buf_len+1] = 0;
            cout << endl << "*" << buf << "*";
      */
    }
  while(!m_impl->tree().finished() && pars_OK);

  if(!pars_OK)
    {
      m_success = false;
      std::cout << "XML parser error: " << errorName();
    }
  else
    {
      m_success = true;
    }

  return m_impl->tree().root() ? *(m_impl->tree().root()) : DataObject::the_void_object();
}

std::istream& InputXMLStream::getDefaultStream() const
{
  assert(m_def_stream);

  return *m_def_stream;
}

void InputXMLStream::setDefaultStream(std::istream& is)
{
  m_def_stream = &is;
}

// ==========================================================================================
// Output
// ==========================================================================================

OutputXMLStream::OutputXMLStream() 
{
  m_def_stream = &std::cout;
}

OutputXMLStream::OutputXMLStream(std::ostream& os)
{
  m_def_stream = &os;
}

OutputXMLStream::~OutputXMLStream() {}

bool OutputXMLStream::write(const DataObject& obj) const
{
  return write(obj, getDefaultStream());
}

bool OutputXMLStream::write(const DataObject& obj, std::ostream& os) const
{
  if (obj.getText() != "") {
     os << obj.getText();
  } else {

    if (obj.name() == "") {
        os << std::endl << "OutputXMLStream::write> No element name found for text " << obj.getText() << std::endl;
        return false;
    }

    os << "<" << obj.name();

    attr_map_t::const_iterator it1;

    const_ref_attr_map_t attr = obj.attributes();

    if (!attr.empty()) {      
        for(it1 = attr.begin(); it1!=attr.end(); ++it1) {
          os << " " << it1->first << "=\"" << it1->second << "\"";
          if ( !os.good() || os.fail() || os.bad() || os.eof() ) {
             std::cerr << "FATAL: Error during write !!" << std::endl;
             std::cerr << " got :" << std::endl;
             std::cerr << "     good" << os.good() << std::endl;
             std::cerr << "     fail" << os.fail() << std::endl;
             std::cerr << "     bad " << os.bad () << std::endl;
             std::cerr << "     eof " << os.eof () << std::endl;
             return false;
          } // end if ios.good ...
       } // end for
    } // end if !attr.empty 

    if (!os) return false;

    const_ref_obj_vec_t child = obj.children();

    if(child.empty()) {
      os << "/>";
    } else {
         os << ">";
         obj_vec_t::const_iterator it2;

         for(it2 = child.begin(); it2!=child.end(); ++it2) {
                os << std::endl; // close previous line
                if (!write(*it2,os)) return false;
         }
        os << std::endl << "</" << obj.name() << ">";
     } // end else (if child.empty)
  } // end else obj.getText == ""
  
  return !os ? false : true;
}


std::ostream& OutputXMLStream::getDefaultStream() const
{
  assert(m_def_stream);

  return *m_def_stream;
}

void OutputXMLStream::setDefaultStream(std::ostream& os)
{
  m_def_stream = &os;
}

} // namespace

