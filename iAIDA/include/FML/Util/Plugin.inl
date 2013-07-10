# include "util.h"
# include "debug.h"

# include <algorithm>
# include <functional>

namespace Util
{

template<class IRes, class ID, class PointerPolicy, class OPT>
AbstractPlugin<IRes,ID,PointerPolicy,OPT>::AbstractPlugin(ID id) : m_detached(false), m_id(id)
{
  // DBG_STRING("creating plugin : " + to_string(id));

  if(!PluginRegistry<IRes,ID, PointerPolicy,OPT>::global_registry().add(this))
    {
      INFO_MSG( to_string(id) + ": plugin already registered (warning)");
    }
}

template<class IRes, class ID, class PointerPolicy, class OPT>
AbstractPlugin<IRes,ID,PointerPolicy,OPT>::AbstractPlugin(ID id, const OPT& opt) : m_detached(false), m_id(id), m_opt(opt)
{
  // DBG_STRING("creating plugin : " + to_string(id));

  if(!PluginRegistry<IRes,ID, PointerPolicy,OPT>::global_registry().add(this))
    {
      INFO_MSG( to_string(id) + ": plugin already registered (warning)");
    }
}


template<class IRes, class ID, class PointerPolicy, class OPT>
AbstractPlugin<IRes,ID,PointerPolicy,OPT>::~AbstractPlugin()
{
  if(!may_delete())
    {
      INFO_MSG(to_string(id()) + "this plugin is still in use but getting deleted now, expect troubles (error in client code)");
    }

  if(!m_detached)
    PluginRegistry<IRes,ID,PointerPolicy,OPT>::global_registry().remove(this);

}

template<class IRes,class ID, class PointerPolicy, class OPT>
PluginRegistry<IRes,ID,PointerPolicy,OPT>::PluginRegistry() 
{
  //  DBG_STRING("plugin registry constructed!");
}

template<class IRes,class ID, class PointerPolicy, class OPT>
PluginRegistry<IRes,ID,PointerPolicy,OPT>::~PluginRegistry() 
{
  //  DBG_STRING("plugin registry destructed!");

  typename pluginmap_t::iterator it = m_map.begin();

  for(; it!=m_map.end(); ++it)
    it->second->detach();
}

template<class IRes,class ID, class PointerPolicy, class OPT>
bool PluginRegistry<IRes,ID,PointerPolicy,OPT>::add(plugin_t * p)
{
  if(!p) return false;

  return m_map.insert(make_pair(p->id(),p)).second;
}

template<class IRes,class ID, class PointerPolicy, class OPT>
bool PluginRegistry<IRes,ID,PointerPolicy,OPT>::remove(plugin_t * p )
{
  if(!p) return false;

  return m_map.erase(p->id()) != 0;
}

template<class IRes,class ID, class PointerPolicy, class OPT>
typename PluginRegistry<IRes,ID,PointerPolicy,OPT>::plugin_t* PluginRegistry<IRes,ID,PointerPolicy,OPT>::query(const ID& id) 
{
  typename pluginmap_t::iterator pos = m_map.find(id);
  return pos!=m_map.end() ? pos->second : 0;
}


template <class T1, class T2>
inline
T1 first_of_pair(std::pair<T1,T2> pr)
{
  return pr.first;
}


template <class T1, class T2>
inline
T2 second_of_pair(std::pair<T1,T2> pr)
{
  return pr.second;
}


template <class InputIterator, class OutputIterator>
OutputIterator alt_transform (InputIterator first, InputIterator last,
                          OutputIterator result)
{ 
  while (first != last) *result++ = second_of_pair(*first++);
  return result;
}

template<class IRes,class ID, class PointerPolicy, class OPT>
std::vector<typename PluginRegistry<IRes,ID,PointerPolicy,OPT>::plugin_t *> PluginRegistry<IRes,ID,PointerPolicy,OPT>::get_vector() 
{
  std::vector<plugin_t*> v;
  if ( m_map.size() > 0 ) v.reserve( m_map.size() );
  alt_transform(m_map.begin(), m_map.end(), std::back_inserter(v));
  return v;
}


template<class IRes,class ID, class PointerPolicy, class OPT>
extern PluginRegistry<IRes,ID,PointerPolicy,OPT> & initialized_engine_registry()
{
  static PluginRegistry<IRes,ID,PointerPolicy,OPT> the_engine_registry;

  return the_engine_registry;
}

template<class IRes,class ID, class PointerPolicy, class OPT>
PluginRegistry<IRes,ID,PointerPolicy,OPT> &PluginRegistry<IRes,ID,PointerPolicy,OPT>::global_registry()
{
  return initialized_engine_registry<IRes,ID,PointerPolicy,OPT>();
}

}
