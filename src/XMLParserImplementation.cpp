// using expat 1.2 interface (XML parser)
#include "XMLParserImplementation.h"
#include <assert.h>


extern "C"
{
# include "expat.h"

// expat callbacks

void DataXML_StartElementHandler(void *userData, 
				 const XML_Char *name, 
				 const XML_Char **atts)
  {
    DataXML::DataObjectTreeCreator *tree = static_cast<DataXML::DataObjectTreeCreator*>(userData);

    assert(tree);
    assert(atts);
    assert(name);

    tree->appendTextAsSubObject();
    
    tree->createAndOpenSubObject();

    assert(tree->current());

    tree->current()->setName(std::string(name));

    const XML_Char **cur_attr = atts;

    while(cur_attr[0])
      {
	tree->current()->setAttribute(cur_attr[0], cur_attr[1]); 
	cur_attr += 2;
      }    
  }

void DataXML_EndElementHandler(void *userData, 
			       const XML_Char *name)
  {
    DataXML::DataObjectTreeCreator *tree = static_cast<DataXML::DataObjectTreeCreator*>(userData);

    assert(tree);
    assert(std::string(name) == tree->current()->name());

    tree->appendTextAsSubObject();
    
    tree->backToParent();
  }


void DataXML_CharacterDataHandler(void *userData,
                                  const XML_Char *s,
        			  int len)
 {
    DataXML::DataObjectTreeCreator *tree = static_cast<DataXML::DataObjectTreeCreator*>(userData);

    assert(tree);
    assert(s);

    std::string text(s,len);

    tree->append_next_freemarkup_segment(text);
  }


} // extern "C"

namespace DataXML 
{



XMLParserImplementation::XMLParserImplementation() 
{
  _init();
}

void XMLParserImplementation::_init()
{
  m_parser = XML_ParserCreate(NULL);
  
  assert(m_parser); // PENDING: no memory
  
  XML_SetElementHandler(m_parser,
			DataXML_StartElementHandler,
			DataXML_EndElementHandler);
  
  XML_SetUserData(m_parser, &m_tree);

  XML_SetCharacterDataHandler(m_parser, 
                              DataXML_CharacterDataHandler);
}

XMLParserImplementation::~XMLParserImplementation() 
{
  XML_ParserFree(m_parser);
}

void XMLParserImplementation::reset()
{
  XML_ParserFree(m_parser);

  m_tree = DataObjectTreeCreator();

  _init();
}

} // end namespace
