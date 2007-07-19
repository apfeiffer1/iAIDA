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
