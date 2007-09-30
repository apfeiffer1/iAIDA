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

# include "expat.h"

#include "DataXML/DataObject.h"

namespace DataXML { 

/// holder of implementation objects, create and initialize parser
class XMLParserImplementation
{
public:

  XMLParserImplementation();
  ~XMLParserImplementation();

  /// reinitialize parser and tree, must be invoke before parsing a new document
  void reset();

  XML_Parser& parser() { return m_parser; }
  DataObjectTreeCreator& tree() { return m_tree; }

private:

  void _init();

  XML_Parser m_parser;
  DataObjectTreeCreator m_tree;
};

} // end namespace
