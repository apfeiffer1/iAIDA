# ifndef _WIN32
# include <dlfcn.h>
#else 
#include <windows.h>
#endif
# include <assert.h>

#ifdef USE_DATAXML
# include "DataXML/DataObject.h"
# include "DataXML/XMLStream.h"
#endif

#include "debug.h"

# include <typeinfo>

#include <ctype.h>

namespace Util
{

  template<class IRes, class ID, class OPT>
  PluginLocator<IRes,ID,OPT>::PluginLocator() : m_urimap()
  {}

  template<class IRes, class ID, class OPT>
  PluginLocator<IRes,ID,OPT>::PluginLocator(std::istream& is)
  {
    _streamIn(is);
  }

  template<class IRes, class ID, class OPT>
  PluginLocator<IRes,ID,OPT>::~PluginLocator()
  {}

  template<class IRes, class ID, class OPT>
  bool PluginLocator<IRes,ID,OPT>::streamOut(std::ostream& os)
  {

  #ifdef USE_DATAXML
    DataXML::DataObject d_loc;

    d_loc.setName("PluginLocator");
    d_loc.setAttribute("IRes", typeid(IRes).name());

    for(typename urimap_t::iterator it = m_urimap.begin(); it!=m_urimap.end(); ++it)
      {
	DataXML::DataObject d_entry;

	d_entry.setName("Plugin");
	d_entry.setAttribute("id", to_string(it->first));
	d_entry.setAttribute("uri", it->second.first);
	d_entry.setAttribute("opt", to_string(it->second.second));

	d_loc.appendChild(d_entry);
      }

    DataXML::OutputXMLStream xml_os(os);

    xml_os.write(d_loc);

    return true;
#else 
    return false; 
#endif
  }

  template<class IRes, class ID, class OPT>
  bool PluginLocator<IRes,ID,OPT>::setURI(const ID& id, 
					  uri_string_t uri, const OPT& opt,
					  bool replace)
  {

    typename urimap_t::iterator pos = m_urimap.find(id);

    if(pos != m_urimap.end())
      if(replace)
	m_urimap.erase(pos);
      else
	return false;

    std::pair<typename urimap_t::iterator,bool> status;

    status = m_urimap.insert(make_pair(id,make_pair(uri,opt)));

    if(!status.second)
      {
	ERROR_MSG("std::map find/insert problem");
	return false;
      }

    return true;
  }

  template<class IRes, class ID, class OPT>
  void PluginLocator<IRes,ID,OPT>::eraseURI(const ID& id)
  {
    typename urimap_t::iterator pos = m_urimap.find(id);
   
    if(pos != m_urimap.end())
      m_urimap.erase(pos);
  }

  template<class IRes, class ID, class OPT>
  uri_string_t PluginLocator<IRes,ID,OPT>::getURI(const ID& id) const
  {
    typename urimap_t::const_iterator pos = m_urimap.find(id);
   
    if(pos != m_urimap.end())
      return pos->second.first;
    else
      return std::string("");
  }

  template<class IRes, class ID, class OPT>
  OPT PluginLocator<IRes,ID,OPT>::getOPT(const ID& id) const
  {
    typename urimap_t::const_iterator pos = m_urimap.find(id);
   
    if(pos != m_urimap.end())
      return pos->second.second;
    else
      return OPT();
  }

  template<class IRes, class ID, class OPT>
  std::vector<ID> PluginLocator<IRes,ID,OPT>::getIDVector() const
  {
    std::vector<ID> v;

    typename urimap_t::const_iterator it = m_urimap.begin();

    while(it != m_urimap.end())
      v.push_back(it->first);

    return v;
  }
  

  template<class IRes, class ID, class OPT>
  bool PluginLocator<IRes,ID,OPT>::_streamIn(std::istream& is)
  {
    if(!is.good())
      return false;

#ifdef USE_DATAXML
    while(is.good())
      {
	int c = is.peek();

	if(c == EOF)
	  return true; // empty stream is OK

	if(!isspace(c))
	  break;

	is.ignore(); // discard next whitespace character
      }

    if(!is.good()) return false; // reading error
    
    DataXML::InputXMLStream xml_is(is);

    const DataXML::DataObject& d_loc = xml_is.read();

    if(!xml_is.success()) return false;

    urimap_t new_urimap;

    if(d_loc.name() == "PluginLocator")
      {
	std::vector<DataXML::DataObject>::const_iterator entry;

	for(entry = d_loc.children().begin(); entry!=d_loc.children().end(); ++entry)
	  if(entry->name() == "Plugin")
	    {
	      ID id;
	      
	      if(!to_value(entry->getAttributeValue("id"),id))
		return false;
	      
	      uri_string_t uri = entry->getAttributeValue("uri");
	      OPT opt;

	      if(!to_value(entry->getAttributeValue("opt"), opt))
		return false;

	      if(new_urimap.insert(make_pair(id,make_pair(uri,opt))).second == false)
		{
		  INFO_MSG("PluginLocator (stream-in): duplicate id " + to_string(id));
		}
	    }
	  else
	    return false;

	// here ok, replace old uri map with the new one
	
	m_urimap = new_urimap;
	return true;
      }

#endif
    return false;
  }

  template<class IRes, class ID, class OPT>
  PluginLoader<IRes,ID, OPT>::PluginLoader()
  {}

  template<class IRes, class ID, class OPT>
  PluginLoader<IRes,ID, OPT>::PluginLoader(std::istream& is)  : m_locator(is)
  {
    _streamIn(is);
  }

  template<class IRes, class ID, class OPT>
  PluginLoader<IRes,ID, OPT>::~PluginLoader()
  {
    typename comp_map_t::iterator it;

    for(it=m_loaded_components.begin(); it!=m_loaded_components.end(); ++it)
      {
#ifndef _WIN32
	dlclose(it->second);
#else
	FreeLibrary ((HINSTANCE) it->second); 	
#endif
# if 0
	while(it->second.second-- > 0)
	  dlclose(it->second.first);
# endif
      }
  }

  template<class IRes, class ID, class OPT>
  bool PluginLoader<IRes,ID, OPT>::streamOut(std::ostream& os)
  {
    if(!m_locator.streamOut(os)) return false;

#ifdef USE_DATAXML
    os << std::endl;

    DataXML::DataObject d_loader;

    d_loader.setName("PluginLoader");
    d_loader.setAttribute("IRes", typeid(IRes).name());

    for(typename comp_map_t::iterator it = m_loaded_components.begin(); 
	it != m_loaded_components.end(); ++it)
      {
	DataXML::DataObject d_entry;

	d_entry.setName("Plugin");
	d_entry.setAttribute("id", to_string(it->first));

	d_loader.appendChild(d_entry);
      }

    DataXML::OutputXMLStream xml_os(os);

    xml_os.write(d_loader);
   
    return true;
#else
    return false;
#endif
   }  

  template<class IRes, class ID, class OPT>
  PluginLocator<IRes,ID,OPT>& PluginLoader<IRes,ID, OPT>::locator()
  {
    return m_locator;
  }

  template<class IRes, class ID, class OPT>
  const PluginLocator<IRes,ID,OPT>& PluginLoader<IRes,ID, OPT>::locator() const
  {
    return m_locator;
  }
 
  template<class IRes, class ID, class OPT>
  bool PluginLoader<IRes,ID, OPT>::loadPluginComponent(const ID& id)
  {

    // check if component already loaded
    if(m_loaded_components.find(id) != m_loaded_components.end())
      {
	if(!PluginRegistry<IRes,ID>::global_registry().query(id))
	  {
	    INFO_MSG("logic error: plugin loaded but not present in the registry");
	  }

	m_last_error.first  = plugin_already_registered;
	m_last_error.second = to_string(id) + ": plugin already registred";

	return false;
      }

    // check if plugin loaded by somebody else
    if(PluginRegistry<IRes,ID>::global_registry().query(id))
      {
	// plugin already exists
	m_last_error.first = plugin_already_registered;
	m_last_error.second = to_string(id) + ": plugin already registred";

	return false;
      }

    // find uri for plugin
    uri_string_t uri = m_locator.getURI(id);

    if(uri.empty())
      {
	m_last_error.first = component_not_found;
	m_last_error.second = to_string(id) + ": component not found";
	return false;
      }

# ifndef _WIN32
    shlib_handle_t new_handle = dlopen(uri.c_str(), RTLD_NOW);

    if(!new_handle)
      { 
	m_last_error.first = system_error;
	m_last_error.second = to_string(id) + ": " + dlerror();	
	return false;
      }
#else 
    shlib_handle_t new_handle = LoadLibrary (uri.c_str () );
    if(!new_handle)
      { 
	m_last_error.first = system_error;
	m_last_error.second = to_string(id) + ": " + to_string(GetLastError ());	
	return false;
      }

#endif

    // check if the plugin really loaded by the component
    // if not - locator provides wrong uri mapping
    if(!PluginRegistry<IRes,ID>::global_registry().query(id))
      {
	m_last_error.first = component_has_no_plugin;
	m_last_error.second = to_string(id) + ": " + uri + ": " + "plugin is not defined in the component";
#ifndef _WIN32
	dlclose(new_handle);
#else 
	FreeLibrary ((HINSTANCE) new_handle);
#endif
	return false;
      }

    // everything ok, insert into map
    m_loaded_components.insert(make_pair(id,new_handle));

# if 0
    // we do not care if uri already there (insert fail)
    // internal count of ldopen does most of the job for us

    comp_map_t::iterator pos = m_loaded_components.insert(make_pair(uri,make_pair(new_handle,0))).first;

    pos->second.second++; // increment the counter
# endif

    return true;    
  }
  
  template<class IRes, class ID, class OPT>
  bool PluginLoader<IRes,ID, OPT>::unloadPluginComponent(const ID& id)
  {
    // check if component was loaded before
    if(m_loaded_components.find(id) == m_loaded_components.end())
      {
	m_last_error.first  = plugin_not_registered;
	m_last_error.second = to_string(id) + ": plugin not found";

	return false;
      }

    typename PluginRegistry<IRes,ID>::plugin_t *plugin_ptr;

    plugin_ptr = PluginRegistry<IRes,ID>::global_registry().query(id);

    if(!plugin_ptr)
      {
	// plugin does not exist
	m_last_error.first = plugin_not_registered;
	m_last_error.second = to_string(id) + ": plugin not registred";
	return false;
      }

    if(!plugin_ptr->may_delete())
      {
	// share violation - somebody is still using the plugin
	m_last_error.first = plugin_share_violation;
	m_last_error.second = to_string(id) + ": plugin not registred";
	return false;
      }

    uri_string_t uri = m_locator.getURI(id);

    if(uri.empty())
      {
	m_last_error.first = component_not_found;
	m_last_error.second = to_string(id) + ": component not found";
	return false;
      }

    typename comp_map_t::iterator pos = m_loaded_components.find(uri);

    if(pos==m_loaded_components.end())
      {
	m_last_error.first = component_not_found;
	m_last_error.second = to_string(id) + ": component not loaded";
      }

# ifndef _WIN32
    if(dlclose(pos->second) != 0)
      {
	m_last_error.first = system_error;
	m_last_error.second = to_string(id) + ": " + dlerror();	
	return false;
      }
#else 
    if (! FreeLibrary ((HINSTANCE) pos->second) )
      {
	m_last_error.first = system_error;
	m_last_error.second = to_string(id) + ": " + to_string(GetLastError());	
	return false;
      }
#endif

    // verify that plugin really removed
    if(PluginRegistry<IRes,ID>::global_registry().query(id))
      {
	m_last_error.first = plugin_share_violation;
	m_last_error.second = to_string(id) + ": unable to unload component (link time shlib?)";
	return false;
      }

# if 0
    if(--pos->second.second == 0)
      m_loaded_components.erase(pos);
# endif

    m_loaded_components.erase(pos);

    return true;
  }
  
  template<class IRes, class ID, class OPT>
  std::pair<typename PluginLoader<IRes,ID, OPT>::err_t, std::string> PluginLoader<IRes,ID, OPT>::lastError()
  {
    return m_last_error;
  }

  template<class IRes, class ID, class OPT>
  bool PluginLoader<IRes,ID, OPT>::_streamIn(std::istream& is)
  {
    if(!is.good())
      return false;

  #ifdef USE_DATAXML

    DataXML::InputXMLStream xml_is(is);

    const DataXML::DataObject& d_loader = xml_is.read();

    if(!xml_is.success()) return false;

    if(d_loader.name() == "PluginLoader")
      {
	std::vector<DataXML::DataObject>::const_iterator entry;

	for(entry = d_loader.children().begin(); entry!=d_loader.children().end(); ++entry)
	  if(entry->name() == "Plugin")
	    {
	      ID id;
	      
	      if(!to_value(entry->getAttributeValue("id"),id))
		return false;
	      
	      if(!loadPluginComponent(id))
		{
		  INFO_MSG("PluginLoader (stream-in): " + to_string(id) + " " + lastError().second);
		}
	    }
	  else
	    return false;

	return true;
      }

#endif
    return false;
  }


}
