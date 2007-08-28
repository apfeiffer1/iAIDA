#include "DataXML/DataObject.h"
#include <assert.h>
namespace DataXML
{

  // just to make things shorter 
  typedef std::map<std::string,std::string> attr_map_t;
  typedef std::vector<DataObject> obj_vec_t;

  typedef const std::map<std::string,std::string>& const_ref_attr_map_t;
  typedef const std::vector<DataObject>& const_ref_obj_vec_t;



DataObject the_void_object_def;

const DataObject& DataObject::the_void_object() 
{ 
  return the_void_object_def; 
}

DataObject::DataObject() {}

DataObject::DataObject(std::string name) : m_name(name) {}

void DataObject::setName(std::string name) { m_name = name; }
std::string DataObject::name() const { return m_name; }

bool DataObject::appendAttribute(std::string attr_name, std::string attr_val)
{
  return m_attr.insert(make_pair(attr_name,attr_val)).second;
}

void DataObject::setAttribute(std::string attr_name, std::string attr_val)
{
  attr_map_t::iterator pos = m_attr.find(attr_name);

  if(pos == m_attr.end())
    m_attr.insert(make_pair(attr_name,attr_val));
  else
    pos->second = attr_val;

}

void DataObject::appendChild(const DataObject& elem)
{
  m_child.push_back(elem);
}

std::string DataObject::getAttributeValue(std::string attr_name) const
{
  attr_map_t::const_iterator pos = m_attr.find(attr_name);

  if(pos == m_attr.end()) return "";
  else
    return pos->second;

}

DataObject* DataObject::getChild(std::string child_name)
{
  std::vector<DataObject>::iterator it;

  for(it=m_child.begin();it!=m_child.end(); it++)
    if( it->name() == child_name ) return &(*it);

  return 0;
}

const DataObject* DataObject::getChild(std::string child_name) const
{
  std::vector<DataObject>::const_iterator it;

  for(it=m_child.begin();it!=m_child.end(); it++)
    if( it->name() == child_name ) return &(*it);

  return 0;
}

DataObject* DataObject::firstChild()
{
  if(m_child.size()>0) return &m_child[0];
  else
    return 0;
}

const DataObject* DataObject::firstChild() const
{
  if(m_child.size()>0) return &m_child[0];
  else
    return 0;
}

DataObject* DataObject::lastChild()
{
  if(m_child.size()>0) return &m_child[m_child.size()-1];
  else
    return 0;
}

const DataObject* DataObject::lastChild() const
{
  if(m_child.size()>0) return &m_child[m_child.size()-1];
  else
    return 0;
}


DataObject TextDataObject(std::string text)
{
  DataObject d_text;
  d_text.setName("@text");
  d_text.setAttribute("text",text);
  return d_text;
}


void DataObject::appendChildText(std::string text)
{
  DataObject d_text;
  d_text.setName("@text");
  d_text.setAttribute("text",text);
  appendChild(d_text);
}

std::string DataObject::getText() const
{
  if(this->name() == "@text")
  {
    attr_map_t::const_iterator pos = m_attr.find("text");
    
    if(pos == m_attr.end()) return "";
    else
      return pos->second;
  } 
  return "";  
}


obj_vec_t& DataObject::children() { return m_child; }
attr_map_t& DataObject::attributes() { return m_attr; }

const_ref_obj_vec_t DataObject::children() const { return m_child; }
const_ref_attr_map_t DataObject::attributes() const { return m_attr; }

//-----------------------------------------

bool DataObjectTreeCreator::empty() { return m_current_obj == 0; }
bool DataObjectTreeCreator::finished() { return m_finished; }
DataObject* DataObjectTreeCreator::root() { return &m_root_obj; }
DataObject* DataObjectTreeCreator::current() { return m_current_obj; }

std::string DataObjectTreeCreator::current_freemarkup() { return m_freemarkup_buffer; }
void DataObjectTreeCreator::append_next_freemarkup_segment(std::string text) { m_freemarkup_buffer += text; }

void DataObjectTreeCreator::createAndOpenSubObject()
{	
  
  if( empty() )
    {
      m_current_obj = &m_root_obj;
    }
  else
    {
      // save parent on stack
      m_parent_stack.push(m_current_obj);
      
      // create new child and append it to the list
      obj_vec_t& childvec = m_current_obj->children();
      
      childvec.push_back(DataObject());
      m_current_obj = & ( childvec.back() );
    }
}

void DataObjectTreeCreator::backToParent()
{
  if(!m_parent_stack.empty())
    {
      m_current_obj = m_parent_stack.top();
      m_parent_stack.pop();
    }
  else
    if(!empty()) // backToParent on empty is no-op
      m_finished = true; // backToParent from root = finished
}


void DataObjectTreeCreator::appendTextAsSubObject()
{
 std::string text = m_freemarkup_buffer;
 
 if(text!="")
 {
 // PENDING: white spaces at the beginning of line are not deleted yet
    if(text != "\n")
    {

      // removes '\n' at the begining and at the end of the string
      text.erase(text.begin());
      text.erase(text.end()-1);

      
      createAndOpenSubObject();

      assert(current());

      current()->setName("@text");
      current()->setAttribute("text",text);

      backToParent();
    } 
 
 m_freemarkup_buffer = "";   
 }
}



} // namespace
