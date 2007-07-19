# ifndef UtilPlugin_h__included__
# define UtilPlugin_h__included__

# include <map>
# include <vector>
# include <string>

/**
 * Plugin Mechanism,
 *
 * \author Jakub T. Moscicki, CERN 2002
 */

/**
 * \page plugin_mechanisms Plugin Mechanisms Details
 *
 * Plugin machanisms may be used to:
 *  - handle plugins by name
 *  - verify version numbers of shared libraries before getting core dumps
 *
 * BUG INFO:
 * g++ segfaults if no link time libraries present and trying to load run-time
 * libraries
 * apparently, if any link time library is present then loading other run-time
 * libs is OK.
 * the difference for PluginRegistry told by nm is as follows:
 * 1) app has at least one link-time lib: nm -D gives
 *    W Util::PluginRegistry
 *    so PluginRegistry is a weak DYNAMIC symbol
 * 2) app has no link-time lib: nm -D gives nothing
 *    so PluginRegistry is a weak STATIC symbol
 *
 * In case 1) initialized_registry() refers to the same static object
 * In case 2) there are multiple static objects and seg-fault.
 *
 * Library has always weak DYNAMIC symbol.
 *
 * Solutions for gcc:
 * 1) add -export-dynamic flag to compile the application (tested on Linux)
 * 2) add link-time dummy library for each type of PluginRegistry
 *
 */

namespace Util
{

  /** 
   * \class PointerPolicy
   * PointerPolicy concept for template arguments. Supports two types:
   * T is a base type, Impl is implementation type. Typedef trait ptr_t define
   * the type of the smart pointer implementing the policy.
   *
   * PointerPolicy acts as a factory for Impl objects but manages the pointers
   * to these objects in its own specific way. So, it may implemenent 
   * mechanisms to protect parent object from being deleted if Impl objects
   * still exist. Parent object in this context is the object which 
   * created Impl objects by calling _new on this policy.
   *
   * Refer to PlainPointerPolicy for details and example.
   */

  /**
   * PlainPointerPolicy implements the simplest PointerPolicy: 
   * plain new, delete and plain pointers, without checking for may_delete.
   * You may model your own policy classes after PlainPointerPolicy.
   */
template <class T>
class PlainPointerPolicy
{
public:
  /// smart pointer type (plain pointer here).
  typedef T* ptr_t;       

  /// plain pointer type (T*).
  typedef T* plain_ptr_t;

  /// create new Impl object with default constructor and return via T pointer.
  /// \warning
  /// argument is just a fake pointer to get around problem in gcc 2.95.2 
  /// with explicit instantiation of member templates inside other templates.
  /// this line would work just fine but not inside the template:
  /// void* v =  PlainPointerPolicy<void>()._new<int>();

  template<class Impl>
  ptr_t _new(Impl*) 
  {
    // for boost::shared_ptr one needs to store a vector of pointers here
    return new Impl;
  }

  /// delete object allocated by _new().
  void _delete(ptr_t ptr)
  {
    delete ptr;
  }

  /// check if can safely delete the parent object 
  /// (no check here, always true). parent object in this context is 
  /// the object which created Impl objects by calling _new on this policy.
  bool _may_delete()
  {
    return true; 
  }

};


/**
 * AbstractPlugin is an abstract base class which associates the identifier
 * with the resource specific to the plugin implementation.
 * IRes_ptr is a pointer type of the resource (may be to abstract interface), 
 * ID is a key-type for std::map.
 * OPT is an optional information type.
 * PointerPolicy specifies the tracking policy for dynamically created 
 * resources. By default use PlainPointerPolicy<IRes>.
 *
 * \todo
 * If dynamic unload present, then AbstractPlugin on destroy should call 
 * back every IRes to self-destroy too (not implemented) -> IRes may be 
 * created in different library address spaces.
 */
template<class IRes, class ID = std::string, class PointerPolicy = PlainPointerPolicy<IRes>, class OPT = std::string >
class AbstractPlugin
{
public:
  /// self-register (add link to this to the registry).
  explicit AbstractPlugin(ID id);

  /// self-register (add link to this to the registry).
  AbstractPlugin(ID id, const OPT& opt);

  /// self-unregister (remove link to this from the registry).
  virtual ~AbstractPlugin();

  /// create resource (to be implemented in subclass).
  virtual typename PointerPolicy::ptr_t create() = 0;

  /// get id.
  ID id() const { return m_id;}

  /// get optional info.
  const OPT& opt() const { return m_opt; }

  /// when plugin registry dies, it explicitly detaches this plugin.
  /// so plugin does not call back registry anymore (does not self unregister).
  /// Note: plugin registry is not intended to die before the plugins and 
  /// proper physical organization of components should not allow this 
  /// (it does not make too much sense anyway). However, cleanup 
  /// mechanisms of static objects in C++ does not provide a way to force 
  /// ordering.
  void detach() { m_detached = true; }

  /// true if plugin may be safely deleted from program's address space.
  /// this is implemented by selected pointer policy, which may keep track
  /// of resource references in the client code.
  bool may_delete() { return m_policy._may_delete(); }


protected:
  /// pointer tracking policy.
  PointerPolicy m_policy;

private:
  bool m_detached;
  ID m_id;
  OPT m_opt;
};

/**
 * Plugin implementation template for statically-known ResImpl_type.
 * 
 */
template<class IRes, class ResImpl_type, class ID = std::string, class PointerPolicy = PlainPointerPolicy<IRes>, class OPT = std::string >
class Plugin : virtual public AbstractPlugin<IRes,ID, PointerPolicy>
{
public:
  Plugin(ID id) : AbstractPlugin<IRes,ID, PointerPolicy,OPT>(id) {}
  Plugin(ID id, const OPT& opt) : AbstractPlugin<IRes,ID, PointerPolicy,OPT>(id,opt) {}
  ~Plugin() {}

  /// create a new resource object of type ResImpl_type using default 
  /// constructor via given PointerPolicy (class template argument).
  virtual typename PointerPolicy::ptr_t create() 
  { 
    // getting around gcc 2.95.2 problem with the call to
//     PlainPointerPolicy<IRes>::_new<ResImpl_type>(); // (syntax error)
//     //static policy impl:
//     ResImpl_type *__fake;
//     return PointerPolicy<IRes>::_new(__fake);

    ResImpl_type *__fake = 0;
    // ported to gcc4
    return this->m_policy._new(__fake);
  }
};

/**
 * Singleton plugin registry for IRes plugins using given ID and PointerPolicy
 * types.
 *
 * Every plugin object (derived from AbstractPlugin<IRes,ID,PointerPolicy>) 
 * automatically registers here and may be found later by its id.
 */
template<class IRes, class ID = std::string, class PointerPolicy = PlainPointerPolicy<IRes>, class OPT = std::string >
class PluginRegistry
{
public:
  PluginRegistry();
  ~PluginRegistry();

  typedef AbstractPlugin<IRes,ID,PointerPolicy,OPT> plugin_t;

  /// add new link to the plugin, not owned (just associated).
  bool add(plugin_t *);

  /// remove link.
  bool remove(plugin_t *);

  /// query for plugin by ID.
  plugin_t* query(const ID& id);

  // get vector with all plugins.
  std::vector<plugin_t*> get_vector();
  
  /// access point to the registry.
  static PluginRegistry<IRes,ID,PointerPolicy,OPT> &global_registry();

private:

  typedef std::map<ID,plugin_t*> pluginmap_t; 

  pluginmap_t m_map;
};

}

# include "Plugin.inl"

# endif




