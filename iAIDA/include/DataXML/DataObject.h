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

# ifndef _DataObject_h__included__
# define _DataObject_h__included__

///////////////////////////////////////////////////
// DataXML: Utility for Storing Simple Object Data
//
// Component: utilities
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
///////////////////////////////////////////////////

# include <string>
# include <vector>
# include <map>
# include <iostream>

# include <stack> /* for tree */

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

namespace DataXML
{

/**
 * XML-streamable data object.
 *
 * Data object provides an abstraction of simple data model (the tree of 
 * children and parents). The data object:
 *   - has a type name
 *   - may contain any number of attributes with unique names + values
 *   - may contain any number of other objects.
 *
 * DataObject represents XML element. In some cases it may also represent a
 * markup-free text. DataObject::getText() may be used to check if
 * data object represents text ("" means that it represents the element).
 * TextDataObject() pseudo-constructor creates a text data object. You may
 * also use the shortcut DataObject::appendChildText(). It makes no sense
 * to modify attributes of text objects.
 *
 * By-value object containment is supported. Reference semantics 
 * (pointer associations) are not yet supported.
 *
 * Typically you create an instance of DataObject out of your own objects.
 * Then you use it as a high-level interface to render to/from XML.
 *
 * [To implement reference semantics we need to intorduce larger units 
 * (groups of objects) for which referential integrity is guaranteed.
 * Maybe ref integrity domain could be a root data object. 
 * Optional object identifiers must also be provided.]
 *
 */


class DataObject
{
public:

  /// data object must have non-empty names, use setName()
  DataObject();

  /// name must be non-empty
  DataObject(std::string name);

  /// @name name handling
                                                        //@{
  std::string name() const;  
  void setName(std::string name);
                                                        //@}

  /// @name shortcuts for attribute handling
                                                        //@{
  /// append new attribute at end, false if name already exists
  bool appendAttribute(std::string attr_name, std::string attr_val);

  /// modify exisiting attribute or append new at end
  void setAttribute(std::string attr_name, std::string attr_val);

  /// remove attribute
  void removeAttribute(std::string attr_name);

  /// return "" if not found
  std::string getAttributeValue(std::string attr_name) const;
                                                        //@}

  /// @name shortcuts for children handling

                                                        //@{
  /// append child objects at end (vector::push_back())
  void appendChild(const DataObject& elem);

  /// get the first child with given name
  /// return 0 if not found
  DataObject* getChild(std::string child_name);

  /// get the first child with given name
  /// return 0 if not found
  const DataObject* getChild(std::string child_name) const;

  /// return 0 if no childern
  DataObject* firstChild();
  const DataObject* firstChild() const;

  /// return 0 if no children
  DataObject* lastChild();
  const DataObject* lastChild() const;

                                                        //@}


  /// @name shortcuts for text handling
                                                        //@{
  /// append PCDATA text as a child object
  void appendChildText(std::string text);

  /// get PCDATA, if 'this' is not text object return ""
  std::string getText() const;
                                                        //@}


  /// @name direct manipulation of childern tree via STL vector
                                                        //@{
  std::vector<DataObject>& children(); 
  const std::vector<DataObject>& children() const;
                                                        //@}




  /// @name direct manipulation of attributes via STL map
                                                        //@{
  
  std::map<std::string,std::string>& attributes();
  const std::map<std::string,std::string>& attributes() const;
                                                        //@}

  static const DataObject& the_void_object();

private:

  std::string m_name;

  std::map<std::string,std::string> m_attr;
  std::vector<DataObject> m_child;

};

  /**
   * Create a text object with given text.
   */
  DataObject TextDataObject(std::string text);


/**
 * convert value to std::string using stream operator <<.
 * consequently, type T must have operator << for ostream
 */

template<class T>
std::string to_string(T val)
{
  std::ostringstream buf;
  buf << val;
#ifndef BADENDS
  buf << std::ends;
#endif
  std::string ret = buf.str();
  return ret;
}

/**
 * convert std::string to value using stream operator >>.
 * consequently, type T must have operator >> for istream
 */

template<class T>
bool to_value(std::string s, T& val)
{
  std::istringstream buf( s.c_str() );
  buf >> std::ws;
  if ( buf.eof() || (! buf.good() ) ) return false;
  buf >> val;
  return true;
}

/** 
 * Traversal & creation of the object tree (utility for parsers).
 * Initially the tree is empty, so root object is created first.
 *
 * Using this tree creator, you can get the following behaviour of
 * your parser:
 *
 * The first XML element encountered is treated as a root element.
 * It is not possible to have multiple root elements (only first XML
 * element may be read-in). The rest is ignored.
 */

class DataObjectTreeCreator
{
public:

  DataObjectTreeCreator() : m_current_obj(0), m_parent_stack(), m_root_obj(), m_finished(false) {}

  /// no current object yet (nothing parsed in, true at the begining)
  bool empty();

  /// root object entirely parsed-in (true at the end)
  bool finished();

  /// get root object (null if empty())
  DataObject* root();

  /// get current data object
  DataObject* current();

  /// append empty data object at end and open it (becomes current).
  /// if tree is empty, then create root and open it
  void createAndOpenSubObject();

  /// go up in the tree (parent object becomes current)
  void backToParent();
  
  /// append new child as text object with the text in current_freemarkup()
  /// buffer and clean the buffer
  void appendTextAsSubObject();
  
  /// buffer with current free markup, if "" then no free markup being read
  std::string current_freemarkup();

  void append_next_freemarkup_segment(std::string text);


private:
  
  DataObject *m_current_obj; /* current data object, 0 if begin */
  std::stack<DataObject*> m_parent_stack; /* trace back parent objects */
  DataObject m_root_obj; /* root object */
  bool m_finished; /* true if finished*/
  std::string m_freemarkup_buffer;
};

} // namespace

# endif
